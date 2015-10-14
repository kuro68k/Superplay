/*
 * report.c
 *
 * Created: 05/04/2015 23:19:56
 *  Author: Paul Qureshi
 */ 

#include <avr/io.h>
#include <string.h>

#include "hw_misc.h"
#include "config.h"
#include "autofire.h"
#include "report.h"

REPORT_t	report;
uint8_t		logical_inputs[NUM_LOGICAL_INPUTS];


/**************************************************************************************************
** Refresh logical inputs
*/
void RPT_logical_inputs_refresh(void)
{
	memset(logical_inputs, 0, sizeof(logical_inputs));

	// PORTA
	uint8_t p = PORTA.IN;
	if (!(p & JOY_UP_PIN_bm))		logical_inputs[LJOY_UP] = 1;
	if (!(p & JOY_DOWN_PIN_bm))		logical_inputs[LJOY_DN] = 1;
	if (!(p & JOY_LEFT_bm))			logical_inputs[LJOY_LF] = 1;
	if (!(p & JOY_RIGHT_bm))		logical_inputs[LJOY_RT] = 1;
	if (!(p & START_PIN_bm))		logical_inputs[LSTART] = 1;
	if (!(p & COIN_PIN_bm))			logical_inputs[LCOIN] = 1;
	if (!(p & AUTO_LOW_5_PIN_bm))	logical_inputs[LAF_LOW_5] = 1;
	if (!(p & AUTO_LOW_6_PIN_bm))	logical_inputs[LAF_LOW_6] = 1;

	// PORTB
	p = PORTB.IN;
	if (!(p & BUTTON_1_PIN_bm))		logical_inputs[LBUTTON1] = 1;
	if (!(p & BUTTON_2_PIN_bm))		logical_inputs[LBUTTON2] = 1;
	if (!(p & BUTTON_3_PIN_bm))		logical_inputs[LBUTTON3] = 1;
	if (!(p & BUTTON_4_PIN_bm))		logical_inputs[LBUTTON4] = 1;
	if (!(p & BUTTON_5_PIN_bm))		logical_inputs[LBUTTON5] = 1;
	if (!(p & BUTTON_6_PIN_bm))		logical_inputs[LBUTTON6] = 1;
	if (!(p & BUTTON_7_PIN_bm))		logical_inputs[LBUTTON7] = 1;
	if (!(p & BUTTON_8_PIN_bm))		logical_inputs[LBUTTON8] = 1;

	// PORTC
	p = PORTC.IN;
	if (!(p & AUTO_LOW_1_PIN_bm))	logical_inputs[LAF_LOW_1] = 1;
	if (!(p & AUTO_LOW_2_PIN_bm))	logical_inputs[LAF_LOW_2] = 1;
	if (!(p & AUTO_LOW_3_PIN_bm))	logical_inputs[LAF_LOW_3] = 1;
	if (!(p & AUTO_LOW_4_PIN_bm))	logical_inputs[LAF_LOW_4] = 1;

	// PORTD
	p = PORTD.IN;
	if (!(p & AUTO_HIGH_1_PIN_bm))	logical_inputs[LAF_HIGH_1] = 1;
	if (!(p & AUTO_HIGH_2_PIN_bm))	logical_inputs[LAF_HIGH_2] = 1;
	if (!(p & AUTO_HIGH_3_PIN_bm))	logical_inputs[LAF_HIGH_3] = 1;
	if (!(p & AUTO_HIGH_4_PIN_bm))	logical_inputs[LAF_HIGH_4] = 1;
	if (!(p & AUTO_HIGH_5_PIN_bm))	logical_inputs[LAF_HIGH_5] = 1;
	if (!(p & AUTO_HIGH_6_PIN_bm))	logical_inputs[LAF_HIGH_6] = 1;

	// PORTE
	p = PORTE.IN;
	if (!(p & ROTARY_1_PIN_bm))		logical_inputs[LROTARY1] = 1;
	if (!(p & ROTARY_2_PIN_bm))		logical_inputs[LROTARY2] = 1;
	if (!(p & ROTARY_3_PIN_bm))		logical_inputs[LROTARY3] = 1;
	if (!(p & ROTARY_4_PIN_bm))		logical_inputs[LROTARY4] = 1;
	if (!(p & ROTARY_5_PIN_bm))		logical_inputs[LROTARY5] = 1;
	if (!(p & ROTARY_6_PIN_bm))		logical_inputs[LROTARY6] = 1;
	if (!(p & ROTARY_7_PIN_bm))		logical_inputs[LROTARY7] = 1;
	if (!(p & ROTARY_8_PIN_bm))		logical_inputs[LROTARY8] = 1;

	// PORTF
	p = PORTF.IN;
	if (!(p & ROTARY_9_PIN_bm))		logical_inputs[LROTARY9] = 1;
	if (!(p & ROTARY_10_PIN_bm))	logical_inputs[LROTARY10] = 1;
	if (!(p & ROTARY_11_PIN_bm))	logical_inputs[LROTARY11] = 1;
	if (!(p & ROTARY_12_PIN_bm))	logical_inputs[LROTARY12] = 1;
	//if (!(p & UNUSED_PIN_bm))		logical_inputs[LCONTROL] = 1;
	if (!(p & MODE_4_PIN_bm))		logical_inputs[LCONTROL] = 1;
	if (!(p & MODE_4AF_PIN_bm))		logical_inputs[LCONTROL] = 1;
	if (!(p & CONTROL_PIN_bm))		logical_inputs[LCONTROL] = 1;
}

/**************************************************************************************************
** Refresh report with current values
*/
void RPT_refresh(void)
{
	memset(&report, 0, sizeof(report));
	
	// joystick
	if (logical_inputs[LJOY_UP])		report.udlr_sscc |= JOY_UP_bm;
	if (logical_inputs[LJOY_DN])		report.udlr_sscc |= JOY_DOWN_bm;
	if (logical_inputs[LJOY_LF])		report.udlr_sscc |= JOY_LEFT_bm;
	if (logical_inputs[LJOY_RT])		report.udlr_sscc |= JOY_RIGHT_bm;

	// meta buttons
	if (logical_inputs[LSTART])			report.udlr_sscc |= BUTTON_START_bm;
	if (logical_inputs[LCOIN])			report.udlr_sscc |= BUTTON_COIN_bm;
	if (logical_inputs[LCONTROL])		report.udlr_sscc |= BUTTON_CONTROL_bm;

	// fire buttons
	if (logical_inputs[LBUTTON1])		report.buttons |= (1<<0);
	if (logical_inputs[LBUTTON2])		report.buttons |= (1<<1);
	if (logical_inputs[LBUTTON3])		report.buttons |= (1<<2);
	if (logical_inputs[LBUTTON4])		report.buttons |= (1<<3);
	if (logical_inputs[LBUTTON5])		report.buttons |= (1<<4);
	if (logical_inputs[LBUTTON6])		report.buttons |= (1<<5);
	if (logical_inputs[LBUTTON7])		report.buttons |= (1<<6);
	if (logical_inputs[LBUTTON8])		report.buttons |= (1<<7);
	if (logical_inputs[LBUTTON9])		report.buttons |= (1<<8);
	if (logical_inputs[LBUTTON10])		report.buttons |= (1<<9);
	if (logical_inputs[LBUTTON11])		report.buttons |= (1<<10);
	if (logical_inputs[LBUTTON12])		report.buttons |= (1<<11);
	if (logical_inputs[LBUTTON13])		report.buttons |= (1<<12);
	if (logical_inputs[LBUTTON14])		report.buttons |= (1<<13);
	if (logical_inputs[LBUTTON15])		report.buttons |= (1<<14);
	if (logical_inputs[LBUTTON16])		report.buttons |= (1<<15);

	// button mode
	if (logical_inputs[LMODE_4])
		report.rot_mode |= BUTTON_MODE_4_gc;
	else if (logical_inputs[LMODE_4AF])
		report.rot_mode |= BUTTON_MODE_4AF_gc;
	else
		report.rot_mode |= BUTTON_MODE_8_gc;

	// autofire
	uint16_t af_mask = AF_read(report.buttons);
	report.buttons &= af_mask;
	
	// remap depending on the mode
	if (logical_inputs[LMODE_4] || logical_inputs[LMODE_4AF])
	{
		report.buttons |= (report.buttons & 0xF) << 4;
		report.buttons &= 0xFF0F;
	}

	// rotary controller
	uint8_t rot = ROTARY_NONE_gc;
	if (logical_inputs[LROTARY1])	rot = 0;
	if (logical_inputs[LROTARY2])	rot = 1;
	if (logical_inputs[LROTARY3])	rot = 2;
	if (logical_inputs[LROTARY4])	rot = 3;
	if (logical_inputs[LROTARY5])	rot = 4;
	if (logical_inputs[LROTARY6])	rot = 5;
	if (logical_inputs[LROTARY7])	rot = 6;
	if (logical_inputs[LROTARY8])	rot = 7;
	if (logical_inputs[LROTARY9])	rot = 8;
	if (logical_inputs[LROTARY10])	rot = 9;
	if (logical_inputs[LROTARY11])	rot = 10;
	if (logical_inputs[LROTARY12])	rot = 11;
	report.rot_mode |= rot << ROTARY_gp;
}