/*
 * kbus.c
 *
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>

#include "global.h"
#include "hw_misc.h"
#include "eeprom.h"
#include "report.h"
#include "usb_config.h"
#include "kbus.h"


KBUS_PACKET_t report = { .command = KCMD_READ_REPORT | 0x80, .length = 16 };

/**************************************************************************************************
** Long report
*/
void KBUS_long_report(KBUS_PACKET_t *res)
{
	res->command = KCMD_READ_REPORT | 0x80;
	res->length = 16;
	RPT_generate_report((uint8_t *)res->data);
}

/**************************************************************************************************
** Process an incoming packet
*/
void KBUS_process_command(const KBUS_PACKET_t *cmd, KBUS_PACKET_t *res)
{
	res->command = cmd->command | 0x80;
	res->length = 0;
	memset(&res->data, 0, KBUS_PACKET_DATA_SIZE);

	if (cmd->length > KBUS_PACKET_DATA_SIZE)
		return;

	switch(cmd->command)
	{
		case KCMD_LOOPBACK:
			memcpy(&res->data, &cmd->data, cmd->length);
			res->length = cmd->length;
			break;

		case KCMD_READ_STRING:
			if (cmd->data[0] == KSTRING_DEVICE_NAME)
				strncpy_P((char *)&res->data, PSTR(USB_STRING_MANUFACTURER), KBUS_PACKET_DATA_SIZE);
			else if (cmd->data[0] == KSTRING_MANUFACTURER)
				strncpy_P((char *)&res->data, PSTR(USB_STRING_PRODUCT), KBUS_PACKET_DATA_SIZE);
			// TODO: fix serial numbers
			//else if (cmd->data[0] == KSTRING_SERIAL_NUMBER)
			//	strncpy((char *)&res->data, (char *)USB_serial_number, KBUS_PACKET_DATA_SIZE);
			// otherwise empty string
			res->length = strnlen((char *)&res->data, KBUS_PACKET_DATA_SIZE);
			break;

		case KCMD_READ_VID_PID:
			*(uint16_t *)&res->data[0] = USB_VID;
			*(uint16_t *)&res->data[2] = USB_PID;
			break;

		case KCMD_ENTER_BOOTLOADER:
			{
				/*
				uint32_t load = 0x4c4f4144;					// "LOAD"
				EEP_DisableMapping();
				EEP_WriteBuffer(&load, sizeof(load), 31);
				_delay_ms(100);								// ensure all writes have definitely finished
				*/
				memcpy_P(0, PSTR("LOAD"), 4);				// copy to .noinit section
				HW_CCPWrite(&RST.CTRL, RST_SWRST_bm);		// reset MCU into bootloader
				NOP();
				break;
			}

		case KCMD_READ_REPORT:
			KBUS_long_report(res);
			break;

		default:
			res->command = 0;	// unknown command response
			break;
	}
}
