/*
 * atari.c
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "config.h"
#include "report.h"
#include "atari.h"

/**************************************************************************************************
* Set up outputs and interrupts
*/
void AC_init(void)
{
	MODE_PORT.INTCTRL = PORT_INTLVL_HI_gc;	// mode pin interrupt
}

/**************************************************************************************************
* Update outputs
*/
void AC_update(void)
{
	uint8_t m0 = 0;
	uint8_t m1 = 0;

	// common to all
	if (input_matrix[LJOY_UP])
		m0 |= AC_JOY_UP_PIN_bm;
	if (input_matrix[LJOY_DN])
		m0 |= AC_JOY_DOWN_PIN_bm;
	if (input_matrix[LJOY_LF])
		m0 |= AC_JOY_LEFT_PIN_bm;
	if (input_matrix[LJOY_RT])
		m0 |= AC_JOY_RIGHT_PIN_bm;

	if (1)	// Atari / Commodore
	{
		if (input_matrix[LBUTTON1])
			m0 |= AC_JOY_B1_PIN_bm;
		if (input_matrix[LBUTTON2])
			m0 |= AC_JOY_B2_PIN_bm;
		m1 = m0;
	}

	if (0)	// PC Engine
	{
		if (input_matrix[LBUTTON1])		// I
			m1 |= AC_JOY_UP_PIN_bm;
		if (input_matrix[LBUTTON2])		// II
			m1 |= AC_JOY_RIGHT_PIN_bm;
		if (input_matrix[LBUTTON15])	// Select
			m1 |= AC_JOY_DOWN_PIN_bm;
		if (input_matrix[LBUTTON16])	// Run
			m1 |= AC_JOY_LEFT_PIN_bm;
	}

	if (0)	// Megadrive
	{
		m1 = m0;	// UDLR
		if (input_matrix[LBUTTON1]	)	// A
			m0 |= AC_JOY_B1_PIN_bm;
		if (input_matrix[LBUTTON16])	// Start
			m0 |= AC_JOY_B2_PIN_bm;

		if (input_matrix[LBUTTON2])		// B
			m1 |= AC_JOY_B1_PIN_bm;
		if (input_matrix[LBUTTON3])		// C
			m1 |= AC_JOY_B2_PIN_bm;
	}

	cli();
	GPIO0 = m0;
	GPIO1 = m1;
	sei();
	AT_set_output();
}
