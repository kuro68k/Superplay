/*
 * io_table.c
 *
 */

#include <avr/io.h>
#include "config.h"
#include "io_table.h"

// logical input to physical IO port mapping
// V2.0 hardware
#define IO_TABLE \
		ENTRY(PB16,			&PORTA, PIN0_bm) \
		ENTRY(PB15,			&PORTA, PIN1_bm) \
		ENTRY(PB14,			&PORTA, PIN2_bm) \
		ENTRY(PB13,			&PORTA, PIN3_bm) \
		ENTRY(PB12,			&PORTA, PIN4_bm) \
		ENTRY(PB11,			&PORTA, PIN5_bm) \
		ENTRY(PB10,			&PORTA, PIN6_bm) \
		ENTRY(PB9,			&PORTA, PIN7_bm) \
		\
		ENTRY(PB8,			&PORTB, PIN0_bm) \
		ENTRY(PB7,			&PORTB, PIN1_bm) \
		ENTRY(PB6,			&PORTB, PIN2_bm) \
		ENTRY(PB5,			&PORTB, PIN3_bm) \
		ENTRY(PB4,			&PORTB, PIN4_bm) \
		ENTRY(PB3,			&PORTB, PIN5_bm) \
		ENTRY(PB2,			&PORTB, PIN6_bm) \
		ENTRY(PB1,			&PORTB, PIN7_bm) \
		\
		ENTRY(PJOY_UP,		&PORTC, PIN4_bm) \
		ENTRY(PJOY_DN,		&PORTC, PIN5_bm) \
		ENTRY(PJOY_LF,		&PORTC, PIN6_bm) \
		ENTRY(PJOY_RT,		&PORTC, PIN7_bm) \
		\
		ENTRY(PMETA,		&PORTD, PIN0_bm) \
		ENTRY(PLED,			&PORTD, PIN1_bm) \
		ENTRY(PA4,			&PORTD, PIN2_bm) \
		ENTRY(PA3,			&PORTD, PIN3_bm) \
		ENTRY(PA2,			&PORTD, PIN4_bm) \
		ENTRY(PA1,			&PORTD, PIN5_bm) \
		\
		ENTRY(PA8,			&PORTE, PIN0_bm) \
		ENTRY(PA7,			&PORTE, PIN1_bm) \
		ENTRY(PA6,			&PORTE, PIN2_bm) \
		ENTRY(PA5,			&PORTE, PIN3_bm) \
		ENTRY(PR4,			&PORTE, PIN4_bm) \
		ENTRY(PR3,			&PORTE, PIN5_bm) \
		ENTRY(PR2,			&PORTE, PIN6_bm) \
		ENTRY(PR1,			&PORTE, PIN7_bm) \
		\
		ENTRY(PR8,			&PORTF, PIN0_bm) \
		ENTRY(PR7,			&PORTF, PIN1_bm) \
		ENTRY(PR6,			&PORTF, PIN2_bm) \
		ENTRY(PR5,			&PORTF, PIN3_bm) \
		ENTRY(PA12,			&PORTF, PIN4_bm) \
		ENTRY(PA11,			&PORTF, PIN5_bm) \
		ENTRY(PA10,			&PORTF, PIN6_bm) \
		ENTRY(PA9,			&PORTF, PIN7_bm)


const __flash  IO_PIN_t	io_pin_table[256] = {
#define	ENTRY(name, port, pin_mask)		[name] = {port, pin_mask},
	IO_TABLE
#undef ENTRY
};
