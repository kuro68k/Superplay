/*
 * config.c
 *
 * Load and save configuration data from EEPROM.
 */

#include <avr/io.h>
#include <string.h>
#include "config.h"


MAPPING_CONFIG_t default_mapping = {
	.id = ATARI_CONFIG_ID,
	.mapping = {	{	LJOY_UP,	128 + LJOY_UP	},
					{	LJOY_DN,	128 + LJOY_DN	},
					{	LJOY_LF,	128 + LJOY_LF	},
					{	LJOY_RT,	128 + LJOY_RT	},
					{	LBUTTON1,	128 + LBUTTON1	},
					{	LBUTTON2,	128 + LBUTTON2	},
					{	LBUTTON3,	128 + LBUTTON3	},
					{	LBUTTON4,	128 + LBUTTON4	},
					{	LBUTTON5,	128 + LBUTTON5	},
					{	LBUTTON6,	128 + LBUTTON6	},
					{	LBUTTON7,	128 + LBUTTON7	},
					{	LBUTTON8,	128 + LBUTTON8	},
					{	LBUTTON9,	128 + LBUTTON9	},
					{	LBUTTON10,	128 + LBUTTON10	},
					{	LBUTTON11,	128 + LBUTTON11	},
					{	LBUTTON12,	128 + LBUTTON12	},
					{	LBUTTON13,	128 + LBUTTON13	},
					{	LBUTTON14,	128 + LBUTTON14	},
					{	LBUTTON15,	128 + LBUTTON15	},
					{	LBUTTON16,	128 + LBUTTON16	}
				},
};
const MAPPING_CONFIG_t *map = &default_mapping;


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
** Prepare config for use
*/
void CFG_init(void)
{
	// size of flexible arrays can't be computed inside initializer
	default_mapping.length = __builtin_object_size(&default_mapping, 0);
	default_mapping.count = (default_mapping.length - 4) / 2;
}

/**************************************************************************************************
** Reset to default mapping
*/
void CFG_load_default_mapping(void)
{
	map = &default_mapping;
}
