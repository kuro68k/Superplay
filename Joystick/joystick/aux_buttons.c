/*
 * aux_buttons.c
 *
 * Created: 15/04/2016 10:58:36
 *  Author: paul.qureshi
 *
 * Serial output to multiple NPIC6C596As to simulate button presses.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "hw_misc.h"
#include "report.h"
#include "aux_buttons.h"


/**************************************************************************************************
** Send report
*/
void AB_send_report(void)
{
	AB_USART.DATA = report.buttons & 0xFF;
	while(!(AB_USART.STATUS & USART_DREIF_bm));		// FIFO has a free slot
	AB_USART.DATA = report.udlr_sscc;
	while(!(AB_USART.STATUS & USART_TXCIF_bm));		// last bit shifted out
	AB_PORT.OUTSET = AB_LAT_PIN_bm;
	NOP();
	AB_PORT.OUTCLR = AB_LAT_PIN_bm | AB_OE_PIN_bm;	// also enable outputs if this is the first report
}

/**************************************************************************************************
** Prepare AUX port for shift register output
*/
void AB_init(void)
{
	if (cfg->aux_mode != AUX_MODE_BUTTONS)
		return;
	
	AB_PORT.OUTSET = AB_OE_PIN_bm | AB_DAT_PIN_bm | AB_CLK_PIN_bm;
	AB_PORT.DIRSET = AB_OE_PIN_bm | AB_DAT_PIN_bm | AB_LAT_PIN_bm | AB_CLK_PIN_bm;
	
	AB_USART.CTRLA = 0;
	AB_USART.CTRLB = USART_TXEN_bm;
	AB_USART.CTRLC = USART_CMODE_MSPI_gc | USART_CHSIZE_8BIT_gc;
	AB_USART.BAUDCTRLA = AB_USART_BSEL & 0xFF;
	AB_USART.BAUDCTRLB = ((AB_USART_BSCALE << 4) & 0xF0) | ((AB_USART_BSEL >> 8) & 0x0F);
}