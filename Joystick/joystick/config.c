/*
 * config.c
 *
 * Load and save configuration data from EEPROM.
 */

#include <avr/io.h>
#include <string.h>
#include "global.h"
#include "eeprom.h"
#include "config.h"


const SETTINGS_CONFIG_t default_settings = {
	.length = sizeof(SETTINGS_CONFIG_t),
	.id = SETTINGS_CONFIG_ID
};
const SETTINGS_CONFIG_t *settings = &default_settings;

const MAPPING_CONFIG_t default_mapping = {
	.length = sizeof(default_mapping),
	.id = MAPPING_CONFIG_ID,
	.count = sizeof(default_mapping.mapping) / 2,
	.mapping = {	{	LJOY_UP,	PJOY_UP		},
					{	LJOY_DN,	PJOY_DN		},
					{	LJOY_LF,	PJOY_LF		},
					{	LJOY_RT,	PJOY_RT		},
					{	LMETA,		PMETA		},
					{	LBUTTON1,	PB1			},
					{	LBUTTON2,	PB2			},
					{	LBUTTON3,	PB3			},
					{	LBUTTON4,	PB4			},
					{	LBUTTON5,	PB5			},
					{	LBUTTON6,	PB6			},
					{	LBUTTON7,	PB7			},
					{	LBUTTON8,	PB8			},
					{	LBUTTON9,	PB9			},
					{	LBUTTON10,	PB10		},
					{	LBUTTON11,	PB11		},
					{	LBUTTON12,	PB12		},
					{	LBUTTON13,	PB13		},
					{	LBUTTON14,	PB14		},
					{	LBUTTON15,	PB15		},
					{	LBUTTON16,	PB16		},
					{	0,			0			}
				},
}
const MAPPING_CONFIG_t map = &default_mapping;

const MAPPING_CONFIG_t * const map = (MAPPING_CONFIG_t *)(EEP_MAPPED_ADDR(EEP_MAPPING_CFG_PAGE, 0));


/**************************************************************************************************
** Calculate CRC for an arbitrary buffer
*/
uint32_t cfg_calc_crc(const void *buffer, uint16_t size)
{
	CRC.CTRL = CRC_RESET_RESET1_gc;
	asm("nop");
	CRC.CTRL = CRC_CRC32_bm | CRC_SOURCE_IO_gc;
	uint8_t *ptr = (uint8_t *)buffer;
	while (size--)
		CRC.DATAIN = *ptr++;
	CRC.CTRL |= CRC_BUSY_bm;
	return CRC.CHECKSUM0 | ((uint32_t)CRC.CHECKSUM0 << 8) | ((uint32_t)CRC.CHECKSUM0 << 16) | ((uint32_t)CRC.CHECKSUM0 << 24);
}

/**************************************************************************************************
** Find config in EEPROM by ID number. Returns 0 if not found or bad CRC.
*/
void * CFG_find_config(uint8_t id)
{
	CONFIG_HEADER_t *header = (CONFIG_HEADER_t *)EEP_MAPPED_ADDR(0, 0);

	do
	{
		if ((header->length == 0xFFFF) || (header->length == 0x0000) ||
			(header->id == 0x00) || (header->id == 0xFF))
			break;

		if (header->id == id)
		{
			// check CRC
			uint32_t *crc = (uint16_t *)header + header->length;
			if (*crc != cfg_calc_crc(header, header->length))
				break;
			return header;
		}
	} while (header < (MAPPED_EEPROM_START + EEPROM_SIZE));
	return 0;
}

/**************************************************************************************************
** Prepare config for use
*/
void CFG_init(void)
{
	EEP_EnableMapping();

	void *ptr;
	if ((ptr = CFG_find_config(SETTINGS_CONFIG_ID)))
	{
		settings = ptr;
		if (settings->length != sizeof(SETTINGS_CONFIG_t))
			settings = default_settings;
	}

	if ((ptr = CFG_find_config(MAPPING_CONFIG_ID)))
		map = ptr;
}
