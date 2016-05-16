/*
 * psx.c
 *
 * Created: 11/05/2016 14:32:21
 *  Author: paul.qureshi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "report.h"
#include "psx.h"


enum
{
	STATE_IDLE = 0,
	STATE_COMMAND,
	STATE_POLL1,
	STATE_POLL2,
};

volatile uint8_t	psx_poll_byte4_AT = 0xFF;
volatile uint8_t	psx_poll_byte5_AT = 0xFF;

/**************************************************************************************************
* Update PSX polling bytes with data from report
*/
void PSX_update(REPORT_t *r)
{
	uint8_t byte4 = 0xFF;
	uint8_t byte5 = 0xFF;
	
	if (r->udlr_sscc & STICK_UP_bm)		byte4 &= ~PSX_UP_bm;
	if (r->udlr_sscc & STICK_DOWN_bm)	byte4 &= ~PSX_DOWN_bm;
	if (r->udlr_sscc & STICK_LEFT_bm)	byte4 &= ~PSX_LEFT_bm;
	if (r->udlr_sscc & STICK_RIGHT_bm)	byte4 &= ~PSX_RIGHT_bm;
	if (r->buttons9_16 & (1<<6))		byte4 &= ~PSX_SELECT_bm;
	if (r->buttons9_16 & (1<<7))		byte4 &= ~PSX_START_bm;
	
	if (r->buttons1_8 & (1<<0))			byte5 &= ~PSX_SQUARE_bm;
	if (r->buttons1_8 & (1<<1))			byte5 &= ~PSX_TRIANGLE_bm;
	if (r->buttons1_8 & (1<<4))			byte5 &= ~PSX_CROSS_bm;
	if (r->buttons1_8 & (1<<5))			byte5 &= ~PSX_CIRCLE_bm;
	
	psx_poll_byte4_AT = byte4;
	psx_poll_byte5_AT = byte5;
}

/**************************************************************************************************
* Set up PSX port and state machine
*/
void PSX_init(void)
{
	// acknowledge generator
	PSX_TC.CTRLA = TC4_UPSTOP_bm;
	PSX_TC.CTRLB = TC45_WGMODE_SINGLESLOPE_gc;
	PSX_TC.CTRLC = 0;
	PSX_TC.CTRLD = 0;
	PSX_TC.CTRLE = TC45_CCDMODE_COMP_gc;	// output on ACK pin
	PSX_TC.INTCTRLA = 0;
	PSX_TC.INTCTRLB = 0;
	PSX_TC.CNT = 0;
	PSX_TC.PER = 17;	// 1us @ 16MHz
	PSX_TC.CCD = 1;
}

/**************************************************************************************************
* PSX SPI interrupt handler / state machine
*/
ISR(PSX_SPI_INT_vect)
{
	static uint8_t	state = STATE_IDLE;
	//static bool config_mode = false;
	
	uint8_t c = PSX_SPI.DATA;
	
	switch(state)
	{
		case STATE_IDLE:
			if (c == 0x01)	// start of packet
			{
				PSX_SEND_ACK;
				state = STATE_COMMAND;
				PSX_SPI.DATA = PSX_MODE_DIGITAL | 1;
			}
			else
				PSX_SPI.DATA = 0xFF;	// default new packet response
			break;
		
		case STATE_COMMAND:
			switch (c)
			{
				default:
					PSX_SPI.DATA = 0xFF;
					state = STATE_IDLE;
					break;
				
				case PSX_CMD_POLL:
					PSX_SEND_ACK;
					PSX_SPI.DATA = 0x5A;
					state = STATE_POLL1;
					break;
			}
			break;
		
		case STATE_POLL1:
			PSX_SEND_ACK;
			PSX_SPI.DATA = psx_poll_byte4_AT;
			state = STATE_POLL2;
			break;

		case STATE_POLL2:
			PSX_SEND_ACK;
			PSX_SPI.DATA = psx_poll_byte5_AT;
			state = STATE_POLL2;
			break;
	}
}
