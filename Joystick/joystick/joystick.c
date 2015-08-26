/*
 * joystick.c
 *
 * Created: 29/03/2015 16:17:58
 *  Author: ???
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <asf.h>

#include "global.h"
#include "hw_misc.h"
#include "config.h"
#include "hid.h"
#include "report.h"
#include "vendor.h"
#include "autofire.h"

int main(void)
{
	PORTF.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm;
	PORTF.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm;

	sysclk_init();
	CFG_init();
	HW_init();
	AF_init();
	HID_init();

	//PORTCFG.CLKEVOUT = PORTCFG_CLKOUTSEL_CLK1X_gc | PORTCFG_CLKOUT_PC7_gc;
	//PORTC.DIRSET = PIN7_bm;

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
	sei();
	udc_start();
	udc_attach();

	for(;;)
	{
		WDR();

		RPT_refresh();
		if (HID_enabled && !VEN_enabled)
			HID_send_report();
		if (VEN_enabled)
			udi_vendor_interrupt_in_run((uint8_t *)&report, sizeof(report), NULL);
	}
}