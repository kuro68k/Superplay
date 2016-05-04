/*
 * atari.c
 *
 * Created: 04/05/2016 15:39:10
 *  Author: paul.qureshi
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include "report.h"
#include "atari.h"

/**************************************************************************************************
* Update outputs
*/
void AC_update(REPORT_t *report)
{
	uint8_t p = report->udlr_sscc & 0x0F;	// UDLR bits
	p |= (report->buttons1_8 & 0x0F) << 4;
	AC_PORT.OUT = p;
}