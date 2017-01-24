/*
 * kbus.c
 *
 * Created: 01/09/2015 13:43:26
 *  Author: Paul Qureshi
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include <conf_usb.h>

#include "global.h"
#include "hw_misc.h"
#include "eeprom.h"
#include "report.h"
#include "kbus.h"

typedef void (*AppPtr)(void) __attribute__ ((noreturn));

/**************************************************************************************************
** Long report
*/
void kbus_long_report(KBUS_PACKET_t *res)
{
	res->length = 16;
/*
	uint8_t b = 0;
	if (input_matrix[0])	b |= (1<<0);
	if (input_matrix[1])	b |= (1<<1);
	if (input_matrix[2])	b |= (1<<2);
	if (input_matrix[3])	b |= (1<<3);
	if (input_matrix[4])	b |= (1<<4);
	if (input_matrix[5])	b |= (1<<5);
	if (input_matrix[6])	b |= (1<<6);
	if (input_matrix[7])	b |= (1<<7);
	res->data[0] = b;
*/
	asm volatile(
		"ldi	r18, 16"			"\n\t"
		"loop%=:"
		"ld		__tmp_reg__, X+"	"\n\t"	// 0
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 0"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 1
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 1"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 2
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 2"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 3
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 3"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 4
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 4"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 5
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 5"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 6
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 6"				"\n\t"
		"ld		__tmp_reg__, X+"	"\n\t"	// 7
		"bst	__tmp_reg__, 0"		"\n\t"
		"bld	r19, 7"				"\n\t"
		"st		Z+, r19"			"\n\t"
		"dec	r18"				"\n\t"
		"brne	loop%="
	:
	: [input] "x" (input_matrix), [output] "z" (&res->data)
	: "r18", "r19"
	);
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

		case KCMD_ENTER_BOOTLOADER:
			{
				uint32_t load = 0x4c4f4144;					// "LOAD"
				EEP_DisableMapping();
				EEP_WriteBuffer(&load, sizeof(load), 31);
				_delay_ms(100);								// ensure all writes have definitely finished
				HW_CCPWrite(&RST.CTRL, RST_SWRST_bm);		// reset MCU into bootloader
				NOP();
				break;
			}

		case KCMD_READ_REPORT:
			kbus_long_report(res);
			break;

		default:
			res->command = 0;	// unknown command response
			break;
	}
}
