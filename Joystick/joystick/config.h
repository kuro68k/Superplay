/*
 * config.h
 *
 */


#ifndef CONFIG_H_
#define CONFIG_H_


// storage locations in EEPROM
#define EEP_MAPPING_CFG_PAGE	0		// 8 pages
#define EEP_MISC_CFG_PAGE		16		// 1 page


// logical inputs
enum LOGICAL_INPUTS_enum
{
	LNONE = 0,

	LJOY_UP,
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

	NUM_LOGICAL_INPUTS,					// must not be >127
	LFORCED = 127
};

#define LOGICAL_OUTPUT_MASK		0x80


// physical input map
#define MAPPING_CONFIG_ID		0x256A
#define NUM_MAPPINGS			124

// generic mapping config
typedef struct
{
	uint16_t	length;
	uint8_t		id;
	uint8_t		count;
	uint8_t		mapping[][2];
} MAPPING_CONFIG_t;




#define MISC_CONFIG_ID			0x97B2

#define AUX_MODE_DISABLED		0
#define AUX_MODE_KBUS			1
#define AUX_MODE_BUTTONS		2

// misc settings
typedef struct
{
	uint16_t	length;
	uint8_t		id;

	uint8_t		af_high_hz;
	uint8_t		af_high_duty_pc;
	uint8_t		af_low_hz;
	uint8_t		af_low_duty_pc;

	uint8_t		aux_mode;

	uint8_t		padding[20];

	uint32_t	crc32;
} SETTINGS_CONFIG_t;



extern const MAPPING_CONFIG_t * const map;
extern const MAPPING_CONFIG_t * const forced;
extern const MISC_CONFIG_t * const cfg;

extern void CFG_init(void);


#endif /* CONFIG_H_ */