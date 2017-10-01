/*
 * hw_misc.c
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "global.h"
#include "config.h"
#include "hw_misc.h"
#include "io_table.h"

FUSES = {
	0xFF,		// fusebyte 0
	0x00,		// fusebyte 1
	0xBE,		// fusebyte 2 bootloader
	//0xFE,		// fusebyte 2 application
	0xFF,		// fusebyte	3
	0xF3,		// fusebyte 4
	0xE4,		// fusebyte 5
};


uint8_t	HW_last_reset_status;


/**************************************************************************************************
** Reconfigure output pins based on settings
*/
void hw_configure_outputs(void)
{
	uint16_t mask = 1;
	for (uint8_t i = 0; i < 16; i++)
	{
		uint8_t pio = settings->leds[i];	// physical I/O
		if ((pio != 0) &&
			io_pin_table[pio].port != 0)
		{
			io_pin_table[pio].port->DIRSET = io_pin_table[pio].pin_mask;
			if (settings->led_output_inversion_map & mask)
			{
				switch(io_pin_table[pio].pin_mask)
				{
					case PIN0_bm:
						ENABLE_PULLUP(io_pin_table[pio].port->PIN0CTRL);
						break;
					case PIN1_bm:
						ENABLE_PULLUP(io_pin_table[pio].port->PIN1CTRL);
						break;
					case PIN2_bm:
						ENABLE_PULLUP(io_pin_table[pio].port->PIN2CTRL);
						break;
					case PIN3_bm:
						ENABLE_PULLUP(io_pin_table[pio].port->PIN3CTRL);
						break;
					case PIN4_bm:
						ENABLE_PULLUP(io_pin_table[pio].port->PIN4CTRL);
						break;
					case PIN5_bm:
						ENABLE_PULLUP(io_pin_table[pio].port->PIN5CTRL);
						break;
					case PIN6_bm:
						ENABLE_PULLUP(io_pin_table[pio].port->PIN6CTRL);
						break;
					case PIN7_bm:
						ENABLE_PULLUP(io_pin_table[pio].port->PIN7CTRL);
						break;
				}
			}
		}
	}
}

/**************************************************************************************************
** Set up hardware after reset
*/
void HW_init(void)
{
	HW_last_reset_status = RST.STATUS;
	RST.STATUS = 0xFF;		// clear all reset flags

	// start watchdog
	WDR();
	while (WDT.STATUS & WDT_SYNCBUSY_bm);
	WDR();
	HW_CCPWrite(&WDT_CTRL, WDT_PER_128CLK_gc | WDT_ENABLE_bm | WDT_CEN_bm);

	// switch inputs inverted to make mapping slightly more efficient

	// port A
	PORTA.OUT = 0;
	PORTA.DIR = 0;
	PORTCFG.MPCMASK = 0xFF;
	PORTA.PIN0CTRL = PORT_INVEN_bm | PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
	//ENABLE_PULLUP(PORTA.PIN0CTRL);

	// port B
	PORTB.OUT = 0;
	PORTB.DIR = 0;
	PORTCFG.MPCMASK = 0xFF;
	PORTB.PIN0CTRL = PORT_INVEN_bm | PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
	//ENABLE_PULLUP(PORTB.PIN0CTRL);

	// port C
	PORTC.OUT = 0;
	PORTC.DIR = 0;
	PORTCFG.MPCMASK = 0x0F;
	ENABLE_PULLUP(PORTC.PIN0CTRL);
	PORTCFG.MPCMASK = 0xF0;
	PORTC.PIN0CTRL = PORT_INVEN_bm | PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;

	// port D
	PORTD.OUT = 0;
	PORTD.DIR = 0;
	PORTCFG.MPCMASK = PIN6_bm | PIN7_bm;
	PORTD.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
	//ENABLE_PULLUP(PORTD.PIN0CTRL);
	PORTCFG.MPCMASK = (uint8_t)~(PIN6_bm | PIN7_bm);
	PORTD.PIN0CTRL = PORT_INVEN_bm | PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;

	// port E
	PORTE.OUT = 0;
	PORTE.DIR = 0;
	PORTCFG.MPCMASK = 0xFF;
	PORTE.PIN0CTRL = PORT_INVEN_bm | PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
	//ENABLE_PULLUP(PORTE.PIN0CTRL);

	// port F
	PORTF.OUT = 0;
	PORTF.DIR = 0;
	PORTCFG.MPCMASK = 0xFF;
	PORTF.PIN0CTRL = PORT_INVEN_bm | PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
	//ENABLE_PULLUP(PORTF.PIN0CTRL);

	// port R
	PORTR.OUT = 0;
	PORTR.DIR = 0;

	hw_configure_outputs();

	SLEEP.CTRL = SLEEP_SMODE_IDLE_gc | SLEEP_SEN_bm;

	// RTC, ticks at approx 1ms intervals
	// 1.024kHz from 32kHz RC oscillator enabled by ASF code (conf_clock.h)
	RTC.CTRL = 0;
	while (RTC.STATUS & RTC_SYNCBUSY_bm);
	RTC.INTCTRL = 0;
	RTC.CNT = 0;
	RTC.PER = 0xFFFF;
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;

	// DMA
	DMA.CTRL = DMA_RESET_bm;
	asm("nop");
	DMA.CTRL = DMA_ENABLE_bm | DMA_DBUFMODE_DISABLED_gc | DMA_PRIMODE_CH0RR123_gc;
}

/**************************************************************************************************
** Reset RTC to zero
*/
void HW_reset_rtc(void)
{
	RTC.CTRL = 0;
	while (RTC.STATUS & RTC_SYNCBUSY_bm);
	RTC.CNT = 0;
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
}

/**************************************************************************************************
** Write a CCP protected register. Registers protected by CCP require the CCP register to be written
** first, followed by writing the protected register within 4 instruction cycles.
**
** Interrupts are temporarily disabled during the write. Code mostly adapted/stolen from Atmel's
** clksys_driver.c and avr_compiler.h.
*/
void HW_CCPWrite(volatile uint8_t *address, uint8_t value)
{
	uint8_t	saved_sreg;

	// disable interrupts if running
	saved_sreg = SREG;
	cli();

	volatile uint8_t * tmpAddr = address;
	RAMPZ = 0;

	asm volatile(
	"movw r30,  %0"       "\n\t"
	"ldi  r16,  %2"       "\n\t"
	"out   %3, r16"       "\n\t"
	"st     Z,  %1"       "\n\t"
	:
	: "r" (tmpAddr), "r" (value), "M" (CCP_IOREG_gc), "i" (&CCP)
	: "r16", "r30", "r31"
	);

	SREG = saved_sreg;
}
