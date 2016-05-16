/*
 * kbus.c
 *
 * Created: 28/05/2015 11:37:30
 *  Author: MoJo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "hw_misc.h"
#include "report.h"
#include "psx.h"
#include "kbus.h"


volatile KBUS_PACKET_t	input_buffer;
//volatile uint8_t		input_write_ptr_AT = 0;
volatile uint8_t		packet_ready_SIG = 0;
volatile uint8_t		timeout_SIG = 0;

KBUS_PACKET_t			packet;


/**************************************************************************************************
* Reset DMA to start of input buffer
*/
void kbus_reset_dma(void)
{
	KBUS_DMA_CH.CTRLA &= ~EDMA_CH_ENABLE_bm;
	while(KBUS_DMA_CH.CTRLA & EDMA_CH_ENABLE_bm);
	
	KBUS_DMA_CH.TRFCNTL = sizeof(KBUS_PACKET_t);
	KBUS_DMA_CH.ADDRL = (( (uint16_t)&input_buffer) >> 0) & 0xFF;
	KBUS_DMA_CH.ADDRH = (( (uint16_t)&input_buffer) >> 8) & 0xFF;
}

/**************************************************************************************************
* Set up K-BUS interface
*/
void KBUS_init(void)
{
	// USART
	KBUS_PORT.DIRCLR = KBUS_RX_PIN_bm;
	KBUS_PORT.DIRSET = KBUS_TX_PIN_bm;
	KBUS_PORT.KBUS_RX_PINCTRL = (KBUS_PORT.KBUS_RX_PINCTRL & ~PORT_ISC_gm) | PORT_ISC_FALLING_gc;	// event triggered on falling edge
	
	KBUS_USART.CTRLA = 0;
	KBUS_USART.BAUDCTRLB = (uint8_t)((uint8_t)BSCALE << 4) | ((uint16_t)BSEL >> 8);
	KBUS_USART.BAUDCTRLA = (uint8_t)BSEL;
	KBUS_USART.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
	KBUS_USART.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm;

	// DMA from USART
	KBUS_DMA_CH.CTRLA = EDMA_CH_RESET_bm;
	NOP();
	KBUS_DMA_CH.CTRLA = 0;
	KBUS_DMA_CH.CTRLB = 0;
	KBUS_DMA_CH.ADDRCTRL = EDMA_CH_RELOAD_BLOCK_gc | EDMA_CH_DIR_INC_gc;
	KBUS_DMA_CH.TRIGSRC = KBUS_DMA_TRIGGER_SRC;
	kbus_reset_dma();
	
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
	KBUS_TC.PER = 0x9C3F;	// timeout, 10mS @ 16MHz/4
	KBUS_TC.CCA = 0x004F;	// end of packet, 20uS @ 16MHz/4
	KBUS_TC.CTRLA = TC45_CLKSEL_DIV4_gc;
}

/**************************************************************************************************
* Handle timer overflows (controller not sending reports)
*/
ISR(KBUS_TC_OVF_vect)
{
	timeout_SIG = 0xFF;
}

/**************************************************************************************************
* Handle timer CCA matches (end of packet)
*/
ISR(KBUS_TC_CCA_vect)
{
	packet_ready_SIG = 0xFF;
}

/**************************************************************************************************
* Send data out over K-BUS
*/
void kbus_send(const void *buffer, uint8_t length)
{
	while(length--)
	{
		while ((KBUS_USART.STATUS & USART_DREIF_bm) == 0);
		KBUS_USART.DATA = *(uint8_t *)buffer++;
	}
}

/**************************************************************************************************
* Main loop, continually reads updates from K-BUS
*/
void KBUS_run(void)
{
	for(;;)
	{
		WDR();
		
		// no response from controller before timeout
		if (timeout_SIG)
		{
			timeout_SIG = 0;

			// ask for updates
			KBUS_PACKET_t cmd;
			cmd.command = CMD_START_REPORTING;
			cmd.length = 0;
			*(uint16_t *)&cmd.data[0] = HW_crc16(&cmd, 2);
			kbus_send(&cmd, 4);
			
			kbus_reset_dma();
			KBUS_DMA_CH.CTRLA |= EDMA_CH_ENABLE_bm;
		}
		
		// packet arrived
		if (packet_ready_SIG)
		{
			packet_ready_SIG = 0;
			
			// make sure DMA isn't still writing to the buffer
			KBUS_DMA_CH.CTRLA &= ~EDMA_CH_ENABLE_bm;
			while(KBUS_DMA_CH.CTRLA & EDMA_CH_ENABLE_bm);
			
			// copy buffer and reset DMA for next packet
			memcpy(&packet, (void *)&input_buffer, sizeof(packet));
			kbus_reset_dma();
			KBUS_DMA_CH.CTRLA |= EDMA_CH_ENABLE_bm;
			
			// validate packet
			if (packet.length > 63)
				continue;
			if (*(uint16_t *)&packet.data[packet.length] != HW_crc16(&packet, 2 + packet.length))
				continue;

			if (input_buffer.command == (CMD_READ_REPORT | RESPONSE_BIT_bm))
			{
				if (packet.length != sizeof(REPORT_t))
					continue;
				
				// handle packet
				PSX_update((REPORT_t *)&packet.data);
			}
		}
	}
}
