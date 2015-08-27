#pragma once

#include <stdint.h>

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
