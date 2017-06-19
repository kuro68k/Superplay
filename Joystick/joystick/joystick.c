/*
 * joystick.c
 *
 */


#include <avr/io.h>
#include <util/delay.h>
#include <asf.h>

#include "global.h"
#include "hw_misc.h"
#include "config.h"
#include "hid.h"
#include "report.h"
#include "autofire.h"
#include "serial_num.h"
#include "usart.h"
#include "aux_buttons.h"
#include "keys.h"


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
	AB_init();

	//PORTCFG.CLKEVOUT = PORTCFG_CLKOUTSEL_CLK1X_gc | PORTCFG_CLKOUT_PC7_gc;
	//PORTC.DIRSET = PIN7_bm;

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
	sei();
	udc_start();
	udc_attach();

PORTA.DIRSET = PIN0_bm;

	for(;;)
	{
		WDR();

//PORTA.OUTSET = PIN0_bm;
		RPT_refresh_input_matrix();
//PORTA.OUTCLR = PIN0_bm;
		AF_apply();
		RPT_refresh_leds();
		//KEY_read();

		//if (cfg->aux_mode == AUX_MODE_BUTTONS)
		//	AB_send_report();

		HID_send_report();
	}
}