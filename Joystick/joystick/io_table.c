/*
 * io_table.c
 *
 * Created: 20/10/2015 15:54:57
 *  Author: MoJo
 */ 

#include <avr/io.h>
#include "io_table.h"

// logical input to physical IO port mapping
#define IO_TABLE \
		ENTRY(NONE,			0, 0) \
		\
		ENTRY(JOY_UP,		&PORTA, PIN0_bm) \
		ENTRY(JOY_DN,		&PORTA, PIN1_bm) \
		ENTRY(JOY_LF,		&PORTA, PIN2_bm) \
		ENTRY(JOY_RT,		&PORTA, PIN3_bm) \
		ENTRY(START,		&PORTA, PIN4_bm) \
		ENTRY(COIN,			&PORTA, PIN5_bm) \
		ENTRY(AUTO_LOW_5,	&PORTA, PIN6_bm) \
		ENTRY(AUTO_LOW_6,	&PORTA, PIN7_bm) \
		\
		ENTRY(BUTTON1,		&PORTB, PIN0_bm) \
		ENTRY(BUTTON2,		&PORTB, PIN1_bm) \
		ENTRY(BUTTON3,		&PORTB, PIN2_bm) \
		ENTRY(BUTTON4,		&PORTB, PIN3_bm) \
		ENTRY(BUTTON5,		&PORTB, PIN4_bm) \
		ENTRY(BUTTON6,		&PORTB, PIN5_bm) \
		ENTRY(BUTTON7,		&PORTB, PIN6_bm) \
		ENTRY(BUTTON8,		&PORTB, PIN7_bm) \
		\
		ENTRY(AUTO_LOW_1,	&PORTC, PIN0_bm) \
		ENTRY(AUTO_LOW_2,	&PORTC, PIN1_bm) \
		ENTRY(AUTO_LOW_3,	&PORTC, PIN2_bm) \
		ENTRY(AUTO_LOW_4,	&PORTC, PIN3_bm) \
		\
		ENTRY(AUTO_HIGH_1,	&PORTD, PIN0_bm) \
		ENTRY(AUTO_HIGH_2,	&PORTD, PIN1_bm) \
		ENTRY(AUTO_HIGH_3,	&PORTD, PIN2_bm) \
		ENTRY(AUTO_HIGH_4,	&PORTD, PIN3_bm) \
		ENTRY(AUTO_HIGH_5,	&PORTD, PIN4_bm) \
		ENTRY(AUTO_HIGH_6,	&PORTD, PIN5_bm) \
		\
		ENTRY(ROTARY1,		&PORTE, PIN0_bm) \
		ENTRY(ROTARY2,		&PORTE, PIN1_bm) \
		ENTRY(ROTARY3,		&PORTE, PIN2_bm) \
		ENTRY(ROTARY4,		&PORTE, PIN3_bm) \
		ENTRY(ROTARY5,		&PORTE, PIN4_bm) \
		ENTRY(ROTARY6,		&PORTE, PIN5_bm) \
		ENTRY(ROTARY7,		&PORTE, PIN6_bm) \
		ENTRY(ROTARY8,		&PORTE, PIN7_bm) \
		\
		ENTRY(ROTARY9,		&PORTF, PIN0_bm) \
		ENTRY(ROTARY10,		&PORTE, PIN1_bm) \
		ENTRY(ROTARY11,		&PORTE, PIN2_bm) \
		ENTRY(ROTARY12,		&PORTE, PIN3_bm) \
		ENTRY(UNUSED,		&PORTE, PIN4_bm) \
		ENTRY(MODE_4,		&PORTE, PIN5_bm) \
		ENTRY(MODE_4AF,		&PORTE, PIN6_bm) \
		ENTRY(CONTROL,		&PORTE, PIN7_bm)


const __flash  IO_PIN_t	io_pin_table[] = {
#define	ENTRY(name, port, pin_mask)		{port, pin_mask},
	IO_TABLE
#undef ENTRY
};
