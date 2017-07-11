/*
 * report.c
 *
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include <string.h>

#include "fastmem.h"
#include "global.h"
#include "hw_misc.h"
#include "config.h"
#include "report.h"
#include "io_table.h"

uint8_t		input_matrix[256];


/**************************************************************************************************
** Set up reports
*/
void RPT_init(void)
{
	//memset(input_matrix, 0, sizeof(input_matrix));
	//input_matrix[LON] = 1;
	//input_matrix[PON] = 1;
}

/**************************************************************************************************
** Refresh physical inputs
*/
void rpt_physical_inputs_refresh(void)
{
#ifndef HARDWARE_V1
	uint8_t p = PORTA.IN;
	input_matrix[PB16] = p & PIN0_bm ? 1 : 0;
	input_matrix[PB15] = p & PIN1_bm ? 1 : 0;
	input_matrix[PB14] = p & PIN2_bm ? 1 : 0;
	input_matrix[PB13] = p & PIN3_bm ? 1 : 0;
	input_matrix[PB12] = p & PIN4_bm ? 1 : 0;
	input_matrix[PB11] = p & PIN5_bm ? 1 : 0;
	input_matrix[PB10] = p & PIN6_bm ? 1 : 0;
	input_matrix[PB9] = p & PIN7_bm ? 1 : 0;

	p = PORTB.IN;
	input_matrix[PB8] = p & PIN0_bm ? 1 : 0;
	input_matrix[PB7] = p & PIN1_bm ? 1 : 0;
	input_matrix[PB6] = p & PIN2_bm ? 1 : 0;
	input_matrix[PB5] = p & PIN3_bm ? 1 : 0;
	input_matrix[PB4] = p & PIN4_bm ? 1 : 0;
	input_matrix[PB3] = p & PIN5_bm ? 1 : 0;
	input_matrix[PB2] = p & PIN6_bm ? 1 : 0;
	input_matrix[PB1] = p & PIN7_bm ? 1 : 0;

	p = PORTC.IN;
	input_matrix[PJOY_UP] = p & PIN4_bm ? 1 : 0;
	input_matrix[PJOY_DN] = p & PIN5_bm ? 1 : 0;
	input_matrix[PJOY_LF] = p & PIN6_bm ? 1 : 0;
	input_matrix[PJOY_RT] = p & PIN7_bm ? 1 : 0;

	p = PORTD.IN;
	input_matrix[PMETA] = p & PIN0_bm ? 1 : 0;
	input_matrix[PLED] = p & PIN1_bm ? 1 : 0;
	input_matrix[PA4] = p & PIN2_bm ? 1 : 0;
	input_matrix[PA3] = p & PIN3_bm ? 1 : 0;
	input_matrix[PA2] = p & PIN4_bm ? 1 : 0;
	input_matrix[PA1] = p & PIN5_bm ? 1 : 0;

	p = PORTE.IN;
	input_matrix[PA8] = p & PIN0_bm ? 1 : 0;
	input_matrix[PA7] = p & PIN1_bm ? 1 : 0;
	input_matrix[PA6] = p & PIN2_bm ? 1 : 0;
	input_matrix[PA5] = p & PIN3_bm ? 1 : 0;
	input_matrix[PR4] = p & PIN4_bm ? 1 : 0;
	input_matrix[PR3] = p & PIN5_bm ? 1 : 0;
	input_matrix[PR2] = p & PIN6_bm ? 1 : 0;
	input_matrix[PR1] = p & PIN7_bm ? 1 : 0;

	p = PORTF.IN;
	input_matrix[PR8] = p & PIN0_bm ? 1 : 0;
	input_matrix[PR7] = p & PIN1_bm ? 1 : 0;
	input_matrix[PR6] = p & PIN2_bm ? 1 : 0;
	input_matrix[PR5] = p & PIN3_bm ? 1 : 0;
	input_matrix[PR12] = p & PIN4_bm ? 1 : 0;
	input_matrix[PR11] = p & PIN5_bm ? 1 : 0;
	input_matrix[PR10] = p & PIN6_bm ? 1 : 0;
	input_matrix[PR9] = p & PIN7_bm ? 1 : 0;

#else
	uint8_t p = PORTA.IN;
	input_matrix[PJOY_UP] = p & PIN0_bm ? 1 : 0;
	input_matrix[PJOY_DN] = p & PIN1_bm ? 1 : 0;
	input_matrix[PJOY_LF] = p & PIN2_bm ? 1 : 0;
	input_matrix[PJOY_RT] = p & PIN3_bm ? 1 : 0;
	input_matrix[PB16] = p & PIN4_bm ? 1 : 0;
	input_matrix[PB15] = p & PIN5_bm ? 1 : 0;
	input_matrix[PA5] = p & PIN6_bm ? 1 : 0;
	input_matrix[PA6] = p & PIN7_bm ? 1 : 0;

	p = PORTB.IN;
	input_matrix[PB1] = p & PIN0_bm ? 1 : 0;
	input_matrix[PB2] = p & PIN1_bm ? 1 : 0;
	input_matrix[PB3] = p & PIN2_bm ? 1 : 0;
	input_matrix[PB4] = p & PIN3_bm ? 1 : 0;
	input_matrix[PB5] = p & PIN4_bm ? 1 : 0;
	input_matrix[PB6] = p & PIN5_bm ? 1 : 0;
	input_matrix[PB7] = p & PIN6_bm ? 1 : 0;
	input_matrix[PB8] = p & PIN7_bm ? 1 : 0;

	p = PORTC.IN;
	input_matrix[PA1] = p & PIN1_bm ? 1 : 0;
	input_matrix[PA2] = p & PIN2_bm ? 1 : 0;
	input_matrix[PA3] = p & PIN3_bm ? 1 : 0;
	input_matrix[PA4] = p & PIN4_bm ? 1 : 0;

	p = PORTD.IN;
	input_matrix[PA7] = p & PIN0_bm ? 1 : 0;
	input_matrix[PA8] = p & PIN1_bm ? 1 : 0;
	input_matrix[PA9] = p & PIN2_bm ? 1 : 0;
	input_matrix[PA10] = p & PIN3_bm ? 1 : 0;
	input_matrix[PA11] = p & PIN4_bm ? 1 : 0;
	input_matrix[PA12] = p & PIN5_bm ? 1 : 0;

	p = PORTE.IN;
	input_matrix[PR12] = p & PIN0_bm ? 1 : 0;
	input_matrix[PR11] = p & PIN1_bm ? 1 : 0;
	input_matrix[PR10] = p & PIN2_bm ? 1 : 0;
	input_matrix[PA9] = p & PIN3_bm ? 1 : 0;
	input_matrix[PR8] = p & PIN4_bm ? 1 : 0;
	input_matrix[PR7] = p & PIN5_bm ? 1 : 0;
	input_matrix[PR6] = p & PIN6_bm ? 1 : 0;
	input_matrix[PR5] = p & PIN7_bm ? 1 : 0;

	p = PORTF.IN;
	input_matrix[PR4] = p & PIN0_bm ? 1 : 0;
	input_matrix[PR3] = p & PIN1_bm ? 1 : 0;
	input_matrix[PR2] = p & PIN2_bm ? 1 : 0;
	input_matrix[PR1] = p & PIN3_bm ? 1 : 0;
	input_matrix[PB14] = p & PIN4_bm ? 1 : 0;
	input_matrix[PR13] = p & PIN5_bm ? 1 : 0;
	input_matrix[PR12] = p & PIN6_bm ? 1 : 0;
	input_matrix[PMETA] = p & PIN7_bm ? 1 : 0;

#endif
}

/**************************************************************************************************
** Map physical to logical inputs
*/
void RPT_refresh_input_matrix(void)
{
	fmemset(input_matrix, 0, 256);
	input_matrix[LON] = 1;
	input_matrix[PON] = 1;

	rpt_physical_inputs_refresh();

//PORTA.OUTSET = PIN0_bm;
//	for (uint8_t i = 0; i < map->count; i++)
//		input_matrix[map->mapping[i][0]] |= input_matrix[map->mapping[i][1]];

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
//PORTA.OUTCLR = PIN0_bm;
}

/**************************************************************************************************
** Update LED control outputs
*/
void RPT_refresh_leds(void)
{
	// set output pins
	for (uint8_t i = 0; i < 16; i++)
	{
		uint8_t pio = settings->leds[i];	// physical I/O
		if ((pio != 0) &&
			io_pin_table[pio].port != 0)
		{
			if (input_matrix[LLED1 + i])
				io_pin_table[pio].port->OUTSET = io_pin_table[pio].pin_mask;
			else
				io_pin_table[pio].port->OUTCLR = io_pin_table[pio].pin_mask;
		}
	}
}

/**************************************************************************************************
** Generate a 128 bit (16 byte) report. Buffer must be at least 16 bytes.
*/
void RPT_generate_report(uint8_t *buffer)
{
	asm volatile(
		"ldi	r18, 16"			"\n\t"
		"loop%=:"					"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 0
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 0"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 1
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 1"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 2
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 2"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 3
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 3"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 4
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 4"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 5
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 5"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 6
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 6"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 7
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 7"				"\n\t"
		"st		Z+, r19"			"\n\t"
		"dec	r18"				"\n\t"
		"brne	loop%="
	:
	: [input] "x" (input_matrix), [output] "z" (buffer)
	: "r18", "r19"
	);
}