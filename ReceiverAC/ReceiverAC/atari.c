/*
 * atari.c
 *
 * Created: 04/05/2016 15:39:10
 *  Author: kuro68k
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
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
void AC_update(REPORT_t *report)
{
	uint8_t	m0, m1;
	
	m0 = report->udlr_sscc & 0x0F;	// UDLR bits

	
	if (1)	// Atari / Commodore
	{
		if (report->buttons1_8 & 0x55)	// odd buttons
			m0 |= AC_JOY_B1_PIN_bm;
		if (report->buttons1_8 & 0xAA)	// even buttons
			m0 |= AC_JOY_B2_PIN_bm;

		if (report->buttons9_16 & 0x55)	// odd buttons
			m0 |= AC_JOY_B1_PIN_bm;
		if (report->buttons9_16 & 0xAA)	// even buttons
			m0 |= AC_JOY_B2_PIN_bm;

		m1 = m0;
	}

	if (0)	// PC Engine
	{
		m1 = 0;
		if (report->buttons1_8 & 0x11)	// I
			m1 |= AC_JOY_UP_PIN_bm;
		if (report->buttons1_8 & 0x22)	// II
			m1 |= AC_JOY_RIGHT_PIN_bm;
		if (report->buttons1_8 & 0x44)	// Select
			m1 |= AC_JOY_DOWN_PIN_bm;
		if (report->buttons1_8 & 0x88)	// Run
			m1 |= AC_JOY_LEFT_PIN_bm;

		if (report->buttons9_16 & 0x11)	// I
			m1 |= AC_JOY_UP_PIN_bm;
		if (report->buttons9_16 & 0x22)	// II
			m1 |= AC_JOY_RIGHT_PIN_bm;
		if (report->buttons9_16 & 0x44)	// Select
			m1 |= AC_JOY_DOWN_PIN_bm;
		if (report->buttons9_16 & 0x88)	// Run
			m1 |= AC_JOY_LEFT_PIN_bm;

		if (report->udlr_sscc & BUTTON_SELECT_bm)
			m1 |= AC_JOY_DOWN_PIN_bm;
		if (report->udlr_sscc & BUTTON_START_bm)
			m1 |= AC_JOY_LEFT_PIN_bm;
	}

	if (0)	// Megadrive
	{
		m1 = m0;	// UDLR
		if (report->buttons1_8 & 0x11)	// A
			m0 |= AC_JOY_B1_PIN_bm;
		if (report->buttons1_8 & 0x22)	// B
			m1 |= AC_JOY_B1_PIN_bm;
		if (report->buttons1_8 & 0x44)	// Start
			m0 |= AC_JOY_B2_PIN_bm;
		if (report->buttons1_8 & 0x88)	// C
			m1 |= AC_JOY_B2_PIN_bm;

		if (report->udlr_sscc & (BUTTON_START_bm | BUTTON_SELECT_bm))
			m0 |= AC_JOY_B2_PIN_bm;
	}

	cli();
	GPIO0 = m0;
	GPIO1 = m1;
	sei();
/*
	asm(
		"push	r18						\n\t"
		"in		r18, %[_GPIO0]			\n\t"
		"cli							\n\t"
		"sbic	%[_PORTC_IN], 2			\n\t"
		"in		r18, %[_GPIO1]			\n\t"
		"out	%[_VPORT0_OUT], r18		\n\t"
		"sei							\n\t"
		"pop	r18						\n\t"
		:
		: [_GPIO0]	"I" (_SFR_IO_ADDR(GPIO0))
		, [_GPIO1]	"I" (_SFR_IO_ADDR(GPIO1))
		, [_PORTC_IN]	"I" (_SFR_IO_ADDR(PORTC_IN))
		, [_VPORT0_OUT]	"I" (_SFR_IO_ADDR(VPORT0_OUT))
	);
*/
}
