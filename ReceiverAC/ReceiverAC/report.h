/*
 * report.h
 *
 * Created: 02/03/2015 15:37:23
 *  Author: kuro68k
 */ 


#ifndef REPORT_H_
#define REPORT_H_


typedef struct
{
	uint8_t	udlr_sscc;
	uint8_t	buttons1_8;
	uint8_t	buttons9_16;
	uint8_t	rot_mode;
} REPORT_t;

// UDLR Start Select Coin Control
#define STICK_UP_bm					(1<<0)
#define STICK_DOWN_bm				(1<<1)
#define STICK_LEFT_bm				(1<<2)
#define STICK_RIGHT_bm				(1<<3)
#define BUTTON_START_bm				(1<<4)
#define BUTTON_SELECT_bm			(1<<5)
#define BUTTON_COIN_bm				(1<<6)
#define BUTTON_CONTROL_bm			(1<<7)

// rotary position, button mode
#define ROTARY_NONE_gc				0xF
#define ROTARY_gp					0
#define ROTARY_gm					0x0F

#define BUTTON_MODE_8_gc			(0<<4)
#define BUTTON_MODE_4_gc			(1<<4)
#define BUTTON_MODE_4AF_gc			(2<<4)
#define BUTTON_MODE_UNDEFINED_gc	(3<<4)
#define	BUTTON_MODE_gp				4
#define BUTTON_MODE_gm				(0b11<<4)


typedef struct 
{
	uint8_t		command;
	REPORT_t	report;
} USB_REPORT_t;

typedef struct 
{
	uint8_t		command;
	REPORT_t	report;
	uint16_t	checksum;
} KBUS_REPORT_t;



#endif /* REPORT_H_ */