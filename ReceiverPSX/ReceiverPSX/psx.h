/*
 * psx.h
 *
 */


#ifndef PSX_H_
#define PSX_H_


#define PSX_PORT			PORTC
#define	PSX_ACK_PIN_bm		PIN3_bm
#define PSX_SS_PIN_bm		PIN4_bm		// attention
#define	PSX_CLK_PIN_nm		PIN5_bm	
#define PSX_MISO_PIN_bm		PIN6_bm		// data
#define	PSX_MOSI_PIN_bm		PIN7_bm		// command

#define	PSX_SPI				SPIC
#define	PSX_SPI_vect		SPIC_INT_vect

#define	PSX_TC				TCC4
#define PSX_SEND_ACK		PSX_TC.CTRLA = TC4_UPSTOP_bm | TC_TC4_CLKSEL_DIV1_gc


// digital controller bits
// byte 0
#define	PSX_BYTE0_SELECT_bm		(1<<0)
#define	PSX_BYTE0_START_bm		(1<<3)
#define	PSX_BYTE0_UP_bm			(1<<4)
#define	PSX_BYTE0_RIGHT_bm		(1<<5)
#define	PSX_BYTE0_DOWN_bm		(1<<6)
#define	PSX_BYTE0_LEFT_bm		(1<<7)
// byte 1
#define	PSX_BYTE1_L2_bm			(1<<0)
#define	PSX_BYTE1_R2_bm			(1<<1)
#define	PSX_BYTE1_L1_bm			(1<<2)
#define	PSX_BYTE1_R1_bm			(1<<3)
#define	PSX_BYTE1_TRIANGLE_bm	(1<<4)
#define	PSX_BYTE1_CIRCLE_bm		(1<<5)
#define	PSX_BYTE1_CROSS_bm		(1<<6)
#define	PSX_BYTE1_SQUARE_bm		(1<<7)



extern void PSX_init(void);
extern void PSX_update(void);


#endif /* PSX_H_ */