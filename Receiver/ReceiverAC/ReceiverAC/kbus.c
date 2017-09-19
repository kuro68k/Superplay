/*
 * kbus.c
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
#include "fastmem.h"
#include "version.h"
#include "config.h"
#include "hw_misc.h"
#include "report.h"
#include "atari.h"
#include "kbus.h"

#define LE_CHR(a,b,c,d)		( ((uint32_t)(a)<<24) | ((uint32_t)(b)<<16) | ((c)<<8) | (d) )

#define RX_BUFFER_SIZE		(sizeof(KBUS_PACKET_t) + 2)

volatile uint8_t		rx_buffer_DMA[RX_BUFFER_SIZE];
volatile KBUS_PACKET_t	*rx_packet_DMA = (KBUS_PACKET_t *)rx_buffer_DMA;
volatile uint8_t		rx_sof_flag_SIG = 0;
volatile uint8_t		rx_complete_SIG = 0;
volatile uint8_t		timeout_SIG = 0;

KBUS_PACKET_t			packet;
KBUS_PACKET_t			mapping_storage;

#pragma region Transport Layer

/**************************************************************************************************
* Transmit a byte
*/
static inline void kbus_tx(uint8_t byte)
{
	while (KBUS_USART.STATUS & USART_DREIF_bm);
	KBUS_USART.DATA = byte;
}

/**************************************************************************************************
** Reset RX to get a new packet
*/
void usart_reset_rx(void)
{
	rx_complete_SIG = 0;
	KBUS_TC.CTRLA = 0;
	KBUS_TC.INTCTRLA = TC4_OVFIF_bm;		// clear interrupt flag
	rx_sof_flag_SIG = 0;
	while (KBUS_DMA_CH.CTRLA & EDMA_CH_ENABLE_bm)
		KBUS_DMA_CH.CTRLA &= ~EDMA_CH_ENABLE_bm;
	KBUS_DMA_CH.ADDRL = (uint16_t)&rx_buffer_DMA[1] & 0xFF;	// 1st byte handled by USART RX interrupt
	KBUS_DMA_CH.ADDRH = ((uint16_t)&rx_buffer_DMA[1] >> 8) & 0xFF;
	KBUS_DMA_CH.TRFCNT = sizeof(KBUS_PACKET_t) - 1 + 2;	// 1 byte received by interrupt, 2 byte CRC
	KBUS_USART.CTRLA |= (KBUS_USART.CTRLA & ~USART_RXCINTLVL_gm) | USART_RXCINTLVL_HI_gc;
}

/**************************************************************************************************
* Set up K-BUS interface. RX DMA configured but not enabled.
*/
void KBUS_init(void)
{
	// USART
	KBUS_PORT.DIRCLR = KBUS_RX_PIN_bm;
	KBUS_PORT.DIRSET = KBUS_TX_PIN_bm;
	KBUS_PORT.KBUS_RX_PINCTRL = (KBUS_PORT.KBUS_RX_PINCTRL & ~PORT_ISC_gm) | PORT_ISC_FALLING_gc;	// event triggered on falling edge

	KBUS_USART.CTRLA = USART_RXCINTLVL_MED_gc;
	KBUS_USART.BAUDCTRLB = (uint8_t)((uint8_t)BSCALE << 4) | ((uint16_t)BSEL >> 8);
	KBUS_USART.BAUDCTRLA = (uint8_t)BSEL;
	KBUS_USART.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
	KBUS_USART.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;

	// DMA from USART
	KBUS_DMA_CH.CTRLA = EDMA_CH_RESET_bm;
	NOP();
	KBUS_DMA_CH.CTRLA = 0;
	KBUS_DMA_CH.CTRLB = EDMA_CH_TRNINTLVL0_bm; //EDMA_CH_TRNINTLVL_LO_gc;
	KBUS_DMA_CH.ADDRCTRL = EDMA_CH_RELOAD_BLOCK_gc | EDMA_CH_DIR_INC_gc;
	KBUS_DMA_CH.TRIGSRC = KBUS_DMA_TRIGGER_SRC;
	usart_reset_rx();

	// incoming character detection event
	KBUS_EVENT_CTRL = KBUS_EVENT_MUX;

	// timeout
	KBUS_TC.CTRLA = 0;
	KBUS_TC.CTRLB = 0;
	KBUS_TC.CTRLC = 0;
	KBUS_TC.CTRLD = TC45_EVACT_RESTART_gc | KBUS_EVSEL;
	KBUS_TC.CTRLE = 0;
	KBUS_TC.INTCTRLA = TC45_OVFINTLVL_LO_gc;
	KBUS_TC.INTCTRLB = 0;
	KBUS_TC.CNT = 0;
	KBUS_TC.PER = 0x1387;	// timeout, 10mS @ 32MHz/64
	KBUS_TC.CTRLA = TC45_CLKSEL_DIV64_gc;
}

/**************************************************************************************************
* USART RX interrupt, handles preamble
*/
ISR(KBUS_USART_RXC_vect)
{
	uint8_t b = KBUS_USART.DATA;

	if ((rx_sof_flag_SIG == 0) && (b == 0xFF))		// wait for preamble to start
	{
		rx_sof_flag_SIG = 0xFF;
		return;
	}

	if (b == 0xFF)									// wait for preamble to end
		return;

	KBUS_DMA_CH.CTRLA |= EDMA_CH_ENABLE_bm;
	rx_buffer_DMA[0] = b;
	KBUS_USART.CTRLA &= ~USART_RXCINTLVL_gm;
}

/**************************************************************************************************
* DMA complete interrupt
*/
ISR(KBUS_DMA_vect)
{
	rx_complete_SIG = 0xFF;
}

/**************************************************************************************************
* Handle timer overflows (controller not sending reports)
*/
ISR(KBUS_TC_OVF_vect)
{
	timeout_SIG = 0xFF;
}

/**************************************************************************************************
* Validate the packet in the input buffer. Command in the packet must be command + response bit
*/
bool kbus_validate_packet(uint8_t command)
{
	if (rx_packet_DMA->length > KBUS_PACKET_DATA_SIZE)
		return false;
	if (*(uint16_t *)&rx_buffer_DMA[sizeof(KBUS_PACKET_t)] != HW_crc16((void *)rx_buffer_DMA, sizeof(KBUS_PACKET_t)))
		return false;

	if (rx_packet_DMA->command != (command | RESPONSE_BIT_bm))
		return false;

	return true;
}

/**************************************************************************************************
* Send packet out over K-BUS, with preamble and trailing checksum word.
*/
void kbus_send_packet(const KBUS_PACKET_t * packet)
{
	kbus_tx(0xFF);	// preamble to aid UART sync/auto-baud
	kbus_tx(0xFF);

	CRC.CTRL = CRC_RESET1_bm;
	NOP();
	CRC.CTRL = CRC_SOURCE_IO_gc;

	uint8_t *buffer = (uint8_t *)packet;
	uint8_t bytes = sizeof(KBUS_PACKET_t);

	while (bytes--)
	{
		kbus_tx(*buffer);
		CRC.DATAIN = *buffer++;
	}

	kbus_tx(CRC.CHECKSUM0);		// CRC, little endian
	kbus_tx(CRC.CHECKSUM1);
}

/**************************************************************************************************
* Find a KBUS device to talk to
*/
void kbus_find_device(void)
{
	uint32_t rx = 0;
	const char tx_buffer[] = "\xFF\xFFKBUS";
	uint8_t i = 0;

	for(;;)
	{
		// send sync signal
		kbus_tx(tx_buffer[i++]);
		if (i >= sizeof(tx_buffer))
			i = 0;

		// check for responses from device
		if (KBUS_USART.STATUS & USART_RXCIF_bm)
		{
			rx <<= 8;
			rx |= KBUS_USART.DATA;
			if (rx == LE_CHR('R', 'E', 'D', 'Y'))
				break;
		}
	}
}

#pragma endregion

/**************************************************************************************************
* Send a KBUS command and get a response. Returns false if response not received before time-out.
* Response will be in rx_packet_DMA.
*/
bool kbus_command(KBUS_PACKET_t *cmd)
{
	uint8_t retries = 3;

	do
	{
		kbus_send_packet(cmd);

		// get response
		uint8_t rx_count = 0;
		do
		{
			if (timeout_SIG)	// device stopped sending
				break;

			if (rx_complete_SIG)
			{
				if (!kbus_validate_packet(cmd->command))
				{
					rx_count++;
					break;
				}
				return true;
			}
		} while (rx_count < 5);

		retries++;
		_delay_ms(1);
	} while(retries--);

	return false;
}

/**************************************************************************************************
* State machine STATE_PING_TEST. Does an echo test to check that the link is working.
*/
bool kbus_state_ping_test(void)
{
	KBUS_PACKET_t cmd;
	cmd.command = KCMD_LOOPBACK;
	cmd.length = 63;
	for (uint8_t i = 0; i < KBUS_PACKET_DATA_SIZE; i++)
		cmd.data[i] = i;

	if (kbus_command(&cmd) &&
		memcmp((void *)rx_packet_DMA->data, &cmd.data, KBUS_PACKET_DATA_SIZE) != 0)
		return true;
	return false;
}

/**************************************************************************************************
* Read configs
*/
bool kbus_read_configs(void)
{
#ifdef BUILD_SIMPLE
	const uint8_t config_id = ATARI_CONFIG_ID;
#endif
#ifdef BUILD_PC_ENGINE
	const uint8_t config_id = PCE_CONFIG_ID;
#endif
#ifdef BUILD_SEGA
	const uint8_t config_id = SEGA3_CONFIG_ID;
#endif

	KBUS_PACKET_t cmd;
	cmd.command = KCMD_READ_CONFIG;
	cmd.data[0] = config_id;

	if (!kbus_command(&cmd))
		return false;

	// check result
	MAPPING_CONFIG_t *new_map = (MAPPING_CONFIG_t *)&packet.data;
	if (new_map->id != config_id)
		return false;
	if (new_map->length > KBUS_PACKET_DATA_SIZE)	// can't handle this yet
		return false;

	// looks okay, load mapping
	if (new_map->length == 0)	// no config of request type available
		return true;

	fmemcpy(&mapping_storage, &packet, sizeof(mapping_storage));
	map = (const MAPPING_CONFIG_t *)&mapping_storage.data;
	return true;
}

/**************************************************************************************************
* Main loop, continually reads updates from K-BUS
*/
void KBUS_run(void)
{
	AC_init();
	usart_reset_rx();

	for(;;)
	{
		// find device to talk to
		for(;;)
		{
			kbus_find_device();				// wait for device to be connected
			if (kbus_state_ping_test())		// test link is working
				break;
		}

		// read configs
		if (!kbus_read_configs())
			break;

		// updates come continuously
		for (;;)
		{
			if (timeout_SIG)
				break;

			if (rx_complete_SIG)
			{
				rx_complete_SIG = 0;
				if (kbus_validate_packet(KCMD_READ_REPORT) &&
					packet.length == 16)	// fixed report size
				{
					RPT_decode_kbus_matrix((uint8_t *)&packet.data);
					RPT_refresh_input_matrix();
					AC_update();
				}

				usart_reset_rx();
			}
		}

		// comms with device failed
		_delay_ms(1);
		CFG_load_default_mapping();
	}
}
