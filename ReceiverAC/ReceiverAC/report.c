/*
 * report.c
 *
 */

#include <avr/io.h>
#include <string.h>

#include "global.h"
#include "config.h"
#include "report.h"

uint8_t		kbus_matrix[128];
uint8_t		input_matrix[128];

/**************************************************************************************************
** Decode 128 bits to 128 bytes to refresh kbus_matrix[]
*/
void RPT_decode_kbus_matrix(uint8_t *buffer)
{
	asm volatile(
		"ldi	__tmp_reg__, 0"		"\n\t"
		"ldi	r18, 16"			"\n\t"
		"loop%=:"					"\n\t"

		"ld		r19, Z+"			"\n\t"

		"bst	r19, 0"				"\n\t"
		"bld	__tmp_reg__, 0"		"\n\t"
		"st		X+, __tmp_reg__"	"\n\t"
		"bst	r19, 1"				"\n\t"
		"bld	__tmp_reg__, 0"		"\n\t"
		"st		X+, __tmp_reg__"	"\n\t"
		"bst	r19, 2"				"\n\t"
		"bld	__tmp_reg__, 0"		"\n\t"
		"st		X+, __tmp_reg__"	"\n\t"
		"bst	r19, 3"				"\n\t"
		"bld	__tmp_reg__, 0"		"\n\t"
		"st		X+, __tmp_reg__"	"\n\t"
		"bst	r19, 4"				"\n\t"
		"bld	__tmp_reg__, 0"		"\n\t"
		"st		X+, __tmp_reg__"	"\n\t"
		"bst	r19, 5"				"\n\t"
		"bld	__tmp_reg__, 0"		"\n\t"
		"st		X+, __tmp_reg__"	"\n\t"
		"bst	r19, 6"				"\n\t"
		"bld	__tmp_reg__, 0"		"\n\t"
		"st		X+, __tmp_reg__"	"\n\t"
		"bst	r19, 7"				"\n\t"
		"bld	__tmp_reg__, 0"		"\n\t"
		"st		X+, __tmp_reg__"	"\n\t"

		"dec	r18"				"\n\t"
		"brne	loop%="
	:
	: [output] "x" (input_matrix), [input] "z" (buffer)
	: "r18", "r19"
	);
}

/**************************************************************************************************
** Map inputs. Call RPT_decode_kbus_matrix() first.
*/
void RPT_refresh_input_matrix(void)
{
	memset(input_matrix, 0, 128);	// clear logical inputs
	input_matrix[LON] = 1;

	for (uint8_t i = 0; i < map->count; i++)
	{
		uint8_t input = map->mapping[i][0];
		if (input > 127)	// hold meta + this input
		{
			if (input_matrix[LMETA])
				input_matrix[input - 128] |= input_matrix[map->mapping[i][1]];
		}
		else
			input_matrix[input] |= input_matrix[map->mapping[i][1]];
	}
}
