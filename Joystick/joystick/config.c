/*
 * config.c
 *
 * Created: 16/08/2015 12:34:57
 *  Author: Paul Qureshi
 */ 

#include <avr/io.h>
#include <string.h>
#include "global.h"
#include "eeprom.h"
#include "config.h"


ct_assert((sizeof(MAPPING_CONFIG_t) % 32) == 0);
ct_assert((sizeof(MISC_CONFIG_t) % 32) == 0);


const MAPPING_CONFIG_t * const map = (MAPPING_CONFIG_t *)(EEP_MAPPED_ADDR(0, 0));
const MISC_CONFIG_t * const cfg = (MISC_CONFIG_t *)(EEP_MAPPED_ADDR(16, 0));

/**************************************************************************************************
** Load default config
*/
void cfg_load_default_mapping(void)
{
	EEP_DisableMapping();
	
	MAPPING_CONFIG_t new_map;
	memset(&new_map, 0, sizeof(new_map));
	
	new_map.config_id = MAPPING_CONFIG_ID;
	new_map.config_size = sizeof(MAPPING_CONFIG_t);
	
	new_map.joy_up		= LJOY_UP;
	new_map.joy_down	= LJOY_DN;
	new_map.joy_left	= LJOY_LF;
	new_map.joy_right	= LJOY_RT;
	new_map.start		= LSTART;
	new_map.coin		= LCOIN;
	
	new_map.button1		= LBUTTON1;
	new_map.button2		= LBUTTON2;
	new_map.button3		= LBUTTON3;
	new_map.button4		= LBUTTON4;
	new_map.button5		= LBUTTON5;
	new_map.button6		= LBUTTON6;
	new_map.button7		= LBUTTON7;
	new_map.button8		= LBUTTON8;
	
	new_map.rotary1		= LROTARY1;
	new_map.rotary2		= LROTARY2;
	new_map.rotary3		= LROTARY3;
	new_map.rotary4		= LROTARY4;
	new_map.rotary5		= LROTARY5;
	new_map.rotary6		= LROTARY6;
	new_map.rotary7		= LROTARY7;
	new_map.rotary8		= LROTARY8;
	new_map.rotary9		= LROTARY9;
	new_map.rotary10	= LROTARY10;
	new_map.rotary11	= LROTARY11;
	new_map.rotary12	= LROTARY12;
	
	new_map.auto_low_1	= LAF_LOW_1;
	new_map.auto_low_2	= LAF_LOW_2;
	new_map.auto_low_3	= LAF_LOW_3;
	new_map.auto_low_4	= LAF_LOW_4;
	new_map.auto_low_5	= LAF_LOW_5;
	new_map.auto_low_6	= LAF_LOW_6;
	new_map.auto_high_1	= LAF_HIGH_1;
	new_map.auto_high_2	= LAF_HIGH_2;
	new_map.auto_high_3	= LAF_HIGH_3;
	new_map.auto_high_4	= LAF_HIGH_4;
	new_map.auto_high_5	= LAF_HIGH_5;
	new_map.auto_high_6	= LAF_HIGH_6;
	
	new_map.unused		= LUNUSED;
	new_map.mode_4		= LMODE_4;
	new_map.mode_4af	= LMODE_4AF;
	new_map.control		= LCONTROL;
	
	// calculate CRC
	CRC.CTRL = CRC_RESET_RESET1_gc;
	asm("nop");
	CRC.CTRL = CRC_CRC32_bm | CRC_SOURCE_IO_gc;
	uint8_t *ptr = (uint8_t *)&new_map;
	for (uint8_t i = 0; i < sizeof(MAPPING_CONFIG_t); i++)
		CRC.DATAIN = *ptr++;
	CRC.CTRL |= CRC_BUSY_bm;
	new_map.crc32 = CRC.CHECKSUM0 | ((uint32_t)CRC.CHECKSUM0 << 8) | ((uint32_t)CRC.CHECKSUM0 << 16) | ((uint32_t)CRC.CHECKSUM0 << 24);

	// save to EEPROM
	EEP_LoadPageBuffer((uint8_t *)&new_map, 32);
	EEP_AtomicWritePage(0);
	EEP_WaitForNVM();
	EEP_LoadPageBuffer((uint8_t *)&new_map+32, 32);
	EEP_AtomicWritePage(0);
	EEP_WaitForNVM();
	
	EEP_EnableMapping();
}

/**************************************************************************************************
** Do some basic checks on a configuration struct
*/
bool cfg_check_config(const void *config, uint16_t expected_id, uint16_t expected_size)
{
	uint8_t *ptr = (uint8_t *)config;
	uint16_t *id = (uint16_t *)ptr;
	uint16_t *size = (uint16_t *)ptr + 2;
	uint32_t *stored_crc = (uint32_t *)ptr + *size - 4;

	if ((*id != expected_id) ||
		(*size != expected_size))
		return false;

	// calculate CRC
	CRC.CTRL = CRC_RESET_RESET1_gc;
	asm("nop");
	CRC.CTRL = CRC_CRC32_bm | CRC_SOURCE_IO_gc;
	for (uint8_t i = 0; i < sizeof(MAPPING_CONFIG_t); i++)
		CRC.DATAIN = *ptr++;
	CRC.CTRL |= CRC_BUSY_bm;
	uint32_t crc = CRC.CHECKSUM0 | ((uint32_t)CRC.CHECKSUM0 << 8) | ((uint32_t)CRC.CHECKSUM0 << 16) | ((uint32_t)CRC.CHECKSUM0 << 24);

	if (*stored_crc != crc)
		return false;
	
	return true;
}

/**************************************************************************************************
** Prepare config for use
*/
void CFG_init(void)
{
	EEP_EnableMapping();

	if (!cfg_check_config(map, MAPPING_CONFIG_ID, sizeof(MAPPING_CONFIG_t)))
		cfg_load_default_mapping();
	
	if (!cfg_check_config(cfg, MISC_CONFIG_ID, sizeof(MISC_CONFIG_t)))
		cfg_load_default_mapping();

}
