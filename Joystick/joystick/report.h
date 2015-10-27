/*
 * report.h
 *
 * Created: 05/04/2015 23:19:49
 *  Author: Paul Qureshi
 */ 


#ifndef REPORT_H_
#define REPORT_H_

#include "config.h"


//	7			6			5			4			3			2			1			0
//	Control		Coin		Select		Start		Right		Left		Down		Up
//	B8			B7			B6			B5			B4			B3			B2			B1
//	B16			B15			B14			B13			B12			B11			B10			B9
//	-			-			[ Button Mode	]		[ Rotary Position						]


typedef struct
{
	uint8_t		udlr_sscc;
	uint16_t	buttons;
	uint8_t		rot_mode;
} REPORT_t;


// udlr_sscc
#define JOY_UP_bm				(1<<0)
#define JOY_DOWN_bm				(1<<1)
#define JOY_LEFT_bm				(1<<2)
#define JOY_RIGHT_bm			(1<<3)
#define BUTTON_START_bm			(1<<4)
#define BUTTON_SELECT_bm		(1<<5)
#define BUTTON_COIN_bm			(1<<6)
#define BUTTON_CONTROL_bm		(1<<7)


// rot_mode
#define ROTARY_NONE_gc				0xF
#define ROTARY_gp					0
#define ROTARY_gm					0x0F

#define BUTTON_MODE_8_gc			(0<<4)
#define BUTTON_MODE_4_gc			(1<<4)
#define BUTTON_MODE_4AF_gc			(2<<4)
#define BUTTON_MODE_16_gc			(3<<4)
#define	BUTTON_MODE_gp				4
#define BUTTON_MODE_gm				(0b11<<4)


extern REPORT_t	report;
extern uint8_t	logical_inputs[128];

extern void RPT_logical_inputs_refresh(void);
extern void RPT_refresh(void);


#endif /* REPORT_H_ */