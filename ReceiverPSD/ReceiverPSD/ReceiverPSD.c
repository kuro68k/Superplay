/*
 * ReceiverPSD.c
 *
 * Created: 27/03/2015 15:18:16
 *  Author: MoJo
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "hw_misc.h"
#include "saturn.h"
#include "kbus.h"

int main(void)
{
	// set idle sleep mode, turn off stuff we don't need
	SLEEP.CTRL	= SLEEP_SMODE_IDLE_gc | SLEEP_SEN_bm;
	PR.PRGEN	= PR_USB_bm | PR_AES_bm | PR_RTC_bm;
	PR.PRPA		= PR_ADC_bm | PR_AC_bm;
	PR.PRPB		= PR_DAC_bm | PR_ADC_bm | PR_AC_bm;
	PR.PRPC		= PR_USART1_bm | PR_USART0_bm | PR_HIRES_bm;
	PR.PRPD		= PR_TWI_bm | PR_USART1_bm | PR_USART0_bm | PR_SPI_bm | PR_HIRES_bm;
	PR.PRPE		= PR_TWI_bm | PR_USART1_bm | PR_SPI_bm | PR_HIRES_bm;
	
	HW_init();
	KBUS_init();
	SAT_init();

	// start interrupts
	HW_CCPWrite(&PMIC.CTRL, PMIC_RREN_bm | PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm);
	sei();

	KBUS_run();
}