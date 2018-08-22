/*
 * main.c
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usb.h"
#include "dfu_config.h"


FUSES = {
	0xFF,		// fusebyte 0
	0x00,		// fusebyte 1
	0xBE,		// fusebyte 2 bootloader
	0xFF,		// fusebyte	3
	0xF3,		// fusebyte 4
	0xE4,		// fusebyte 5
};


typedef void (*AppPtr)(void) __attribute__ ((noreturn));

extern void CCPWrite(volatile uint8_t *address, uint8_t value);

volatile bool reset_flag = false;

int main(void)
{
	if (!CheckStartConditions())
	{
		// exit bootloader
		AppPtr application_vector = (AppPtr)0x000000;
		CCP = CCP_IOREG_gc;		// unlock IVSEL
		PMIC.CTRL = 0;			// disable interrupts, set vector table to app section
		EIND = 0;				// indirect jumps go to lower 128k of app section
		RAMPZ = 0;				// LPM uses lower 64k of flash
		application_vector();
	}

	CCPWrite(&PMIC.CTRL, PMIC_IVSEL_bm);

	usb_configure_clock();
	usb_init();
	PMIC.CTRL |= PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
	sei();
	usb_attach();

	while (!reset_flag);
	_delay_ms(25);
	usb_detach();
	_delay_ms(100);
	for(;;)
		CCPWrite(&RST.CTRL, RST_SWRST_bm);
}
