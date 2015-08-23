/*
 * saturn_interrupt.S
 *
 * Created: 01/05/2015 15:16:35
 *  Author: MoJo
 */ 

#include <avr/io.h>

PORTA_INT0_vect:
	// save registers
	push	ZH
	in		ZH, SREG
	push	ZH
	push	ZL

	// select GPIO to output
	in		ZL, VPORT0_IN
	andi	ZL, 0x03			// mask lower 2 bits (TH, TR)
	eor		ZH, ZH				// GPIOs 0-3 are at addresses 0-3

	ld		ZH, Z				// read selected GPIO
	out		VPORT0_OUT, ZH

	// restore registers
	pop		ZL
	pop		ZH
	out		SREG, ZH
	pop		ZH
	reti