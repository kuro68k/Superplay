/*
 * kbus.h
 *
 */


#ifndef KBUS_H_
#define KBUS_H_


/**************************************************************************************************
* Hardware
*/

// USART baud rate settings. 1Mbaud at 16MHz
#define BSEL					128
#define BSCALE					-7


#define KBUS_PORT				PORTD
#define KBUS_RX_PIN_bm			PIN2_bm
#define KBUS_TX_PIN_bm			PIN3_bm
#define KBUS_RX_PINCTRL			PIN2CTRL

#define KBUS_USART				USARTD0
#define KBUS_USART_RXC_vect		USARTD0_RXC_vect

#define KBUS_TC					TCC4
#define KBUS_TC_OVF_vect		TCC4_OVF_vect
#define KBUS_TC_CCA_vect		TCC4_CCA_vect

#define KBUS_DMA_CH				EDMA.CH0
#define KBUS_DMA_TRIGGER_SRC	EDMA_CH_TRIGSRC_USARTC0_RXC_gc

#define KBUS_EVENT_CTRL			EVSYS.CH0CTRL
#define KBUS_EVENT_STROBE		EVSYS.STROBE
#define KBUS_EVENT_MUX			EVSYS_CHMUX_PORTD_PIN6_gc
#define KBUS_EVSEL				TC45_EVSEL_CH0_gc


/**************************************************************************************************
* K-BUS packets
*/

typedef struct
{
	uint8_t		command;
	uint8_t		length;
	uint8_t		data[63+2];		// up to 63 data bytes, plus 2 checksum bytes
} KBUS_PACKET_t;


#define CMD_ECHO						0x50
#define CMD_READ_STRING					0x51
#define CMD_READ_VID_PID				0x52
#define CMD_READ_DESCRIPTOR				0x53
#define CMD_START_REPORTING				0x54
#define CMD_STOP_REPORTING				0x55
#define CMD_SET_AUTOFIRE_RATE			0x56
#define CMD_READ_AUTOFIRE_RATE			0x57
#define CMD_SET_BUTTON_AUTOFIRE			0x58
#define CMD_READ_BUTTON_AUTOFIRE		0x59
#define CMD_READ_REPORT					0x5A
#define CMD_READ_CONFIG					0x5B

#define ERROR_UNSPECIFIED_FAILURE		0x80
#define	ERROR_UNKNOWN_COMMAND			0x81

#define RESPONSE_BIT_bm					0x80


/**************************************************************************************************
* Publc functions and variables
*/

extern void KBUS_init(void);
extern void KBUS_run(void);



#endif /* KBUS_H_ */
