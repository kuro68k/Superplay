/*
 * usart.c
 *
 * USART comms using KBUS protocol, with main (shared with USB) and aux ports.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
#include "global.h"
#include "kbus.h"
#include "config.h"
#include "hw_misc.h"
#include "usart.h"

// peripherals
#define DMA_RX_CH			DMA.CH0
#define DMA_RX_CH_vect		DMA_CH0_vect
#define DMA_TX_CH			DMA.CH1
#define RX_TC				TCD1
#define RX_TC_OVF_vect		TCD1_OVF_vect
#define RX_TC_CCA_vect		TCD1_CCA_vect
#define RX_EVENT_CHMUX		EVSYS_CHMUX_PORTD_PIN6_gc
#define RX_TC_DIV			TC_CLKSEL_DIV1_gc
#define RX_TC_TIMEOUT		0x01DF		// 20us @ 24MHz

// 2Mbaud @ 16MHz
#define USART_BSEL			0
#define USART_BSCALE		0
#define USART_CLK2X			USART_CLK2X_bm


enum
{
	STATE_WAIT_FOR_START,
	STATE_READ_LENGTH,
	STATE_READ_DATA
};

#define RX_BUFFER_SIZE		(sizeof(KBUS_PACKET_t) + 2)		// 2 byte CRC
#define TX_BUFFER_SIZE		(sizeof(KBUS_PACKET_t) + 4)		// 2 byte preamble, 2 byte CRC

volatile uint8_t rx_buffer_DMA[RX_BUFFER_SIZE];
volatile uint8_t rx_sof_flag_SIG = 0;
volatile uint8_t rx_complete_SIG = 0;

uint8_t tx_buffer[TX_BUFFER_SIZE] = { 0xFF, 0xFF };			// 0xFF == preamble
KBUS_PACKET_t *tx_packet = (KBUS_PACKET_t *)&tx_buffer[2];


/**************************************************************************************************
** Reset DMA pointers
*/
static inline void usart_reset_dma_rx_pointer(void)
{
	DMA_RX_CH.DESTADDR0 = (uint16_t)rx_buffer_DMA & 0xFF;
	DMA_RX_CH.DESTADDR1 = ((uint16_t)rx_buffer_DMA >> 8) & 0xFF;
	DMA_RX_CH.DESTADDR2 = 0;
}

static inline void usart_reset_dma_tx_pointer(void)
{
	DMA_TX_CH.SRCADDR0 = (uint16_t)tx_buffer & 0xFF;
	DMA_TX_CH.SRCADDR1 = ((uint16_t)tx_buffer >> 8) & 0xFF;
	DMA_TX_CH.SRCADDR2 = 0;
}

/**************************************************************************************************
** Set up USARTs
*/
void USART_init(void)
{
	// USART
	USARTD1.CTRLA = 0;
	USARTD1.CTRLB = USART_CLK2X;
	USARTD1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
	USARTD1.BAUDCTRLA = USART_BSEL & 0xFF;
	USARTD1.BAUDCTRLB = ((USART_BSCALE << 4) & 0xF0) | ((USART_BSEL >> 8) & 0x0F);

	// RX DMA
	DMA_RX_CH.CTRLA = DMA_CH_RESET_bm;
	asm("nop");
	DMA_RX_CH.CTRLA = DMA_CH_BURSTLEN_1BYTE_gc;
	DMA_RX_CH.CTRLB = DMA_CH_TRNINTLVL_LO_gc;
	DMA_RX_CH.ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc | DMA_CH_SRCDIR_FIXED_gc |
						 DMA_CH_DESTRELOAD_BURST_gc | DMA_CH_DESTDIR_INC_gc;
	DMA_RX_CH.TRFCNT = RX_BUFFER_SIZE;
	DMA_RX_CH.REPCNT = 0;

	DMA_RX_CH.TRIGSRC = DMA_CH_TRIGSRC_USARTD1_RXC_gc;
	DMA_RX_CH.SRCADDR0 = (uint16_t)&USARTD1.DATA & 0xFF;
	DMA_RX_CH.SRCADDR1 = ((uint16_t)&USARTD1.DATA >> 8) & 0xFF;
	DMA_RX_CH.SRCADDR2 = 0;
	usart_reset_dma_rx_pointer();

	// TX DMA
	DMA_TX_CH.CTRLA = DMA_CH_RESET_bm;
	asm("nop");
	DMA_TX_CH.CTRLA = DMA_CH_BURSTLEN_1BYTE_gc | DMA_CH_SINGLE_bm;
	DMA_TX_CH.ADDRCTRL = DMA_CH_SRCRELOAD_TRANSACTION_gc | DMA_CH_SRCDIR_INC_gc |
						 DMA_CH_DESTRELOAD_TRANSACTION_gc | DMA_CH_DESTDIR_FIXED_gc;
	DMA_TX_CH.TRFCNT = TX_BUFFER_SIZE;
	DMA_TX_CH.REPCNT = 0;

	DMA_TX_CH.TRIGSRC = DMA_CH_TRIGSRC_USARTD1_DRE_gc;
	DMA_TX_CH.DESTADDR0 = (uint16_t)&USARTD1.DATA & 0xFF;
	DMA_TX_CH.DESTADDR1 = ((uint16_t)&USARTD1.DATA >> 8) & 0xFF;
	DMA_TX_CH.DESTADDR2 = 0;
	usart_reset_dma_tx_pointer();

	// RX timers
	RX_TC.CTRLA = 0;
	RX_TC.CTRLB = 0;
	RX_TC.CTRLC = 0;
	RX_TC.CTRLD = TC_EVACT_RESTART_gc;		// reset counter to zero when RX pin changes
	RX_TC.CTRLE = 0;
	RX_TC.INTCTRLA = TC_OVFINTLVL_HI_gc;	// avoid spurious end of frame detection
	RX_TC.INTCTRLB = TC_CCAINTLVL_LO_gc;	// detect end of frame
	RX_TC.INTCTRLB = 0;
	RX_TC.INTFLAGS = 0xFF;					// clear all flags
	RX_TC.PER = RX_TC_TIMEOUT;
	RX_TC.CNT = 0;

	EVSYS.CH0CTRL = 0;
	EVSYS.CH0MUX = RX_EVENT_CHMUX;
	RX_TC.CTRLD |= TC_EVSEL_CH0_gc;
}

/**************************************************************************************************
** Reset RX to get a new packet
*/
void usart_reset_rx(void)
{
	rx_complete_SIG = 0;
	RX_TC.CTRLA = 0;
	RX_TC.INTCTRLA = TC0_OVFIF_bm;		// clear interrupt flag
	rx_sof_flag_SIG = 0;
	while (DMA_RX_CH.CTRLA & DMA_CH_ENABLE_bm)
		DMA_RX_CH.CTRLA &= ~DMA_CH_ENABLE_bm;
	DMA_RX_CH.DESTADDR0 = (uint16_t)&rx_buffer_DMA[1] & 0xFF;	// 1st byte handled by USART RX interrupt
	DMA_RX_CH.DESTADDR1 = ((uint16_t)&rx_buffer_DMA[1] >> 8) & 0xFF;
	DMA_RX_CH.DESTADDR2 = 0;
	DMA_RX_CH.TRFCNT = sizeof(KBUS_PACKET_t) - 1 + 2;	// 1 byte received by interrupt, 2 byte CRC
	USARTD1.CTRLA |= (USARTD1.CTRLA & ~USART_RXCINTLVL_gm) | USART_RXCINTLVL_HI_gc;
}

/**************************************************************************************************
** USART timeout interrupt
*/
ISR(RX_TC_OVF_vect)
{
	usart_reset_rx();
}

/**************************************************************************************************
** USART RX interrupts. Skip preamble bytes and then start DMA
*/
ISR(USARTD1_RXC_vect)
{
	uint8_t b = USARTD1.DATA;

	if ((rx_sof_flag_SIG == 0) && (b == 0xFF))		// wait for preamble to start
	{
		rx_sof_flag_SIG = 0xFF;
		return;
	}

	if (b == 0xFF)									// wait for preamble to end
		return;

	DMA_RX_CH.CTRLA |= DMA_CH_ENABLE_bm;
	RX_TC.CNT = 0;
	RX_TC.CTRLA = RX_TC_DIV;						// start timeout counter

	rx_buffer_DMA[0] = b;
	USARTD1.CTRLA &= ~USART_RXCINTLVL_gm;
}

/**************************************************************************************************
**
*/
ISR(DMA_RX_CH_vect)
{
	rx_complete_SIG = 0xFF;
}

/**************************************************************************************************
** Check CRC on RX buffer
*/
bool usart_check_crc(const uint8_t *buffer)
{
	uint8_t		len = buffer[1];
	len += 2;	// add command and length
	uint16_t	crc = *(uint16_t *)&buffer[len];

	if (len > KBUS_PACKET_DATA_SIZE)	// sanity check
		return false;

	CRC.CTRL = CRC_RESET_RESET1_gc;
	asm("nop");
	CRC.CTRL = CRC_SOURCE_IO_gc;
	for (uint8_t i = 0; i < len; i++)
		CRC.DATAIN = buffer[i];
	if (crc != ((CRC.CHECKSUM1 << 8) | CRC.CHECKSUM0))
		return false;
	return true;
}

/**************************************************************************************************
** Add CRC to TX buffer
*/
void usart_add_tx_crc(void)
{
	uint16_t	*crc = (uint16_t *)&tx_buffer[2 + sizeof(KBUS_PACKET_t)];	// preamble + packet

	CRC.CTRL = CRC_RESET_RESET1_gc;
	asm("nop");
	CRC.CTRL = CRC_SOURCE_IO_gc;
	for (uint8_t i = 2; i < sizeof(KBUS_PACKET_t) + 2; i++)
		CRC.DATAIN = tx_buffer[i];
	*crc = (CRC.CHECKSUM1 << 8) | CRC.CHECKSUM0;
}

/**************************************************************************************************
** Look for USART connection to KBUS. Returns true if found
*/
bool USART_check_for_bus(void)
{
	HW_reset_rtc();
	USARTD1.CTRLB |= USART_RXEN_bm;
	uint32_t rx = 0;

	do
	{
		if (USARTD1.STATUS & USART_RXCIF_bm)
		{
			rx <<= 8;
			rx |= USARTD1.DATA;
			if (rx == LE_CHR('K', 'B', 'U', 'S'))
			{
				// send response
				while (DMA_TX_CH.CTRLB & DMA_CH_CHBUSY_bm);
				memcpy_P((void *)tx_buffer, PSTR("\xFF\xFFREDY"), 6);
				DMA_TX_CH.TRFCNT = 6;
				DMA_TX_CH.CTRLA |= DMA_CH_ENABLE_bm;

				return true;
			}
		}
	} while (RTC.CNT < 100);	// approx 100ms

	USARTD1.CTRLB &= ~USART_RXEN_bm;
	return false;
}

/**************************************************************************************************
** Handling incoming commands
*/
void USART_run(void)
{
	// main
	if (rx_complete_SIG != 0)
	{
		rx_complete_SIG = 0;

		if (usart_check_crc((uint8_t*)rx_buffer_DMA))
		{
			while (DMA_TX_CH.CTRLB & DMA_CH_CHBUSY_bm);	// wait for previous TX to finish
			KBUS_process_command((KBUS_PACKET_t *)rx_buffer_DMA, tx_packet);
			usart_add_tx_crc();
			//usart_reset_dma_tx_pointer();
			//DMA_TX_CH.TRFCNT = TX_BUFFER_SIZE;
			DMA_TX_CH.CTRLA |= DMA_CH_ENABLE_bm;
		}

		usart_reset_rx();
	}

	while (DMA_TX_CH.CTRLB & DMA_CH_CHBUSY_bm);
	KBUS_long_report(tx_packet);
	usart_add_tx_crc();
	//usart_reset_main_dma_tx_pointer();

	//MAIN_TX_DMA_CH.CTRLB |= DMA_CH_TRNIF_bm;
	DMA_TX_CH.TRFCNT = TX_BUFFER_SIZE;
	DMA_TX_CH.CTRLA |= DMA_CH_ENABLE_bm;
}
