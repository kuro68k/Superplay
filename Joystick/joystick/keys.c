/*
 * keys.c
 *
 * Treat buttons as "keys", with simple debouncing and repeat inhibit etc.
 */

#include <avr/io.h>
#include "hw_misc.h"
#include "keys.h"

uint8_t	last_keys = 0;

/**************************************************************************************************
* Set up key input system after reset
*/
void KEY_init(void)
{
	KEY_TC.CTRLA = 0;
	KEY_TC.CTRLB = 0;
	KEY_TC.CTRLC = 0;
	KEY_TC.CTRLD = 0;
	KEY_TC.CTRLE = 0;
	KEY_TC.INTFLAGS = TC0_OVFIF_bm;
	KEY_TC.INTCTRLA = 0;
	KEY_TC.INTCTRLB = 0;
	KEY_TC.PER = KEY_TC_PER;
	KEY_TC.CNT = 0;
	KEY_TC.CTRLA = KEY_TC_DIV;
}

/**************************************************************************************************
* Read keys
*/
uint8_t KEY_read(void)
{
	if (!(KEY_TC.INTFLAGS & TC0_OVFIF_bm))			// only re-read keys on timer overflow
		return 0;
	KEY_TC.INTFLAGS = TC0_OVFIF_bm;

	uint8_t	pressed_keys;
	uint8_t new_keys = PORTB.IN;
	last_keys &= new_keys;							// clear any no-longer pressed keys
	pressed_keys = new_keys & ~last_keys;			// return only newly pressed keys
	last_keys = new_keys;

	return pressed_keys;
}

/**************************************************************************************************
* Clear all pressed keys until released and pressed again
*/
void KEY_clear(void)
{
	last_keys = 0xFF;
}