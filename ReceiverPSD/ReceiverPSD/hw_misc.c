/*
 * hw_misc.c
 *
 * Created: 27/03/2015 15:19:04
 *  Author: MoJo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "hw_misc.h"

/**************************************************************************************************
* Set up hardware
*/
void HW_init(void)
{
	SLEEP.CTRL	= SLEEP_SMODE_IDLE_gc | SLEEP_SEN_bm;

	// set 16MHz CPU clock
	OSC.CTRL |= OSC_XOSCEN_bm;											// enable external oscillator
	while(!(OSC.STATUS & OSC_XOSCRDY_bm));								// wait for oscillator to become ready
	HW_CCPWrite(&CLK.PSCTRL, CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc);		// 1:1:1 dividers, resulting in 16MHz
	HW_CCPWrite(&CLK.CTRL, CLK_SCLKSEL_XOSC_gc);						// switch to 16MHz clock
	OSC.CTRL = OSC_XOSCEN_bm;											// disable all other clock sources

	// PORT A, Saturn
	PORTA.OUT = 0;
	PORTA.DIR = PIN2_bm;
	
	// Port B, Dreamcast
	PORTB.OUT = 0;
	PORTB.DIR = 0;
	ENABLE_PULLUP(PORTB.PIN0CTRL);
	ENABLE_PULLUP(PORTB.PIN1CTRL);
	ENABLE_PULLUP(PORTB.PIN2CTRL);
	ENABLE_PULLUP(PORTB.PIN3CTRL);
	
	// Port C, I2C, Playstation 1/2
	PORTC.DIR = PIN0_bm | PIN1_bm | PSX_ACK_PIN_bm | PSX_DAT_PIN_bm;
	ENABLE_PULLUP(PORTC.PIN2CTRL);		// ATT
	ENABLE_PULLUP(PORTC.PIN4CTRL);		// ATT
	ENABLE_PULLUP(PORTC.PIN5CTRL);		// CMD
	ENABLE_PULLUP(PORTC.PIN7CTRL);		// CLK
	PSX_SPI.INTCTRL = 0;
	PSX_SPI.CTRL = SPI_ENABLE_bm | SPI_MODE_0_gc;
	
	// PORT D, LED, button, USB
	PORTD.OUT = 0;
	PORTD.DIR = LED_RED_PIN_bm | LED_GREEN_PIN_bm | PIN2_bm | LED_BLUE_PIN_bm | PIN4_bm;
	ENABLE_PULLUP(PORTD.PIN6CTRL);		// USB D-
	ENABLE_PULLUP(PORTD.PIN7CTRL);		// USB D+
	
	// PORT E, KBUS
	PORTE.OUT = 0;
	PORTE.DIR = PIN0_bm | PIN1_bm;
	
	// PORT R, xtal
	PORTR.OUT = 0;
	PORTR.DIR = 0;
	
	
	// DMA
	DMA.CTRL = DMA_RESET_bm;
	NOP();
	DMA.CTRL = DMA_ENABLE_bm | DMA_DBUFMODE_DISABLED_gc | DMA_PRIMODE_CH0123_gc;
}

/**************************************************************************************************
** Write a CCP protected register.
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

/**************************************************************************************************
* Calculate a CRC16 for given buffer
*/
uint16_t HW_crc16(const void *buffer, uint8_t length)
{
	CRC.CTRL = CRC_RESET1_bm;
	NOP();
	CRC.CTRL = CRC_SOURCE_IO_gc;
	
	while(length--)
		CRC.DATAIN = *(uint8_t *)buffer++;
	return ((CRC.CHECKSUM1 << 8) | CRC.CHECKSUM0);
}
