/*
 * keys.c
 *
 * Meta "keys"
 */

#include <avr/io.h>
#include <string.h>
#include <stdbool.h>
#include "hw_misc.h"
#include "config.h"
#include "report.h"
#include "keys.h"

#define	DEBOUNCE	10

uint8_t		key_matrix[32];
uint8_t		meta_af_toggle[16];
bool		meta_af_select_high = false;
uint8_t		meta_af_set[16];

/**************************************************************************************************
* Set up key input system after reset
*/
void KEY_init(void)
{
	//KEY_TC.CTRLA = 0;
	//KEY_TC.CTRLB = 0;
	//KEY_TC.CTRLC = 0;
	//KEY_TC.CTRLD = 0;
	//KEY_TC.CTRLE = 0;
	//KEY_TC.INTFLAGS = TC0_OVFIF_bm;
	//KEY_TC.INTCTRLA = 0;
	//KEY_TC.INTCTRLB = 0;
	//KEY_TC.PER = KEY_TC_PER;
	//KEY_TC.CNT = 0;
	//KEY_TC.CTRLA = KEY_TC_DIV;

	uint16_t mask = 1;
	for (uint8_t i = 0; i < 16; i++)
	{
		if (settings->meta_af_toggle & mask)
			meta_af_toggle[i] = 1;
		mask <<= 1;
	}
}

/**************************************************************************************************
* Read keys
*/
void KEY_read(void)
{
	static uint8_t idx = 0;

	if (key_matrix[idx] > 0)
		key_matrix[idx]--;

	if (input_matrix[LMETA])
	{
		if (input_matrix[idx] && (key_matrix[idx] = 0))		// newly pressed
		{
			key_matrix[idx] = DEBOUNCE;

			if (idx >= LBUTTON1)			// AF toggle
			{
				uint8_t i = idx - LBUTTON1;
				if (meta_af_toggle[i])
					meta_af_set[i] ^= 1;
			}
			else							// stick meta controls
			{
				switch(idx)
				{
					case LJOY_UP:
						if (settings->meta_af_select_with_stick)
							meta_af_select_high = true;
						break;
					case LJOY_DN:
						if (settings->meta_af_select_with_stick)
							meta_af_select_high = false;
						break;
					case LJOY_LF:
						//if (settings->meta_mapping_select_with_stick)
							//
						break;
					case LJOY_RT:
						//if (settings->meta_mapping_select_with_stick)
							//
						break;
				}
			}
		}
	}

	idx++;
	idx &= 0x1F;

	// apply AF
	void *af_matrix;
	if (meta_af_select_high) af_matrix = &input_matrix[LAF_HIGH_1];
	else					 af_matrix = &input_matrix[LAF_LOW_1];

#define ORM		"ld		__tmp_reg__, X"		"\n\t"	\
				"ld		r18, Z+"			"\n\t"	\
				"or		__tmp_reg__, r18"	"\n\t"	\
				"st		X+, __tmp_reg__"	"\n\t"

	asm volatile(
		ORM ORM ORM ORM ORM ORM ORM ORM ORM ORM ORM ORM ORM ORM ORM ORM		// 16
	:
	: "x" (af_matrix),
	  "z" (meta_af_set)
	: "r18"
	);
}
