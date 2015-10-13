/*
 * autofire.c
 *
 * Created: 03/06/2015 21:17:54
 *  Author: Paul Qureshi
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
const __flash uint8_t clksel[] = {	TC_CLKSEL_DIV1_gc, TC_CLKSEL_DIV2_gc, TC_CLKSEL_DIV4_gc,
									TC_CLKSEL_DIV8_gc, TC_CLKSEL_DIV64_gc, TC_CLKSEL_DIV256_gc,
									TC_CLKSEL_DIV1024_gc };



uint8_t	af1_counters[16];
uint8_t af2_counters[16];
uint8_t volatile af1_count_AT = 0;	// !! ATOMIC !!
uint8_t volatile af2_count_AT = 0;	// !! ATOMIC !!

uint16_t	af_map = 0xFFFF;		// autofire state for each button
uint16_t	AF_high_map = 0;
uint16_t	AF_low_map = 0;


/**************************************************************************************************
** Find the best timer settings for a given frequency. Returns true if a usable (but possibly
** very inaccurate) setting is found.
*/
bool af_calc_timer(float freq, uint8_t *clksel, uint16_t *period)
{
	uint32_t	cycles;
	uint32_t	per;
	uint32_t	lowest_error = UINT32_MAX;
	bool		found = false;

	cycles = F_CPU / freq;

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
	memset(af1_counters, AF_CLKMUL, sizeof(af1_counters));
	memset(af2_counters, AF_CLKMUL, sizeof(af2_counters));

	// re-load settings if required
	if ((cfg->af_mode == CFG_AF_MODE_FIXED) ||
		(cfg->af_mode == CFG_AF_MODE_TOGGLE_HIGH))
		AF_high_map = cfg->af_mask;

	uint16_t	per = AF1_PER;
	uint8_t		clksel = AF1_CLKSEL;
	float		freq;
	
	freq = cfg->af_high_05hz / 2;	// convert o Hz
	freq *= AF_CLKMUL;				// timer runs AF_CLKMUL times faster than autofire
	if (!af_calc_timer(freq, &clksel, &per))
	{
		clksel = AF1_CLKSEL;
		per = AF1_PER;
	}

	AF_TC1.CTRLA = 0;
	AF_TC1.CTRLB = 0;
	AF_TC1.CTRLC = 0;
	AF_TC1.CTRLD = 0;
	AF_TC1.CTRLE = 0;
	AF_TC1.INTCTRLA = TC_OVFINTLVL_LO_gc;
	AF_TC1.INTCTRLB = 0;
	AF_TC1.CNT = 0;
	AF_TC1.PER = per;
	AF_TC1.CTRLA = clksel;

	freq = cfg->af_low_05hz / 2;	// convert o Hz
	freq *= 8;						// timer runs 8x faster than autofire
	if (!af_calc_timer(freq, &clksel, &per))
	{
		clksel = AF2_CLKSEL;
		per = AF2_PER;
	}

	AF_TC2.CTRLA = 0;
	AF_TC2.CTRLB = 0;
	AF_TC2.CTRLC = 0;
	AF_TC2.CTRLD = 0;
	AF_TC2.CTRLE = 0;
	AF_TC2.INTCTRLA = TC_OVFINTLVL_LO_gc;
	AF_TC2.INTCTRLB = 0;
	AF_TC2.CNT = 0;
	AF_TC2.PER = per;
	AF_TC2.CTRLA = clksel;
}

/**************************************************************************************************
** Timer overflow interrupt handlers
*/
ISR(AF_TC1_OVF_vect)
{
	af1_count_AT++;
}

ISR(AF_TC2_OVF_vect)
{
	af2_count_AT++;
}

/**************************************************************************************************
** Read current autofire state, and reset any non-pressed buttons
*/
uint16_t AF_read(uint16_t buttons, uint8_t button_mode)
{
	uint8_t		i;
	uint16_t	mask;
	uint8_t		af1_count, af2_count;

	// check AF settings
	switch(cfg->af_mode)
	{
		case CFG_AF_MODE_HIGH_LOW:
			AF_low_map = PORTC.IN & 0x0F;
			AF_low_map |= (PORTA.IN & 0xC0) >> 2;
			AF_high_map = PORTD.IN & 0x3F;
			break;
			
		case CFG_AF_MODE_HIGH_WITH_LEDS:
			AF_low_map = 0;
			AF_high_map = PORTD.IN & 0x3F;
			PORTC.OUT = (PORTC.OUT & 0xF0) | (AF_high_map & 0x0F);			// LEDs
			PORTA.OUT = (PORTA.OUT & 0x3F) | ((AF_high_map & 0x30) << 2);	//
			break;
		
		default:
		case CFG_AF_MODE_FIXED:
			AF_low_map = 0;
			AF_high_map = cfg->af_mask;
			break;
		
		case CFG_AF_MODE_TOGGLE_HIGH:
			AF_low_map = 0;
			if (PORTA.IN & START_PIN_bm)	// hold start to toggle
				AF_high_map ^= KEY_read();
			else
				KEY_clear();
			PORTC.OUT = (PORTC.OUT & 0xF0) | (AF_high_map & 0x0F);			// LEDs
			PORTA.OUT = (PORTA.OUT & 0x3F) | ((AF_high_map & 0x30) << 2);	//
			break;
	}

	if (button_mode == BUTTON_MODE_4AF_gc)
	{
		AF_low_map = 0;
		AF_high_map = 0xF0;
	}


	// non-AF buttons always active
	af_map |= ~AF_high_map;
	af_map |= ~AF_low_map;
	
	cli();
	af1_count = af1_count_AT;
	af1_count_AT = 0;
	af2_count = af2_count_AT;
	af2_count_AT = 0;
	sei();


	// decrement counters
	mask = 1;
	for (i = 0; i < 16; i++)
	{
		af1_counters[i] -= af1_count;
		while (af1_count > AF_CLKMUL)	// counter underflowed
		{
			af1_count += AF_CLKMUL;
			if (AF_high_map & mask)
				af_map ^= mask;
		}	

		af2_counters[i] -= af2_count;
		while (af2_count > AF_CLKMUL)	// counter underflowed
		{
			af2_count += AF_CLKMUL;
			if (AF_low_map & mask)
				af_map ^= mask;
		}	
	}

	
	// handle unpressed buttons
	mask = 1;
	for (i = 0; i < 16; i++)
	{
		if (!(buttons & mask))	// button not pressed
		{
			af1_counters[i] = AF_CLKMUL;	// reset counters so button engages instantly when pressed
			af2_counters[i] = AF_CLKMUL;
			af_map |= mask;
		}
	}
	
	return af_map;
}
