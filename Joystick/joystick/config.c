/*
 * config.c
 *
 * Created: 16/08/2015 12:34:57
 *  Author: Paul Qureshi
 *
 * Load and save configuration data from EEPROM.
 */ 

#include <avr/io.h>
#include <string.h>
#include "global.h"
#include "eeprom.h"
#include "config.h"


// check that configs are a multiple of an EEPROM page size
ct_assert((sizeof(MAPPING_CONFIG_t) % 32) == 0);
ct_assert((sizeof(MISC_CONFIG_t) % 32) == 0);


const MAPPING_CONFIG_t * const map = (MAPPING_CONFIG_t *)(EEP_MAPPED_ADDR(EEP_MAPPING_CFG_PAGE, 0));
const MISC_CONFIG_t * const cfg = (MISC_CONFIG_t *)(EEP_MAPPED_ADDR(EEP_MISC_CFG_PAGE, 0));

/**************************************************************************************************
** Calculate CRC for an arbitrary buffer
*/
uint32_t cfg_calc_crc(const void *buffer, uint16_t size)
{
	CRC.CTRL = CRC_RESET_RESET1_gc;
	asm("nop");
	CRC.CTRL = CRC_CRC32_bm | CRC_SOURCE_IO_gc;
	uint8_t *ptr = (uint8_t *)buffer;
	//for (uint16_t i = 0; i < sizeof(MAPPING_CONFIG_t); i++)
	while (size--)
		CRC.DATAIN = *ptr++;
	CRC.CTRL |= CRC_BUSY_bm;
	return CRC.CHECKSUM0 | ((uint32_t)CRC.CHECKSUM0 << 8) | ((uint32_t)CRC.CHECKSUM0 << 16) | ((uint32_t)CRC.CHECKSUM0 << 24);
}

/**************************************************************************************************
** Load default mapping config
*/
void cfg_load_default_mapping(void)
{
	EEP_DisableMapping();
	
	MAPPING_CONFIG_t new_map;
	memset(&new_map, 0, sizeof(new_map));
	
	new_map.config_id = MAPPING_CONFIG_ID;
	new_map.config_size = sizeof(MAPPING_CONFIG_t);

	for (uint8_t i = 1; i < NUM_LOGICAL_INPUTS; i++)
	{
		new_map.logical[i] = i;
		new_map.physical[i] = i;
	}
	new_map.logical[0] = LFORCED;
	
	new_map.crc32 = cfg_calc_crc(&new_map, sizeof(new_map) - 4);

	// save to EEPROM
	EEP_WriteBuffer(&new_map, sizeof(new_map), EEP_MAPPING_CFG_PAGE);
	EEP_EnableMapping();
}

/**************************************************************************************************
** Load default misc config
*/
void cfg_load_default_misc_config(void)
{
	EEP_DisableMapping();
	
	MISC_CONFIG_t new_cfg;
	memset(&new_cfg, 0, sizeof(new_cfg));
	
	new_cfg.config_id = MISC_CONFIG_ID;
	new_cfg.config_size = sizeof(MISC_CONFIG_t);
	
	new_cfg.af_high_05hz = 15 * 2;
	new_cfg.af_low_05hz = 2 * 2;
	
	new_cfg.crc32 = cfg_calc_crc(&new_cfg, sizeof(new_cfg) - 4);

	// save to EEPROM
	EEP_WriteBuffer(&new_cfg, sizeof(new_cfg), EEP_MISC_CFG_PAGE);
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
		(*size != expected_size) ||
		(*size > 512))
		return false;

	// calculate CRC
	CRC.CTRL = CRC_RESET_RESET1_gc;
	asm("nop");
	CRC.CTRL = CRC_CRC32_bm | CRC_SOURCE_IO_gc;
	for (uint16_t i = 0; i < sizeof(MAPPING_CONFIG_t); i++)
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
		cfg_load_default_misc_config();
}
