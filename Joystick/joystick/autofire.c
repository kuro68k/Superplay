/*
 * autofire.c
 *
 * Created: 03/06/2015 21:17:54
 *  Author: MoJo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "autofire.h"


uint8_t	volatile	af1_pressed_SIG = 0;
uint8_t	volatile	af2_pressed_SIG = 0;

uint8_t	af_setting[16];


/**************************************************************************************************
** Set up autofire timers
*/
void AF_init(void)
{
	memset(af_setting, 0, sizeof(af_setting));
	
	AF_TC1.CTRLA = 0;
	AF_TC1.CTRLB = 0;
	AF_TC1.CTRLC = 0;
	AF_TC1.CTRLD = 0;
	AF_TC1.CTRLE = 0;
	AF_TC1.INTCTRLA = TC_OVFINTLVL_LO_gc;
	AF_TC1.INTCTRLB = 0;
	AF_TC1.CNT = 0;
	AF_TC1.PER = AF1_PER;
	AF_TC1.CTRLA = AF1_DIV;

	AF_TC2.CTRLA = 0;
	AF_TC2.CTRLB = 0;
	AF_TC2.CTRLC = 0;
	AF_TC2.CTRLD = 0;
	AF_TC2.CTRLE = 0;
	AF_TC2.INTCTRLA = TC_OVFINTLVL_LO_gc;
	AF_TC2.INTCTRLB = 0;
	AF_TC2.CNT = 0;
	AF_TC2.PER = AF2_PER;
	AF_TC2.CTRLA = AF2_DIV;
}

/**************************************************************************************************
** Timer overflow interrupt handlers
*/
ISR(AF_TC1_OVF_vect)
{
	af1_pressed_SIG = ~af1_pressed_SIG;
}

ISR(AF_TC2_OVF_vect)
{
	af2_pressed_SIG = ~af2_pressed_SIG;
}

/**************************************************************************************************
** Get autofire button mask
*/
uint16_t AF_get_mask(void)
{
	uint16_t	mask = 0;
	uint16_t	bit = 1;
	uint8_t		af1, af2;
	
	af1 = af1_pressed_SIG;
	af2 = af2_pressed_SIG;
	
	for (uint8_t i = 0; i < 16; i++)
	{
		switch(af_setting[i])
		{
			default:
			case 0:	// no autofire
				mask |= bit;
				break;
			
			case 1:	// AF1
				if (af1)
					mask |= bit;
				break;

			case 2:	// AF2
				if (af2)
					mask |= bit;
				break;
		}
	}
	
	return mask;
}