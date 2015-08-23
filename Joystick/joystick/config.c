/*
 * config.c
 *
 * Created: 16/08/2015 12:34:57
 *  Author: MoJo
 */ 

#include <avr/io.h>
#include <string.h>
#include "eeprom.h"
#include "config.h"

const CONFIG_t * const cfg = (CONFIG_t *)(EEP_MAPPED_ADDR(0, 0));

/**************************************************************************************************
** Load default config
*/
void cfg_load_default(void)
{
	EEP_DisableMapping();
	
	CONFIG_t new_cfg;
	memset(&new_cfg, 0, sizeof(new_cfg));
	
	new_cfg.config_id = CONFIG_ID;
	new_cfg.config_size = sizeof(CONFIG_t);
	
	new_cfg.button_mode = CFG_BUTTON_MODE_SWITCHED;
	
	//new_cfg.af_mode = CFG_AF_MODE_HIGH_LOW;
	new_cfg.af_mode = CFG_AF_MODE_HIGH_WITH_LEDS;
	new_cfg.af_high_05hz = (15*2);
	new_cfg.af_low_05hz = (5*2);
	new_cfg.af_mask = 0;
	new_cfg.af_save_state = 0;
	
	new_cfg.rotary_mode = CFG_ROTARY_MODE_DISABLED;
	
	new_cfg.start_af_toggle_mode = 0;
	
	// calculate CRC
	CRC.CTRL = CRC_RESET_RESET1_gc;
	asm("nop");
	CRC.CTRL = CRC_CRC32_bm | CRC_SOURCE_IO_gc;
	uint8_t *ptr = (uint8_t *)&new_cfg;
	for (uint8_t i = 0; i < sizeof(CONFIG_t); i++)
		CRC.DATAIN = *ptr++;
	CRC.CTRL |= CRC_BUSY_bm;
	new_cfg.crc32 = CRC.CHECKSUM0 | ((uint32_t)CRC.CHECKSUM0 << 8) | ((uint32_t)CRC.CHECKSUM0 << 16) | ((uint32_t)CRC.CHECKSUM0 << 24);

	// save to EEPROM
	EEP_LoadPageBuffer((uint8_t *)&new_cfg, sizeof(new_cfg));
	EEP_AtomicWritePage(0);
	EEP_WaitForNVM();
	
	EEP_EnableMapping();
}

/**************************************************************************************************
** Prepare config for use
*/
void CFG_init(void)
{
	EEP_EnableMapping();
	
	// calculate CRC
	CRC.CTRL = CRC_RESET_RESET1_gc;
	asm("nop");
	CRC.CTRL = CRC_CRC32_bm | CRC_SOURCE_IO_gc;
	uint8_t *ptr = (uint8_t *)cfg;
	for (uint8_t i = 0; i < sizeof(CONFIG_t); i++)
		CRC.DATAIN = *ptr++;
	CRC.CTRL |= CRC_BUSY_bm;
	uint32_t crc = CRC.CHECKSUM0 | ((uint32_t)CRC.CHECKSUM0 << 8) | ((uint32_t)CRC.CHECKSUM0 << 16) | ((uint32_t)CRC.CHECKSUM0 << 24);

	// validate config
	if ((cfg->config_id != CONFIG_ID) ||
		(cfg->config_size != sizeof(CONFIG_t)) ||
		(cfg->crc32 != crc))
		cfg_load_default();
}
