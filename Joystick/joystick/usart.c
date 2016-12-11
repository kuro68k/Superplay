/*
 * usart.c
 *
 * Created: 02/09/2015 09:27:28
 *  Author: Paul Qureshi
 *
 * USART comms using KBUS protocol, with main (shared with USB) and aux ports.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdbool.h>
#include "kbus.h"
#include "config.h"
#include "usart.h"


enum
{
	STATE_WAIT_FOR_START,
	STATE_READ_LENGTH,
	STATE_READ_DATA
};

#define BUFFER_SIZE		(sizeof(KBUS_PACKET_t) + 2)

volatile uint8_t main_rx_buffer_DMA[BUFFER_SIZE];
volatile uint8_t aux_rx_buffer_DMA[BUFFER_SIZE];
volatile uint8_t main_rx_SIG = 0;
volatile uint8_t aux_rx_SIG = 0;

uint8_t main_tx_buffer_DMA[BUFFER_SIZE];
uint8_t aux_tx_buffer_DMA[BUFFER_SIZE];


/**************************************************************************************************
** Reset DMA pointers
*/
inline void usart_reset_main_dma_rx_pointer(void)
{
	MAIN_TX_DMA_CH.DESTADDR0 = (uint16_t)main_rx_buffer_DMA & 0xFF;
	MAIN_TX_DMA_CH.DESTADDR1 = ((uint16_t)main_rx_buffer_DMA >> 8) & 0xFF;
	MAIN_TX_DMA_CH.DESTADDR2 = 0;
}

inline void usart_reset_aux_dma_rx_pointer(void)
{
	AUX_TX_DMA_CH.DESTADDR0 = (uint16_t)main_rx_buffer_DMA & 0xFF;
	AUX_TX_DMA_CH.DESTADDR1 = ((uint16_t)main_rx_buffer_DMA >> 8) & 0xFF;
	AUX_TX_DMA_CH.DESTADDR2 = 0;
}

inline void usart_reset_main_dma_tx_pointer(void)
{
	MAIN_TX_DMA_CH.SRCADDR0 = (uint16_t)main_tx_buffer_DMA & 0xFF;
	MAIN_TX_DMA_CH.SRCADDR1 = ((uint16_t)main_tx_buffer_DMA >> 8) & 0xFF;
	MAIN_TX_DMA_CH.SRCADDR2 = 0;
}

inline void usart_reset_aux_dma_tx_pointer(void)
{
	AUX_TX_DMA_CH.SRCADDR0 = (uint16_t)aux_tx_buffer_DMA & 0xFF;
	AUX_TX_DMA_CH.SRCADDR1 = ((uint16_t)aux_tx_buffer_DMA >> 8) & 0xFF;
	AUX_TX_DMA_CH.SRCADDR2 = 0;
}

/**************************************************************************************************
** Set up USARTs
*/
void USART_init(void)
{
	// USARTs	
	USART_t *u = &MAIN_USART;
	
	for (uint8_t i = 0; i < 2; i++)
	{
		u->CTRLA = 0;
		u->CTRLB = USART_CLK2X;
		u->CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
		u->BAUDCTRLA = USART_BSEL & 0xFF;
		u->BAUDCTRLB = ((USART_BSCALE << 4) & 0xF0) | ((USART_BSEL >> 8) & 0x0F);
		u = &AUX_USART;
		if (cfg->aux_mode != AUX_MODE_KBUS)
			break;
	}
	
	if (cfg->aux_mode == AUX_MODE_KBUS)
	{
		AUX_USART.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
		AUX_USART.CTRLA |= USART_RXCINTLVL_HI_gc;
	}


	// RX DMA
	DMA_CH_t *ch = &MAIN_RX_DMA_CH;
	for (uint8_t i = 0; i < 2; i++)
	{
		ch->CTRLA = DMA_CH_RESET_bm;
		asm("nop");
		ch->CTRLA = DMA_CH_BURSTLEN_1BYTE_gc;
		ch->ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc | DMA_CH_SRCDIR_FIXED_gc |
					  DMA_CH_DESTRELOAD_TRANSACTION_gc | DMA_CH_DESTDIR_INC_gc;
		ch->TRFCNT = BUFFER_SIZE;	// maximum packet size
		ch->REPCNT = 0;
		
		ch = &AUX_RX_DMA_CH;
		if (cfg->aux_mode != AUX_MODE_KBUS)
			break;
	}

	MAIN_TX_DMA_CH.TRIGSRC = MAIN_RX_DMA_TRIGSRC;
	MAIN_TX_DMA_CH.SRCADDR0 = (uint16_t)&MAIN_USART.DATA & 0xFF;
	MAIN_TX_DMA_CH.SRCADDR1 = ((uint16_t)&MAIN_USART.DATA >> 8) & 0xFF;
	MAIN_TX_DMA_CH.SRCADDR2 = 0;
	usart_reset_main_dma_rx_pointer();

	if (cfg->aux_mode == AUX_MODE_KBUS)
	{
		AUX_TX_DMA_CH.TRIGSRC = MAIN_RX_DMA_TRIGSRC;
		AUX_TX_DMA_CH.SRCADDR0 = (uint16_t)&MAIN_USART.DATA & 0xFF;
		AUX_TX_DMA_CH.SRCADDR1 = ((uint16_t)&MAIN_USART.DATA >> 8) & 0xFF;
		AUX_TX_DMA_CH.SRCADDR2 = 0;
		usart_reset_aux_dma_rx_pointer();
	}

	// TX DMA
	ch = &MAIN_TX_DMA_CH;
	for (uint8_t i = 0; i < 2; i++)
	{
		ch->CTRLA = DMA_CH_RESET_bm;
		asm("nop");
		ch->CTRLA = DMA_CH_BURSTLEN_1BYTE_gc;
		ch->ADDRCTRL = DMA_CH_SRCRELOAD_TRANSACTION_gc | DMA_CH_SRCDIR_INC_gc |
					  DMA_CH_DESTRELOAD_BURST_gc | DMA_CH_DESTDIR_FIXED_gc;
		ch->TRFCNT = 4;	// minimum packet size
		ch->REPCNT = 0;
		
		ch = &AUX_TX_DMA_CH;
		if (cfg->aux_mode != AUX_MODE_KBUS)
			break;
	}

	MAIN_TX_DMA_CH.TRIGSRC = MAIN_TX_DMA_TRIGSRC;
	MAIN_TX_DMA_CH.DESTADDR0 = (uint16_t)&MAIN_USART.DATA & 0xFF;
	MAIN_TX_DMA_CH.DESTADDR1 = ((uint16_t)&MAIN_USART.DATA >> 8) & 0xFF;
	MAIN_TX_DMA_CH.DESTADDR2 = 0;
	usart_reset_main_dma_tx_pointer();

	if (cfg->aux_mode == AUX_MODE_KBUS)
	{
		AUX_TX_DMA_CH.TRIGSRC = AUX_TX_DMA_TRIGSRC;
		AUX_TX_DMA_CH.DESTADDR0 = (uint16_t)&AUX_USART.DATA & 0xFF;
		AUX_TX_DMA_CH.DESTADDR1 = ((uint16_t)&AUX_USART.DATA >> 8) & 0xFF;
		AUX_TX_DMA_CH.DESTADDR2 = 0;
		usart_reset_aux_dma_tx_pointer();
	}

	// RX timers
	TC0_t *tc = (TC0_t *)&MAIN_RX_TC;
	for (uint8_t i = 0; i < 2; i++)
	{
		tc->CTRLA = 0;
		tc->CTRLB = 0;
		tc->CTRLC = 0;
		tc->CTRLD = TC_TC0_EVACT_RESTART_gc;	// reset counter to zero when RX pin changes
		tc->CTRLE = 0;
		tc->INTCTRLA = TC_TC0_OVFINTLVL_HI_gc;	// avoid spurious end of frame detection
		tc->INTCTRLB = TC_TC0_CCAINTLVL_LO_gc;	// detect end of frame
		tc->INTCTRLB = 0;
		tc->INTFLAGS = 0xFF;					// clear all flags
		tc->PER = 0xFFFF;
		tc->CCA = USART_TC_CCA;					// end of frame detection
		tc->CNT = USART_TC_CCA+1;				// avoid triggering after starting timer
		tc->CTRLA = USART_TC_DIV;				// start timer
		
		tc = &AUX_RX_TC;
		if (cfg->aux_mode != AUX_MODE_KBUS)
			break;
	}
	
	EVSYS.CH0CTRL = 0;
	EVSYS.CH0MUX = MAIN_RX_EVENT_CHMUX;
	MAIN_RX_TC.CTRLD |= TC_TC0_EVSEL_CH0_gc;

	if (cfg->aux_mode == AUX_MODE_KBUS)
	{
		EVSYS.CH1CTRL = 0;
		EVSYS.CH1MUX = AUX_RX_EVENT_CHMUX;
		AUX_RX_TC.CTRLD |= TC_TC0_EVSEL_CH1_gc;
	}
}

/**************************************************************************************************
** USART timer overflow interrupts. Avoid spurious end of frame detection.
*/
ISR(MAIN_RX_TC_OVF_vect)
{
	MAIN_RX_TC.CNT = USART_TC_CCA+1;
}

ISR(AUX_RX_TC_OVF_vect)
{
	AUX_RX_TC.CNT = USART_TC_CCA+1;
}

/**************************************************************************************************
** USART timer compare interrupts. End of frame detection.
*/
ISR(MAIN_RX_TC_CCA_vect)
{
	main_rx_SIG = 0xFF;
}

ISR(AUX_RX_TC_CCA_vect)
{
	aux_rx_SIG = 0xFF;
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
void usart_add_crc(uint8_t *buffer)
{
	uint8_t		len = buffer[1];
	len += 2;	// add command and length
	uint16_t	*crc = (uint16_t *)&buffer[len];
	
	if (len > KBUS_PACKET_DATA_SIZE)	// sanity check
		return;
	
	CRC.CTRL = CRC_RESET_RESET1_gc;
	asm("nop");
	CRC.CTRL = CRC_SOURCE_IO_gc;
	for (uint8_t i = 0; i < len; i++)
		CRC.DATAIN = buffer[i];
	*crc = (CRC.CHECKSUM1 << 8) | CRC.CHECKSUM0;
}

/**************************************************************************************************
** Handling incoming commands
*/
void USART_run(void)
{
	// main
	if (main_rx_SIG != 0)
	{
		main_rx_SIG = 0;
		MAIN_RX_DMA_CH.CTRLA &= ~DMA_CH_ENABLE_bm;
		if (usart_check_crc((uint8_t*)main_rx_buffer_DMA) && !(MAIN_TX_DMA_CH.CTRLB & DMA_CH_CHBUSY_bm))	// don't send if previous TX still in progress)
		{
			KBUS_process_command((KBUS_PACKET_t *)main_rx_buffer_DMA, (KBUS_PACKET_t *)main_tx_buffer_DMA);
			usart_add_crc(main_tx_buffer_DMA);
			usart_reset_main_dma_tx_pointer();
			MAIN_TX_DMA_CH.TRFCNT = main_tx_buffer_DMA[1] + 2;	// data length + command + length
			MAIN_TX_DMA_CH.CTRLA |= DMA_CH_ENABLE_bm;
		}
		while (MAIN_RX_DMA_CH.CTRLA & DMA_CH_ENABLE_bm)	// wait for DMA to disable
			MAIN_RX_DMA_CH.CTRLA &= ~DMA_CH_ENABLE_bm;
		usart_reset_main_dma_rx_pointer();
		MAIN_RX_DMA_CH.CTRLA |= DMA_CH_ENABLE_bm;
	}

	// aux
	if (aux_rx_SIG != 0)
	{
		aux_rx_SIG = 0;
	}
}
