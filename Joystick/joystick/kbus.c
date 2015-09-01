/*
 * kbus.c
 *
 * Created: 01/09/2015 13:43:26
 *  Author: Paul Qureshi
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <string.h>
#include <conf_usb.h>

#include "kbus.h"

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
				strncpy_P((char *)&res->data, PSTR(USB_DEVICE_PRODUCT_NAME), KBUS_PACKET_DATA_SIZE);
			else if (cmd->data[0] == KSTRING_MANUFACTURER)
				strncpy_P((char *)&res->data, PSTR(USB_DEVICE_MANUFACTURE_NAME), KBUS_PACKET_DATA_SIZE);
			else if (cmd->data[0] == KSTRING_SERIAL_NUMBER)
				strncpy((char *)&res->data, (char *)USB_serial_number, KBUS_PACKET_DATA_SIZE);
			// otherwise empty string
			res->length = strnlen((char *)&res->data, KBUS_PACKET_DATA_SIZE);
			break;
	
		case KCMD_READ_VID_PID:
			*(uint16_t *)&res->data[0] = USB_DEVICE_VENDOR_ID;
			*(uint16_t *)&res->data[2] = USB_DEVICE_PRODUCT_ID;
			break;
		
		default:
			res->command = 0;	// unknown command response
			break;
	}
}