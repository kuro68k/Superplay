/*
 * saturn.c
 *
 * Created: 01/05/2015 15:06:13
 *  Author: MoJo
 */ 

#include <avr/io.h>
#include "report.h"
#include "saturn.h"

/**************************************************************************************************
* Set up Saturn interface
*/
void SAT_init(void)
{
	SAT_PORT.DIRSET = SAT_D1_PIN_bm | SAT_D0_PIN_bm | SAT_D3_PIN_bm | SAT_D2_PIN_bm;
	SAT_PORT.DIRCLR = SAT_TH_PIN_bm | SAT_TR_PIN_bm | SAT_TL_PIN_bm;
	SAT_PORT.INT0MASK = SAT_TH_PIN_bm | SAT_TR_PIN_bm;
	SAT_PORT.INTCTRL = (SAT_PORT.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_HI_gc;
	PORTCFG.VPCTRLA = PORTCFG_VP02MAP_PORTA_gc;		// virtual port for fast access in interrupt
}

/**************************************************************************************************
* Load GPIOs with Saturn bits
*/
void SAT_update(const REPORT_t * const report)
{
	uint8_t	temp0, temp1, temp2, temp3;
	
	// GPIO		D1	D0	D3	D2
	// 0		Y	Z	R	X
	// 1		Dn	Up	Rt	Lf
	// 2		C	B	St	A
	// 3		1	1	L	1
	
	// GPIO0
	temp0 = 0;
	if (report->buttons1_8 & (1<<4))			temp0 |= SAT_D1_PIN_bm;
	if (report->buttons1_8 & (1<<5))			temp0 |= SAT_D0_PIN_bm;
	if (report->buttons1_8 & (1<<7))			temp0 |= SAT_D3_PIN_bm;
	if (report->buttons1_8 & (1<<3))			temp0 |= SAT_D2_PIN_bm;

	// GPIO1
	temp1 = 0;
	if (report->udlr_sscc & STICK_DOWN_bm)		temp1 |= SAT_D1_PIN_bm;
	if (report->udlr_sscc & STICK_UP_bm)		temp1 |= SAT_D0_PIN_bm;
	if (report->udlr_sscc & STICK_RIGHT_bm)		temp1 |= SAT_D3_PIN_bm;
	if (report->udlr_sscc & STICK_LEFT_bm)		temp1 |= SAT_D2_PIN_bm;

	// GPIO2
	temp2 = 0;
	if (report->buttons1_8 & (1<<2))			temp2 |= SAT_D1_PIN_bm;
	if (report->buttons1_8 & (1<<1))			temp2 |= SAT_D0_PIN_bm;
	if (report->udlr_sscc & BUTTON_START_bm)	temp2 |= SAT_D3_PIN_bm;
	if (report->buttons1_8 & (1<<0))			temp2 |= SAT_D2_PIN_bm;

	// GPIO3
	temp3 = SAT_D1_PIN_bm | SAT_D0_PIN_bm | SAT_D2_PIN_bm;
	if (report->buttons1_8 & (1<<6))			temp3 |= SAT_D3_PIN_bm;

	GPIO0 = temp0;
	GPIO0 = temp1;
	GPIO0 = temp2;
	GPIO3 =	temp3;
}
