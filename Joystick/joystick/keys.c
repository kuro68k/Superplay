/*
 * keys.c
 *
 * Treat buttons as "keys", with simple debouncing and repeat inhibit etc.
 */

#include <avr/io.h>
#include "hw_misc.h"
#include "report.h"
#include "keys.h"

uint8_t		key_matrix[256];

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
void KEY_read(void)
{
	if (!(KEY_TC.INTFLAGS & TC0_OVFIF_bm))			// only re-read keys on timer overflow
		return 0;
	KEY_TC.INTFLAGS = TC0_OVFIF_bm;

	memcpy(key_matrix, input_matrix, sizeof(key_matrix));
}
