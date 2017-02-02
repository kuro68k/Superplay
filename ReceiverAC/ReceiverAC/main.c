/*
 * ReceiverAC.c
 *
 * KBUS receiver for Atari/Commodore and other simple directly connected controllers
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "hw_misc.h"
#include "config.h"
#include "kbus.h"

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

#define VERSION_MAJOR	0
#define VERSION_MINOR	1

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

	WATCHDOG_ON;

	// set idle sleep mode, turn off stuff we don't need
	SLEEP.CTRL	= SLEEP_SMODE_IDLE_gc | SLEEP_SEN_bm;
	PR.PRGEN	= PR_XCL_bm | PR_RTC_bm;
	PR.PRPA		= PR_DAC_bm | PR_ADC_bm | PR_AC_bm;
	PR.PRPC		= PR_TWI_bm | PR_USART0_bm | PR_SPI_bm | PR_HIRES_bm | PR_TC5_bm;
	PR.PRPD		= PR_TWI_bm | PR_SPI_bm | PR_HIRES_bm | PR_TC4_bm | PR_TC5_bm;

	CFG_init();
	HW_init();
	KBUS_init();

	// start interrupts
	HW_CCPWrite(&PMIC.CTRL, PMIC_RREN_bm | PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm);
	sei();

	KBUS_run();
}

