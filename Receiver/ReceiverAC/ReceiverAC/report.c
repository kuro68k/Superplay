/*
 * report.c
 *
 */

#include <avr/io.h>
#include <string.h>

#include "fastmem.h"
#include "config.h"
#include "report.h"

uint8_t		input_matrix[256];

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
	: [output] "x" (&input_matrix[128]), [input] "z" (buffer)
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

	//for (uint8_t i = 0; i < map->count; i++)
	//	input_matrix[map->mapping[i][0]] |= input_matrix[map->mapping[i][1]];

	void *im = (void *)input_matrix;
	void *mapping = (void *)&map->mapping[0][0];
	uint8_t count = map->count;

	asm volatile(
		"movw	r20, %a[im]"			"\n\t"
		"loop%=:"						"\n\t"
		"ld		r18, %a[mapping]+"		"\n\t"	// map->mapping[i][0]
		"ld		r19, %a[mapping]+"		"\n\t"	// map->mapping[i][1]

		"movw	%a[im], r20"			"\n\t"
		"add	%A[im], r19"			"\n\t"
		"adc	%B[im], r1"				"\n\t"
		"ld		r19, %a[im]"			"\n\t"	// kbus_matrix[map->mapping[i][1]]

		"movw	%a[im], r20"			"\n\t"
		"add	%A[im], r18"			"\n\t"
		"adc	%B[im], r1"				"\n\t"
		"ld		r18, %a[im]"			"\n\t"	// input_matrix[map->mapping[i][0]]

		"or		r18, r19"				"\n\t"
		"st		%a[im], r18"			"\n\t"
		"dec	%[count]"				"\n\t"
		"brne	loop%="					"\n\t"
	: [im] "+e" (im),
	  [mapping] "+e" (mapping),
	  [count] "+r" (count),
	  "=m" (input_matrix)
	:
	: "r18", "r19", "r20", "r21"
	);
}
