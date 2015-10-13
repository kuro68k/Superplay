/*
 * config.h
 *
 * Created: 15/07/2015 16:32:00
 *  Author: Paul Qureshi
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_


// physical inputs
#define CONFIG_ID		0x2768

typedef struct
{
	uint16_t	config_id;				// must be CONFIG_ID
	uint16_t	config_size;

	uint8_t		joy_up;
	uint8_t		joy_down;
	uint8_t		joy_left;
	uint8_t		joy_right;
	
	uint8_t		start;
	uint8_t		coin;
	
	uint8_t		button1;
	uint8_t		button2;
	uint8_t		button3;
	uint8_t		button4;
	uint8_t		button5;
	uint8_t		button6;
	uint8_t		button7;
	uint8_t		button8;
	
	uint8_t		rotary1;
	uint8_t		rotary2;
	uint8_t		rotary3;
	uint8_t		rotary4;
	uint8_t		rotary5;
	uint8_t		rotary6;
	uint8_t		rotary7;
	uint8_t		rotary8;
	uint8_t		rotary9;
	uint8_t		rotary10;
	uint8_t		rotary11;
	uint8_t		rotary12;
	
	uint8_t		auto_low_1;
	uint8_t		auto_low_2;
	uint8_t		auto_low_3;
	uint8_t		auto_low_4;
	uint8_t		auto_low_5;
	uint8_t		auto_low_6;
	
	uint8_t		auto_high_1;
	uint8_t		auto_high_2;
	uint8_t		auto_high_3;
	uint8_t		auto_high_4;
	uint8_t		auto_high_5;
	uint8_t		auto_high_6;

	uint8_t		unused;
	uint8_t		mode_4;
	uint8_t		mode_4af;
	uint8_t		control;

	uint8_t		padding[16];	// round up to 64 bytes
	uint32_t	crc32;
} CONFIG_t;

// logical inputs
enum LOGICAL_BUTTONS_enum
{
	LJOY_UP = 0,
	LJOY_DN,
	LJOY_LF,
	LJOY_RT,
	
	LSTART,
	LCOIN,
	
	LBUTTON1,
	LBUTTON2,
	LBUTTON3,
	LBUTTON4,
	LBUTTON5,
	LBUTTON6,
	LBUTTON7,
	LBUTTON8,
	LBUTTON9,
	LBUTTON10,
	LBUTTON11,
	LBUTTON12,
	LBUTTON13,
	LBUTTON14,
	LBUTTON15,
	LBUTTON16,
	
	LROTARY1,
	LROTARY2,
	LROTARY3,
	LROTARY4,
	LROTARY5,
	LROTARY6,
	LROTARY7,
	LROTARY8,
	LROTARY9,
	LROTARY10,
	LROTARY11,
	LROTARY12,
	
	LAF_LOW_1,
	LAF_LOW_2,
	LAF_LOW_3,
	LAF_LOW_4,
	LAF_LOW_5,
	LAF_LOW_6,
	
	LAF_HIGH_1,
	LAF_HIGH_2,
	LAF_HIGH_3,
	LAF_HIGH_4,
	LAF_HIGH_5,
	LAF_HIGH_6,
	
	LUNUSED,
	LMODE_4,
	LMODE_4AF,
	
	LCONTROL,
	
	NUM_LOGICAL_INPUTS
};















typedef struct
{
	uint16_t	config_id;				// must be CONFIG_ID
	uint16_t	config_size;
	
	uint8_t		button_mode;			// CFG_BUTTON_MODE_*
	
	uint8_t		af_mode;				// CFG_AF_MODE_*
	uint8_t		af_high_05hz;
	uint8_t		af_low_05hz;
	uint16_t	af_mask;				// default autofire setting
	uint8_t		af_save_state;			// save state to EEPROM

	uint8_t		rotary_mode;			// CFG_ROTARY_MODE_*

	uint8_t		start_af_toggle_mode;	// hold START to toggle autofire

	uint8_t		padding[14];

	uint32_t	crc32;
} CONFIG_t_old;


#define CFG_BUTTON_MODE_SWITCHED		0
#define CFG_BUTTON_MODE_8				1
#define CFG_BUTTON_MODE_4				2
#define CFG_BUTTON_MODE_4AF				3
#define	CFG_BUTTON_MODE_16				4

#define CFG_AF_MODE_HIGH_LOW			0
#define CFG_AF_MODE_HIGH_WITH_LEDS		1
#define CFG_AF_MODE_FIXED				2
#define CFG_AF_MODE_TOGGLE_HIGH			3

#define	CFG_ROTARY_MODE_DISABLED		0
#define	CFG_ROTARY_MODE_BUTTONS			1
#define	CFG_ROTARY_MODE_POV				2
#define	CFG_ROTARY_MODF_ADDITIONAL_AF	3



extern const CONFIG_t * const cfg;

extern void CFG_init(void);


#endif /* CONFIG_H_ */