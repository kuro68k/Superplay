
/*
 * atari_asm.S
 *
 */

#include <avr/io.h>

.section text
.global AT_set_output


AT_set_output:
	push	r18
	in		r18, GPIO0
	cli
	sbic	VPORT1_IN, 2
	in		r18, GPIO1
	out		VPORT0_OUT, r18
	sei
	pop		r18
	ret


// handle the select pin changing
// does not affect the status register, no need to save it
PORTC_INT0_vect:
	push	r18
	in		r18, GPIO0
	sbic	VPORT1_IN, 2
	in		r18, GPIO1
	out		VPORT0_OUT, r18
	pop		r18
	reti
