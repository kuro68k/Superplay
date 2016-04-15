/*
 * joystick.c
 *
 * Created: 29/03/2015 16:17:58
 *  Author: Paul Qureshi
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
#include "serial_num.h"
#include "usart.h"


#define VERSION_MAJOR	1
#define VERSION_MINOR	0

typedef struct {
	char		magic_string[8];
	uint8_t		version_major;
	uint8_t		version_minor;
	uint8_t		mcu_signature[3];
	uint32_t	flash_size_b;
	uint16_t	page_size_b;
	uint32_t	eeprom_size_b;
	uint16_t	eeprom_page_size_b;
} FW_INFO_t;

// data embedded in firmware image so that the bootloader program can read it
volatile const __flash FW_INFO_t firmware_info =	{	{ 0x59, 0x61, 0x6d, 0x61, 0x4e, 0x65, 0x6b, 0x6f },		// "YamaNeko" magic identifier
														VERSION_MAJOR,
														VERSION_MINOR,
														{ SIGNATURE_0, SIGNATURE_1, SIGNATURE_2 },
														APP_SECTION_SIZE,
														APP_SECTION_PAGE_SIZE,
														EEPROM_SIZE,
														EEPROM_PAGE_SIZE
													};

int main(void)
{
	firmware_info.magic_string[0];	// prevent firmware_info being optimized away
	
	PORTF.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm;
	PORTF.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm;

	sysclk_init();
	CFG_init();
	HW_init();
	AF_init();
	HID_init();
	USB_init_build_usb_serial_number();
	USART_init();

	//PORTCFG.CLKEVOUT = PORTCFG_CLKOUTSEL_CLK1X_gc | PORTCFG_CLKOUT_PC7_gc;
	//PORTC.DIRSET = PIN7_bm;

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
	sei();
	udc_start();
	udc_attach();

	for(;;)
	{
		WDR();

		RPT_logical_inputs_refresh();
		//AF_apply();
		
		//if (HID_enabled && !VEN_enabled)
			HID_send_report();
		//if (VEN_enabled)
		if(1)
		{
			RPT_refresh();
			udi_vendor_interrupt_in_run((uint8_t *)&report, sizeof(report), NULL);
		}
	}
}