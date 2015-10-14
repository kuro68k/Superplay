/*
 * config.h
 *
 * Created: 15/07/2015 16:32:00
 *  Author: Paul Qureshi
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_


// physical inputs
#define MAPPING_CONFIG_ID		0x256A

typedef struct
{
	uint16_t	config_id;				// must be MAPPING_CONFIG_ID
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

	uint8_t		padding[14];	// round up to 64 bytes
	uint32_t	crc32;
} MAPPING_CONFIG_t;

// logical inputs
enum LOGICAL_INPUTS_enum
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
	LAF_LOW_7,
	LAF_LOW_8,
	LAF_LOW_9,
	LAF_LOW_10,
	LAF_LOW_11,
	LAF_LOW_12,
	LAF_LOW_13,
	LAF_LOW_14,
	LAF_LOW_15,
	LAF_LOW_16,
	
	LAF_HIGH_1,
	LAF_HIGH_2,
	LAF_HIGH_3,
	LAF_HIGH_4,
	LAF_HIGH_5,
	LAF_HIGH_6,
	LAF_HIGH_7,
	LAF_HIGH_8,
	LAF_HIGH_9,
	LAF_HIGH_10,
	LAF_HIGH_11,
	LAF_HIGH_12,
	LAF_HIGH_13,
	LAF_HIGH_14,
	LAF_HIGH_15,
	LAF_HIGH_16,
	
	LUNUSED,
	LMODE_4,
	LMODE_4AF,
	
	LCONTROL,
	
	NUM_LOGICAL_INPUTS					// must not be >127
};

#define LOGICAL_OUTPUT_MASK				0x80




#define MISC_CONFIG_ID			0x97B2

typedef struct
{
	uint16_t	config_id;				// must be MISC_CONFIG_ID
	uint16_t	config_size;
	
	uint8_t		af_high_05hz;
	uint8_t		af_low_05hz;
	uint8_t		af_save_state;			// save state to EEPROM

	uint8_t		padding[21];

	uint32_t	crc32;
} MISC_CONFIG_t;



extern const MISC_CONFIG_t * const cfg;

extern void CFG_init(void);


#endif /* CONFIG_H_ */