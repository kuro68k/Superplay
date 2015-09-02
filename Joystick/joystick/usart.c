/*
 * usart.c
 *
 * Created: 02/09/2015 09:27:28
 *  Author: Paul Qureshi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdbool.h>
#include "kbus.h"
#include "usart.h"


enum
{
	STATE_WAIT_FOR_START,
	STATE_READ_LENGTH,
	STATE_READ_DATA
};

volatile uint8_t main_rx_buffer_AT[sizeof(KBUS_PACKET_t) + 2];
volatile uint8_t aux_rx_buffer_AT[sizeof(KBUS_PACKET_t) + 2];
volatile uint8_t main_rx_SIG = 0;
volatile uint8_t aux_rx_SIG = 0;

uint8_t main_response_DMA[sizeof(KBUS_PACKET_t)+2];
uint8_t aux_response_DMA[sizeof(KBUS_PACKET_t)+2];


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
	}
	
	AUX_USART.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
	AUX_USART.CTRLA |= USART_RXCINTLVL_HI_gc;

	// DMA
	DMA_CH_t *ch = &MAIN_DMA_CH;
	for (uint8_t i = 0; i < 2; i++)
	{
		ch->CTRLA = DMA_CH_RESET_bm;
		asm("nop");
		ch->CTRLA = DMA_CH_BURSTLEN_1BYTE_gc;
		ch->ADDRCTRL = DMA_CH_SRCRELOAD_TRANSACTION_gc | DMA_CH_SRCDIR_INC_gc |
					  DMA_CH_DESTRELOAD_BURST_gc | DMA_CH_DESTDIR_FIXED_gc;
		ch->TRFCNT = 4;	// minimum packet size
		ch->REPCNT = 0;
		
		ch = &AUX_DMA_CH;
	}

	MAIN_DMA_CH.TRIGSRC = MAIN_DMA_TRIGSRC;
	MAIN_DMA_CH.DESTADDR0 = (uint16_t)&MAIN_USART.DATA & 0xFF;
	MAIN_DMA_CH.DESTADDR1 = ((uint16_t)&MAIN_USART.DATA >> 8) & 0xFF;
	MAIN_DMA_CH.DESTADDR2 = 0;
	MAIN_DMA_CH.SRCADDR0 = (uint16_t)main_response_DMA & 0xFF;
	MAIN_DMA_CH.SRCADDR1 = ((uint16_t)main_response_DMA >> 8) & 0xFF;
	MAIN_DMA_CH.SRCADDR2 = 0;

	AUX_DMA_CH.TRIGSRC = AUX_DMA_TRIGSRC;
	AUX_DMA_CH.DESTADDR0 = (uint16_t)&AUX_USART.DATA & 0xFF;
	AUX_DMA_CH.DESTADDR1 = ((uint16_t)&AUX_USART.DATA >> 8) & 0xFF;
	AUX_DMA_CH.DESTADDR2 = 0;
	AUX_DMA_CH.SRCADDR0 = (uint16_t)aux_response_DMA & 0xFF;
	AUX_DMA_CH.SRCADDR1 = ((uint16_t)aux_response_DMA >> 8) & 0xFF;
	AUX_DMA_CH.SRCADDR2 = 0;
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
		uint8_t buffer[sizeof(main_rx_buffer_AT)];
		MAIN_USART.CTRLA &= ~USART_RXCINTLVL_gm;	// disable RX interrupt
		memcpy(buffer, (void *)main_rx_buffer_AT, sizeof(buffer));
		MAIN_USART.CTRLA |= USART_RXCINTLVL_HI_gc;	// restart RX interrupt
		if (usart_check_crc(buffer) && !(MAIN_DMA_CH.CTRLB & DMA_CH_CHBUSY_bm))	// don't send if previous TX still in progress)
		{
			KBUS_process_command((KBUS_PACKET_t *)buffer, (KBUS_PACKET_t *)main_response_DMA);
			usart_add_crc(main_response_DMA);
			MAIN_DMA_CH.TRFCNT = main_response_DMA[1] + 2;	// data length + command + length
			MAIN_DMA_CH.CTRLA |= DMA_CH_ENABLE_bm;
		}
	}

	// aux
	if (aux_rx_SIG != 0)
	{
		aux_rx_SIG = 0;
	}
}

/**************************************************************************************************
** RX interrupt handlers
*/
ISR(MAIN_RXC_vect)
{
	static uint8_t state = STATE_WAIT_FOR_START;
	static uint8_t ptr = 0;
	static uint8_t len = 0;
	
	uint8_t data = MAIN_USART.DATA;		// also clears RXCIF bit
	
	switch(state)
	{
		case STATE_WAIT_FOR_START:
			if (data == KBUS_FRAME_START)
			{
				state = STATE_READ_LENGTH;
				ptr = 0;
			}
			break;
		
		case STATE_READ_LENGTH:
			main_rx_buffer_AT[ptr++] = data;
			if (ptr > 3)
			{
				len = *(uint16_t *)&main_rx_buffer_AT[2];
				if (len > KBUS_PACKET_DATA_SIZE)
					state = STATE_WAIT_FOR_START;
				else
				{
					state = STATE_READ_DATA;
					len += 2;	// also read the CRC
				}
			}
			break;
		
		case STATE_READ_DATA:
			main_rx_buffer_AT[ptr++] = data;
			len--;
			if (len == 0)
			{
				main_rx_SIG = 0xFF;
				state = STATE_WAIT_FOR_START;
			}
			break;
	}
}
