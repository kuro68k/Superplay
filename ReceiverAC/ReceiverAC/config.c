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


const MAPPING_CONFIG_t default_mapping = {
	.length = sizeof(default_mapping),
	.id = MAPPING_CONFIG_ID,
	.count = (sizeof(default_mapping) - 4) / 2,
	.mapping = {	{	LJOY_UP,	LJOY_UP		},
					{	LJOY_DN,	LJOY_DN		},
					{	LJOY_LF,	LJOY_LF		},
					{	LJOY_RT,	LJOY_RT		},
					{	LBUTTON1,	LBUTTON1	},
					{	LBUTTON1,	LBUTTON2	},
					{	LBUTTON1,	LBUTTON3	},
					{	LBUTTON1,	LBUTTON4	},
					{	LBUTTON1,	LBUTTON5	},
					{	LBUTTON1,	LBUTTON6	},
					{	LBUTTON1,	LBUTTON7	},
					{	LBUTTON1,	LBUTTON8	},
					{	LBUTTON1,	LBUTTON9	},
					{	LBUTTON1,	LBUTTON10	},
					{	LBUTTON1,	LBUTTON11	},
					{	LBUTTON1,	LBUTTON12	},
					{	LBUTTON1,	LBUTTON13	},
					{	LBUTTON1,	LBUTTON14	},
					{	LBUTTON1,	LBUTTON15	},
					{	LBUTTON1,	LBUTTON16	},
					{	0,			0			}
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
