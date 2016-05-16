/*
 * psx.h
 *
 * Created: 11/05/2016 14:32:30
 *  Author: paul.qureshi
 */ 


#ifndef PSX_H_
#define PSX_H_

#include "report.h"


#define PSX_SPI						SPIC
#define PSX_SPI_INT_vect			SPIC_INT_vect

#define PSX_TC						TCC4
#define PSX_SEND_ACK				PSX_TC.CTRLA |= TC45_CLKSEL_DIV1_gc;

// connections to PSX
#define PSX_PORT					PORTC
#define PSX_ACK_PIN_bm				PIN3_bm
#define PSX_ATT_PIN_bm				PIN4_bm
#define PSX_CMD_PIN_bm				PIN5_bm
#define PSX_DAT_PIN_bm				PIN6_bm
#define PSX_CLK_PIN_bm				PIN7_bm


// polling bits
// bytes 4
#define PSX_SELECT_bm				(1<<0)
#define PSX_START_bm				(1<<3)
#define PSX_UP_bm					(1<<4)
#define PSX_RIGHT_bm				(1<<5)
#define PSX_DOWN_bm					(1<<6)
#define PSX_LEFT_bm					(1<<7)
// byte 5
#define PSX_L2_bm					(1<<0)
#define PSX_R2_bm					(1<<1)
#define PSX_L1_bm					(1<<2)
#define PSX_R1_bm					(1<<3)
#define PSX_TRIANGLE_bm				(1<<4)
#define PSX_CIRCLE_bm				(1<<5)
#define PSX_CROSS_bm				(1<<6)
#define PSX_SQUARE_bm				(1<<7)


// PSX comms protocol
#define PSX_MODE_DIGITAL			0x40
#define PSX_MODE_ANALOGUE			0x70
#define PSX_MODE_CONFIG				0xF0

#define PSX_CMD_READ_POLL_STATE		0x41
#define PSX_CMD_POLL				0x42
#define PSX_CMD_CONFIG				0x43
#define PSX_CMD_CHANGE_MODE			0x44
#define PSX_CMD_STATUS				0x45
#define PSX_CMD_CONST1				0x46
#define PSX_CMD_CONST2				0x47
#define PSX_CMD_CONST3				0x4C
#define PSX_CMD_MAP_MOTORS			0x4D
#define PSX_CMD_ANA_CONFIG			0x4F


extern void PSX_update(REPORT_t *r);
extern void PSX_init(void);



#endif /* PSX_H_ */