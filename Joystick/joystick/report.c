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
	if (!(p & JOY_LEFT_PIN_bm))		input_matrix[PJOY_LF] = 1;
	if (!(p & JOY_RIGHT_PIN_bm))	input_matrix[PJOY_RT] = 1;
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
** Map physical to logical inputs
*/
void RPT_refresh_input_matrix(void)
{
	rpt_physical_inputs_refresh();

	memset(input_matrix, 0, 128);	// clear logical inputs
	input_matrix[LON] = 1;

	for (uint8_t i = 0; i < map->count; i++)
		input_matrix[map->mapping[i][0]] |= input_matrix[map->mapping[i][1]];
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
** Generate a 128 bit (16 byte) report. Buffer must be at least 16 bytes.
*/
void RPT_generate_report(uint8_t *buffer)
{
	asm volatile(
		"ldi	r18, 16"			"\n\t"
		"loop%=:"					"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 0
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 0"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 1
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 1"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 2
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 2"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 3
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 3"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 4
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 4"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 5
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 5"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 6
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 6"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 7
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 7"				"\n\t"
		"st		Z+, r19"			"\n\t"
		"dec	r18"				"\n\t"
		"brne	loop%="
	:
	: [input] "x" (input_matrix), [output] "z" (buffer)
	: "r18", "r19"
	);
}