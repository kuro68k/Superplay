/*
 * autofire.c
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "hw_misc.h"
#include "config.h"
#include "report.h"
#include "keys.h"
#include "autofire.h"


const __flash uint16_t prescalers[] = { 1, 2, 4, 8, 64, 256, 1024 };
const __flash uint8_t clksel[] = {	TC_TC0_CLKSEL_DIV1_gc, TC_TC0_CLKSEL_DIV2_gc, TC_TC0_CLKSEL_DIV4_gc,
									TC_TC0_CLKSEL_DIV8_gc, TC_TC0_CLKSEL_DIV64_gc, TC_TC0_CLKSEL_DIV256_gc,
									TC_TC0_CLKSEL_DIV1024_gc };



uint8_t	af_high_counters[16];
uint8_t af_low_counters[16];
uint8_t volatile af_high_count_AT = 0;	// !! ATOMIC !!
uint8_t volatile af_low_count_AT = 0;	// !! ATOMIC !!

uint16_t	af_map = 0xFFFF;		// autofire state for each button
uint16_t	AF_high_map = 0;
uint16_t	AF_low_map = 0;


/**************************************************************************************************
** Find the best timer settings for a given frequency. Returns true if a usable (but possibly
** very inaccurate) setting is found.
*/
bool af_calc_timer(float freq_hz, uint8_t *clksel, uint16_t *period)
{
	uint32_t	cycles;
	uint32_t	per;
	uint32_t	lowest_error = UINT32_MAX;
	bool		found = false;

	cycles = F_CPU / freq_hz;

	// find best prescaler
	for (uint8_t i = 0; i < (sizeof(prescalers) / sizeof(uint16_t)); i++)
	{
		per = cycles / prescalers[i];
		per--;
		if (per > 0xFFFF)	// unusable
			continue;

		uint32_t error;
		error = abs((int32_t)cycles - (int32_t)((per + 1) * prescalers[i]));
		if (error < lowest_error)
		{
			lowest_error = error;
			*clksel = clksel[i];
			*period = per;
			found = true;
		}
	}

	return found;
}

/**************************************************************************************************
** Set up autofire timers
*/
void AF_init(void)
{
	memset(af_high_counters, AF_CLKMUL, sizeof(af_high_counters));
	memset(af_low_counters, AF_CLKMUL, sizeof(af_low_counters));

	uint16_t	per = AF1_PER;
	uint8_t		clksel = AF1_CLKSEL;
	float		freq_hz;
/*
	freq_hz = settings->af_high_hz;
	freq_hz *= AF_CLKMUL;				// timer runs AF_CLKMUL times faster than autofire
	if (!af_calc_timer(freq_hz, &clksel, &per))
	{
		clksel = AF1_CLKSEL;
		per = AF1_PER;
	}

	AF_TC1.CTRLA = 0;
	AF_TC1.CTRLB = 0;
	AF_TC1.CTRLC = 0;
	AF_TC1.CTRLD = 0;
	AF_TC1.CTRLE = 0;
	AF_TC1.INTCTRLA = TC_TC0_OVFINTLVL_LO_gc;
	AF_TC1.INTCTRLB = 0;
	AF_TC1.CNT = 0;
	AF_TC1.PER = per;
	AF_TC1.CTRLA = clksel;

	freq_hz = cfg->af_low_05hz / 2;	// convert to Hz
	freq_hz *= 8;						// timer runs 8x faster than autofire
	if (!af_calc_timer(freq_hz, &clksel, &per))
	{
		clksel = AF2_CLKSEL;
		per = AF2_PER;
	}

	AF_TC2.CTRLA = 0;
	AF_TC2.CTRLB = 0;
	AF_TC2.CTRLC = 0;
	AF_TC2.CTRLD = 0;
	AF_TC2.CTRLE = 0;
	AF_TC2.INTCTRLA = TC_TC0_OVFINTLVL_LO_gc;
	AF_TC2.INTCTRLB = 0;
	AF_TC2.CNT = 0;
	AF_TC2.PER = per;
	AF_TC2.CTRLA = clksel;
*/
}

/**************************************************************************************************
** Timer overflow interrupt handlers
*/
ISR(AF_TC1_OVF_vect)
{
	af_high_count_AT++;
}

ISR(AF_TC2_OVF_vect)
{
	af_low_count_AT++;
}

/**************************************************************************************************
** Apply autofire to selected buttons
*/
void AF_apply(void)
{
	uint8_t		i;
	uint16_t	mask;
	uint8_t		af_high_count, af_low_count;


	cli();
	af_high_count = af_high_count_AT;
	af_high_count_AT = 0;
	af_low_count = af_low_count_AT;
	af_low_count_AT = 0;
	sei();


	// decrement counters
	/* FIXME
	mask = 1;
	for (i = 0; i < 16; i++)
	{
		af_high_counters[i] -= af_high_count;
		while (af_high_count > AF_CLKMUL)	// counter underflowed
		{
			af_high_count += AF_CLKMUL;
			if (logical_inputs[LAF_HIGH_1 + i])
				af_map ^= mask;
		}

		af_low_counters[i] -= af_low_count;
		while (af_low_count > AF_CLKMUL)	// counter underflowed
		{
			af_low_count += AF_CLKMUL;
			if (logical_inputs[LAF_LOW_1 + i])
				af_map ^= mask;
		}
	}


	// handle unpressed buttons
	mask = 1;
	for (i = 0; i < 16; i++)
	{
		if (!logical_inputs[LBUTTON1 + i])		// button not pressed
		{
			af_high_counters[i] = AF_CLKMUL;	// reset counters so button engages instantly when pressed
			af_low_counters[i] = AF_CLKMUL;
			af_map |= mask;
		}
	}


	// apply AF settings
	mask = 1;
	for (i = 0; i < 16; i++)
	{
		if (!(af_map & mask))
			logical_inputs[LBUTTON1 + i] = 0;
	}
	*/
}
