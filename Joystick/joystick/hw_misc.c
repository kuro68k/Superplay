/*
 * hw_misc.c
 *
 * Created: 18/12/2014 21:22:20
 *  Author: Paul Qureshi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "global.h"
#include "config.h"
#include "hw_misc.h"

FUSES = {
	0xFF,		// fusebyte 0
	0x00,		// fusebyte 1
	0xFE,		// fusebyte 2
	0xFF,		// fusebyte	3
	0xF3,		// fusebyte 4
	0xE0,		// fusebyte 5
};


uint8_t	HW_last_reset_status;


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
	HW_CCPWrite(&WDT_CTRL, WDT_PER_8KCLK_gc | WDT_ENABLE_bm | WDT_CEN_bm);

	// port A
	PORTA.OUT = 0;
	//if (cfg->af_mode == CFG_AF_MODE_HIGH_LOW)
	//	PORTA.DIR = 0;
	//else		
		PORTA.DIR = AUTO_LOW_5_PIN_bm | AUTO_LOW_6_PIN_bm;
	PORTCFG.MPCMASK = 0xFF;
	ENABLE_PULLUP(PORTA.PIN0CTRL);

	// port B
	PORTB.DIR = 0;
	PORTCFG.MPCMASK = 0xFF;
	ENABLE_PULLUP(PORTB.PIN0CTRL);

	// port C
	PORTC.OUT = 0;
	//if (cfg->af_mode == CFG_AF_MODE_HIGH_LOW)
	//	PORTC.DIR = PIN4_bm | PIN5_bm | DEBUG_TX_PIN_bm;
	//else
		PORTC.DIR = ~(DEBUG_RX_PIN_bm);
	PORTCFG.MPCMASK = 0xFF;
	ENABLE_PULLUP(PORTC.PIN0CTRL);

	// port D
	PORTD.DIR = USB_UI_TX_PIN_bm;
	PORTCFG.MPCMASK = 0xFF;
	ENABLE_PULLUP(PORTD.PIN0CTRL);

	// port E
	PORTE.DIR = 0;
	PORTCFG.MPCMASK = 0xFF;
	ENABLE_PULLUP(PORTE.PIN0CTRL);

	// port F
	PORTF.DIR = 0;
	PORTCFG.MPCMASK = 0xFF;
	ENABLE_PULLUP(PORTF.PIN0CTRL);

	// port R
	PORTR.DIR = 0;
	
	SLEEP.CTRL = SLEEP_SMODE_IDLE_gc | SLEEP_SEN_bm;
	
	// DMA
	DMA.CTRL = DMA_RESET_bm;
	asm("nop");
	DMA.CTRL = DMA_ENABLE_bm | DMA_DBUFMODE_DISABLED_gc | DMA_PRIMODE_CH0RR123_gc;
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
