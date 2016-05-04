/*
 * hw_misc.c
 *
 * Created: 27/03/2015 15:19:04
 *  Author: MoJo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "kbus.h"
#include "hw_misc.h"

/**************************************************************************************************
* Set up hardware
*/
void HW_init(void)
{
	SLEEP.CTRL	= SLEEP_SMODE_IDLE_gc | SLEEP_SEN_bm;

	// set 16MHz CPU clock
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
	OSC.CTRL |= OSC_XOSCEN_bm;
	while(!(OSC.STATUS & OSC_XOSCRDY_bm));
	HW_CCPWrite(&CLK.PSCTRL, CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc);
	HW_CCPWrite(&CLK.CTRL, CLK_SCLKSEL_XOSC_gc);
	OSC.CTRL = OSC_XOSCEN_bm;		// disable other clocks

	// PORTA, unused
	PORTA.OUT = 0x00;
	PORTA.DIR = 0xFF;

	// PORT C, host
	PORTC.OUT = 0x00;
	PORTC.DIR = 0xFF;

	// PORT D, LED, KBUS
	PORTD.OUT = LED_PIN_bm | KBUS_TX_PIN_bm;
	PORTD.DIR = ~KBUS_RX_PIN_bm;
	ENABLE_PULLUP(PORTD.PIN6CTRL);		// KBUS RX
	ENABLE_PULLUP(PORTD.PIN7CTRL);		// KBUS TX
	
	// PORT R, xtal
	PORTR.OUT = 0;
	PORTR.DIR = 0;
	
	
	// DMA
	EDMA.CTRL = EDMA_RESET_bm;
	NOP();
	EDMA.CTRL = EDMA_ENABLE_bm | EDMA_CHMODE_PER0123_gc | EDMA_PRIMODE_CH0123_gc;
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
