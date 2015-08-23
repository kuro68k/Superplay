/*
 * vendor.c
 *
 * Created: 04/04/2015 16:10:36
 *  Author: MoJo
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <asf.h>
#include "vendor.h"

bool VEN_enabled = false;


/**************************************************************************************************
** Enable vendor interface, set default state
*/
bool VEN_callback_enable(void)
{
	PORTF.OUTSET = PIN2_bm;
	VEN_enabled = true;
	return true;
}

/**************************************************************************************************
** Disable vendor interface
*/
bool VEN_callback_disable(void)
{
	PORTF.OUTCLR = PIN2_bm;
	VEN_enabled = false;
	return true;
}

/**************************************************************************************************
** Receive control data from host
*/
bool VEN_callback_setup_out_received(void)
{
	PORTF.OUTTGL = PIN0_bm;
	return true;
}

/**************************************************************************************************
** Interrupt token received, send any pending data to host
*/
bool VEN_callback_setup_in_received(void)
{
	PORTF.OUTTGL = PIN1_bm;
	return true;
}
