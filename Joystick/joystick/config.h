/*
 * config.h
 *
 * Created: 15/07/2015 16:32:00
 *  Author: ???
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_


#define CONFIG_ID		0x2768

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
} CONFIG_t;


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