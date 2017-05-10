/*
 * psx.c
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "version.h"
#include "config.h"
#include "report.h"
#include "psx.h"

/**************************************************************************************************
* Set up outputs and interrupts
*/
void PSX_init(void)
{
	GPIO0 = 0;
	GPIO1 = 0;
	
	PSX_SPI.CTRL = SPI_DORD_bm | SPI_MODE_3_gc;
	PSX_SPI.CTRLB = SPI_BUFMODE_BUFMODE1_gc;
	PSX_SPI.INTCTRL = SPI_RXCIE_bm | SPI_SSIE_bm | SPI_INTLVL_HI_gc;
	PSX_SPI.STATUS = 0xFF;
	PSX_SPI.CTRL |= SPI_ENABLE_bm;
	
	// acknowledge signal generated with timer
	PSX_TC.CTRLA = 0;
	PSX_TC.CTRLB = TC_TC4_WGMODE_NORMAL_gc;
	PSX_TC.CTRLC = 0;
	PSX_TC.CTRLD = 0;
	PSX_TC.CTRLE = TC_TC4_CCCMODE_COMP_gc | TC_TC4_CCDMODE_COMP_gc;;
	PSX_TC.INTCTRLA = 0;
	PSX_TC.INTCTRLB = 0;
	PSX_TC.CNT = 0;
	PSX_TC.PER = 0x40;
	PSX_TC.CCD = 0x3F;		// 4us @ div1
}

/**************************************************************************************************
* Update outputs
*/
void PSX_update(void)
{
	uint8_t m0 = 0;
	uint8_t m1 = 0;

	if (input_matrix[LJOY_UP])
		m0 |= PSX_BYTE0_UP_bm;
	if (input_matrix[LJOY_DN])
		m0 |= PSX_BYTE0_DOWN_bm;
	if (input_matrix[LJOY_LF])
		m0 |= PSX_BYTE0_LEFT_bm;
	if (input_matrix[LJOY_RT])
		m0 |= PSX_BYTE0_RIGHT_bm;
	if (input_matrix[LBUTTON15])
		m0 |= PSX_BYTE0_SELECT_bm;
	if (input_matrix[LBUTTON16])
		m0 |= PSX_BYTE0_START_bm;

	if (input_matrix[LBUTTON1])
		m0 |= PSX_BYTE1_SQUARE_bm;
	if (input_matrix[LBUTTON2])
		m0 |= PSX_BYTE1_TRIANGLE_bm;
	if (input_matrix[LBUTTON3])
		m0 |= PSX_BYTE1_L1_bm;

	if (input_matrix[LBUTTON4])
		m0 |= PSX_BYTE1_CROSS_bm;
	if (input_matrix[LBUTTON5])
		m0 |= PSX_BYTE1_CIRCLE_bm;
	if (input_matrix[LBUTTON6])
		m0 |= PSX_BYTE1_R1_bm;

	if (input_matrix[LBUTTON7])
		m0 |= PSX_BYTE1_L2_bm;
	if (input_matrix[LBUTTON8])
		m0 |= PSX_BYTE1_R2_bm;


	cli();
	GPIO0 = m0;
	GPIO1 = m1;
	sei();
}

/**************************************************************************************************
* SPI interrupt handler
*/
enum
{
	STATE_IDLE,
	STATE_READ_COMMAND,
	STATE_READ_STATUS_1,
	STATE_READ_STATUS_2,
};

ISR(PSX_SPI_vect)
{
	static uint8_t state = STATE_IDLE;
	
	if (PSX_SPI.STATUS & SPI_SSIF_bm)	// slave select
	{
		PSX_SPI.DATA = 0xFF;
		PSX_SPI.STATUS = SPI_SSIF_bm;
		state = STATE_IDLE;
	}
	
	if (PSX_SPI.STATUS & SPI_RXCIF_bm)
	{
		//PSX_SPI.STATUS = SPI_RXCIF_bm;
		uint8_t cmd = PSX_SPI.DATA;		// reading clears interrupt flag
		
		switch (state)
		{
			case STATE_IDLE:
				if (cmd == 0x01)
				{
					PSX_SEND_ACK;
					PSX_SPI.DATA = 0x41;	// digital, 1 word of data
					state = STATE_READ_COMMAND;
				}
				break;
			
			case STATE_READ_COMMAND:
				if (cmd == 0x42)		// read state
				{
					PSX_SEND_ACK;
					PSX_SPI.DATA = 0x5A;
					state = STATE_READ_STATUS_1;
				}
				break;
			
			case STATE_READ_STATUS_1:
				PSX_SEND_ACK;
				PSX_SPI.DATA = GPIO0;
				state = STATE_READ_STATUS_2;
				break;

			case STATE_READ_STATUS_2:
				PSX_SPI.DATA = GPIO1;
				state = STATE_IDLE;
				break;
		}
	}
}
