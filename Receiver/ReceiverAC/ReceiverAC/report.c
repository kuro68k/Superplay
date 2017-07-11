/*
 * report.c
 *
 */

#include <avr/io.h>
#include <string.h>

#include "fastmem.h"
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
		"ldi	r20, 0"				"\n\t"
		"ldi	r18, 16"			"\n\t"
		"loop%=:"					"\n\t"

		"ld		r19, Z+"			"\n\t"

		"bst	r19, 0"				"\n\t"
		"bld	r20, 0"				"\n\t"
		"st		X+, r20"			"\n\t"
		"bst	r19, 1"				"\n\t"
		"bld	r20, 0"				"\n\t"
		"st		X+, r20"			"\n\t"
		"bst	r19, 2"				"\n\t"
		"bld	r20, 0"				"\n\t"
		"st		X+, r20"			"\n\t"
		"bst	r19, 3"				"\n\t"
		"bld	r20, 0"				"\n\t"
		"st		X+, r20"			"\n\t"
		"bst	r19, 4"				"\n\t"
		"bld	r20, 0"				"\n\t"
		"st		X+, r20"			"\n\t"
		"bst	r19, 5"				"\n\t"
		"bld	r20, 0"				"\n\t"
		"st		X+, r20"			"\n\t"
		"bst	r19, 6"				"\n\t"
		"bld	r20, 0"				"\n\t"
		"st		X+, r20"			"\n\t"
		"bst	r19, 7"				"\n\t"
		"bld	r20, 0"				"\n\t"
		"st		X+, r20"			"\n\t"

		"dec	r18"				"\n\t"
		"brne	loop%="
	:
	: [output] "x" (kbus_matrix), [input] "z" (buffer)
	: "r18", "r19", "r20"
	);
}

/**************************************************************************************************
** Map inputs. Call RPT_decode_kbus_matrix() first.
*/
void RPT_refresh_input_matrix(void)
{
	fmemset(input_matrix, 0, 128);	// clear logical inputs
	input_matrix[LON] = 1;

	for (uint8_t i = 0; i < map->count; i++)
	{
		uint8_t input = map->mapping[i][0];
		if (input & 0x80)	// hold meta + this input
		{
			if (input_matrix[LMETA])
				input_matrix[input & 0x7F] |= kbus_matrix[map->mapping[i][1]];
		}
		else
			input_matrix[input] |= kbus_matrix[map->mapping[i][1]];
	}
}
