/*
 * report.c
 *
 */

#include <avr/io.h>
#include <string.h>

#include "global.h"
#include "hw_misc.h"
#include "config.h"
#include "report.h"
#include "io_table.h"

REPORT_t	report;
uint8_t		input_matrix[256];


/**************************************************************************************************
** Set up reports
*/
void RPT_init(void)
{
	memset(input_matrix, 0, sizeof(input_matrix));
	input_matrix[LON] = 1;
	input_matrix[PON] = 1;
}

/**************************************************************************************************
** Refresh physical inputs
*/
void rpt_physical_inputs_refresh(void)
{
	// PORTA
	uint8_t p = PORTA.IN;
	if (!(p & JOY_UP_PIN_bm))		input_matrix[PJOY_UP] = 1;
	if (!(p & JOY_DOWN_PIN_bm))		input_matrix[PJOY_DN] = 1;
	if (!(p & JOY_LEFT_bm))			input_matrix[PJOY_LF] = 1;
	if (!(p & JOY_RIGHT_bm))		input_matrix[PJOY_RT] = 1;
	if (!(p & START_PIN_bm))		input_matrix[PB16] = 1;
	if (!(p & COIN_PIN_bm))			input_matrix[PB15] = 1;
	if (!(p & AUTO_LOW_5_PIN_bm))	input_matrix[PA5] = 1;
	if (!(p & AUTO_LOW_6_PIN_bm))	input_matrix[PA6] = 1;

	// PORTB
	p = PORTB.IN;
	if (!(p & BUTTON_1_PIN_bm))		input_matrix[PB1] = 1;
	if (!(p & BUTTON_2_PIN_bm))		input_matrix[PB2] = 1;
	if (!(p & BUTTON_3_PIN_bm))		input_matrix[PB3] = 1;
	if (!(p & BUTTON_4_PIN_bm))		input_matrix[PB4] = 1;
	if (!(p & BUTTON_5_PIN_bm))		input_matrix[PB5] = 1;
	if (!(p & BUTTON_6_PIN_bm))		input_matrix[PB6] = 1;
	if (!(p & BUTTON_7_PIN_bm))		input_matrix[PB7] = 1;
	if (!(p & BUTTON_8_PIN_bm))		input_matrix[PB8] = 1;

	// PORTC
	p = PORTC.IN;
	if (!(p & AUTO_LOW_1_PIN_bm))	input_matrix[PA1] = 1;
	if (!(p & AUTO_LOW_2_PIN_bm))	input_matrix[PA2] = 1;
	if (!(p & AUTO_LOW_3_PIN_bm))	input_matrix[PA3] = 1;
	if (!(p & AUTO_LOW_4_PIN_bm))	input_matrix[PA4] = 1;

	// PORTD
	p = PORTD.IN;
	if (!(p & AUTO_HIGH_1_PIN_bm))	input_matrix[PA9] = 1;
	if (!(p & AUTO_HIGH_2_PIN_bm))	input_matrix[PA10] = 1;
	if (!(p & AUTO_HIGH_3_PIN_bm))	input_matrix[PA11] = 1;
	if (!(p & AUTO_HIGH_4_PIN_bm))	input_matrix[PA12] = 1;
	if (!(p & AUTO_HIGH_5_PIN_bm))	input_matrix[PA13] = 1;
	if (!(p & AUTO_HIGH_6_PIN_bm))	input_matrix[PA14] = 1;

	// PORTE
	p = PORTE.IN;
	if (!(p & ROTARY_1_PIN_bm))		input_matrix[PR1] = 1;
	if (!(p & ROTARY_2_PIN_bm))		input_matrix[PR2] = 1;
	if (!(p & ROTARY_3_PIN_bm))		input_matrix[PR3] = 1;
	if (!(p & ROTARY_4_PIN_bm))		input_matrix[PR4] = 1;
	if (!(p & ROTARY_5_PIN_bm))		input_matrix[PR5] = 1;
	if (!(p & ROTARY_6_PIN_bm))		input_matrix[PR6] = 1;
	if (!(p & ROTARY_7_PIN_bm))		input_matrix[PR7] = 1;
	if (!(p & ROTARY_8_PIN_bm))		input_matrix[PR8] = 1;

	// PORTF
	p = PORTF.IN;
	if (!(p & ROTARY_9_PIN_bm))		input_matrix[PR9] = 1;
	if (!(p & ROTARY_10_PIN_bm))	input_matrix[PR10] = 1;
	if (!(p & ROTARY_11_PIN_bm))	input_matrix[PR11] = 1;
	if (!(p & ROTARY_12_PIN_bm))	input_matrix[PR12] = 1;
	if (!(p & UNUSED_PIN_bm))		input_matrix[PB13] = 1;
	if (!(p & MODE_4_PIN_bm))		input_matrix[PB12] = 1;
	if (!(p & MODE_4AF_PIN_bm))		input_matrix[PB11] = 1;
	if (!(p & CONTROL_PIN_bm))		input_matrix[PMETA] = 1;

	input_matrix[LOFF] = 0;
	input_matrix[LON] = 1;
}

/**************************************************************************************************
** Map physical to logical inputs and apply forced inputs
*/
void RPT_refresh_input_matrix(void)
{
	rpt_physical_inputs_refresh();

	input_matrix[LOFF] = 0;
	input_matrix[LON] = 1;

	for (uint8_t i = 0; i < map->count; i++)
		input_matrix[map->mapping[i][0]] = input_matrix[map->mapping[i][1]];
}

/**************************************************************************************************
** Update LED control outputs
*/
void RPT_refresh_leds(void)
{
	// set output pins
	for (uint8_t i = 0; i < 16; i++)
	{
		uint8_t pio = settings->leds[i];	// physical I/O
		if ((pio != 0) &&
			io_pin_table[pio].port != 0)
		{
			if (input_matrix[LLED1 + i])
				io_pin_table[pio].port->OUTSET = io_pin_table[pio].pin_mask;
			else
				io_pin_table[pio].port->OUTCLR = io_pin_table[pio].pin_mask;
		}
	}
}

/**************************************************************************************************
** Refresh report with current values
*/
void RPT_refresh(void)
{
	memset(&report, 0, sizeof(report));

	// joystick
	uint8_t b = 0;
	if (input_matrix[LJOY_UP])		b |= JOY_UP_bm;
	if (input_matrix[LJOY_DN])		b |= JOY_DOWN_bm;
	if (input_matrix[LJOY_LF])		b |= JOY_LEFT_bm;
	if (input_matrix[LJOY_RT])		b |= JOY_RIGHT_bm;

	// meta buttons
	//if (input_matrix[LSTART])			b |= BUTTON_START_bm;
	//if (input_matrix[LCOIN])			b |= BUTTON_COIN_bm;
	//if (input_matrix[LCONTROL])		b |= BUTTON_CONTROL_bm;
	report.udlr_sscc = b;

	// fire buttons
	uint16_t w = 0;
	if (input_matrix[LBUTTON1])		w |= (1<<0);
	if (input_matrix[LBUTTON2])		w |= (1<<1);
	if (input_matrix[LBUTTON3])		w |= (1<<2);
	if (input_matrix[LBUTTON4])		w |= (1<<3);
	if (input_matrix[LBUTTON5])		w |= (1<<4);
	if (input_matrix[LBUTTON6])		w |= (1<<5);
	if (input_matrix[LBUTTON7])		w |= (1<<6);
	if (input_matrix[LBUTTON8])		w |= (1<<7);
	if (input_matrix[LBUTTON9])		w |= (1<<8);
	if (input_matrix[LBUTTON10])		w |= (1<<9);
	if (input_matrix[LBUTTON11])		w |= (1<<10);
	if (input_matrix[LBUTTON12])		w |= (1<<11);
	if (input_matrix[LBUTTON13])		w |= (1<<12);
	if (input_matrix[LBUTTON14])		w |= (1<<13);
	if (input_matrix[LBUTTON15])		w |= (1<<14);
	if (input_matrix[LBUTTON16])		w |= (1<<15);
	report.buttons = w;

/*
	// button mode
	if (logical_inputs[LMODE_4])
		report.rot_mode |= BUTTON_MODE_4_gc;
	else if (logical_inputs[LMODE_4AF])
		report.rot_mode |= BUTTON_MODE_4AF_gc;
	else
		report.rot_mode |= BUTTON_MODE_8_gc;

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
*/
}