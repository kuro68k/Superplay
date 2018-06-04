/*
 * config.h
 *
 */


#ifndef CONFIG_H_
#define CONFIG_H_


// logical inputs
enum LOGICAL_INPUTS_enum
{
	LOFF = 0x00,
	LON,

	LJOY_UP = 0x02,
	LJOY_DN,
	LJOY_LF,
	LJOY_RT,
	LMETA = 0x0F,

	LBUTTON1 = 0x10,
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

	LAF_LOW_1 = 0x20,
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

	LAF_HIGH_1 = 0x30,
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

	LROTARY1 = 0x40,
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
	LROTARY13,
	LROTARY14,
	LROTARY15,
	LROTARY16,

	LLED1 = 0x70,
	LLED2,
	LLED3,
	LLED4,
	LLED5,
	LLED6,
	LLED7,
	LLED8,
	LLED9,
	LLED10,
	LLED11,
	LLED12,
	LLED13,
	LLED14,
	LLED15,
	LLED16,
};

enum PHYSICAL_INPUTS_enum
{
	POFF = 0x80,
	PON,

	PJOY_UP = 0x82,
	PJOY_DN,
	PJOY_LF,
	PJOY_RT,
	PLED = 0x8E,
	PMETA = 0x8F,

	PB1 = 0x90,
	PB2,
	PB3,
	PB4,
	PB5,
	PB6,
	PB7,
	PB8,
	PB9,
	PB10,
	PB11,
	PB12,
	PB13,
	PB14,
	PB15,
	PB16,

	PA1 = 0xA0,
	PA2,
	PA3,
	PA4,
	PA5,
	PA6,
	PA7,
	PA8,
	PA9,
	PA10,
	PA11,
	PA12,
	PA13,
	PA14,
	PA15,
	PA16,

	PR1 = 0xB0,
	PR2,
	PR3,
	PR4,
	PR5,
	PR6,
	PR7,
	PR8,
	PR9,
	PR10,
	PR11,
	PR12,
	PR13,
	PR14,
	PR15,
	PR16,
};


// header block for all configs
#define CONFIG_SIGNATURE			0xCF951968
typedef struct
{
	uint32_t	signature;
	uint16_t	length;
	uint8_t		id;
} CONFIG_HEADER_t;


// generic mapping config
typedef struct
{
	uint16_t	length;
	uint8_t		id;
	uint8_t		count;
	uint8_t		mapping[][2];
} MAPPING_CONFIG_t;


#define SETTINGS_CONFIG_ID		1
#define MAPPING_CONFIG_ID		4


// misc settings
typedef struct
{
	uint16_t	length;
	uint8_t		id;

	uint8_t		af_high_hz;
	uint8_t		af_high_duty_pc;
	uint8_t		af_low_hz;
	uint8_t		af_low_duty_pc;
	uint8_t		af_remember_state;

	uint8_t		leds[16];
	uint16_t	led_output_inversion_map;
	uint8_t		led_display_mapping;
	uint16_t	led_display_timeout_ms;

	uint8_t		meta_af_toggle;
	uint8_t		meta_af_select_with_stick;
	uint8_t		meta_mapping_select_with_stick;

	uint8_t		rotary_num_positions;
	uint8_t		rotary_enable_pov;
	uint8_t		rotary_enable_buttons;
} SETTINGS_CONFIG_t;



extern const MAPPING_CONFIG_t * map;
extern const SETTINGS_CONFIG_t * settings;

extern void CFG_init(void);


#endif /* CONFIG_H_ */