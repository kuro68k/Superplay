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

/**************************************************************************************************
** Refresh report with current values
*/
void RPT_refresh(void)
{
	memset(&report, 0, sizeof(report));
	
	// joystick
	if (!(PORTA.IN & JOY_UP_PIN_bm))	report.udlr_sscc |= JOY_UP_bm;
	if (!(PORTA.IN & JOY_DOWN_PIN_bm))	report.udlr_sscc |= JOY_DOWN_bm;
	if (!(PORTA.IN & JOY_LEFT_bm))		report.udlr_sscc |= JOY_LEFT_bm;
	if (!(PORTA.IN & JOY_RIGHT_bm))		report.udlr_sscc |= JOY_RIGHT_bm;

	// meta buttons
	if (!(PORTA.IN & START_PIN_bm))		report.udlr_sscc |= BUTTON_START_bm;
	if (!(PORTA.IN & COIN_PIN_bm))		report.udlr_sscc |= BUTTON_COIN_bm;
	if (!(PORTF.IN & CONTROL_PIN_bm))	report.udlr_sscc |= BUTTON_CONTROL_bm;


	// fire buttons
	if (!(PORTB.IN & BUTTON_1_PIN_bm))	report.buttons |= (1<<0);
	if (!(PORTB.IN & BUTTON_2_PIN_bm))	report.buttons |= (1<<1);
	if (!(PORTB.IN & BUTTON_3_PIN_bm))	report.buttons |= (1<<2);
	if (!(PORTB.IN & BUTTON_4_PIN_bm))	report.buttons |= (1<<3);
	if (!(PORTB.IN & BUTTON_5_PIN_bm))	report.buttons |= (1<<4);
	if (!(PORTB.IN & BUTTON_6_PIN_bm))	report.buttons |= (1<<5);
	if (!(PORTB.IN & BUTTON_7_PIN_bm))	report.buttons |= (1<<6);
	if (!(PORTB.IN & BUTTON_8_PIN_bm))	report.buttons |= (1<<7);

	// button mode
	switch (cfg->button_mode)
	{
		case CFG_BUTTON_MODE_SWITCHED:
			if (!(PORTF.IN & MODE_4_bm))
				report.rot_mode |= BUTTON_MODE_4_gc;
			else if (!(PORTF.IN & MODE_4AF_bm))
				report.rot_mode |= BUTTON_MODE_4AF_gc;
			else
				report.rot_mode |= BUTTON_MODE_8_gc;
			break;
		
		default:
		case CFG_BUTTON_MODE_8:
			report.rot_mode |= BUTTON_MODE_8_gc;
			break;

		case CFG_BUTTON_MODE_4:
			report.rot_mode |= BUTTON_MODE_4_gc;
			break;

		case CFG_BUTTON_MODE_4AF:
			report.rot_mode |= BUTTON_MODE_4AF_gc;
			break;

		case CFG_BUTTON_MODE_16:
			report.rot_mode |= BUTTON_MODE_16_gc;
			break;
	}

	// autofire
	uint16_t af_mask = AF_read(report.buttons, report.rot_mode & BUTTON_MODE_gm);
	report.buttons &= af_mask;
	
	// remap depending on the mode
	if (((report.rot_mode & BUTTON_MODE_gm) == BUTTON_MODE_4_gc) ||
		((report.rot_mode & BUTTON_MODE_gm) == BUTTON_MODE_4AF_gc))
	{
		report.buttons |= (report.buttons & 0xF) << 4;
		report.buttons &= 0xF;
	}


	// rotary controller
	uint8_t rot = ROTARY_NONE_gc;
	uint8_t p = PORTE.IN;
	if (!(p & ROTARY_1_PIN_bm))		rot = 0;
	if (!(p & ROTARY_2_PIN_bm))		rot = 1;
	if (!(p & ROTARY_3_PIN_bm))		rot = 2;
	if (!(p & ROTARY_4_PIN_bm))		rot = 3;
	if (!(p & ROTARY_5_PIN_bm))		rot = 4;
	if (!(p & ROTARY_6_PIN_bm))		rot = 5;
	if (!(p & ROTARY_7_PIN_bm))		rot = 6;
	if (!(p & ROTARY_8_PIN_bm))		rot = 7;
	p = PORTF.IN;
	if (!(p & ROTARY_9_PIN_bm))		rot = 8;
	if (!(p & ROTARY_10_PIN_bm))	rot = 9;
	if (!(p & ROTARY_11_PIN_bm))	rot = 10;
	if (!(p & ROTARY_12_PIN_bm))	rot = 11;
	report.rot_mode |= rot << ROTARY_gp;
}