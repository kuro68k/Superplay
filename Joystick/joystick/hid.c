/*
 * hid.c
 *
 * Created: 04/04/2015 16:10:24
 *  Author: Paul Qureshi
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include <asf.h>

#include "report.h"
#include "vendor.h"
#include "kbus.h"
#include "hid.h"

// from ASF, saves duplication
extern uint8_t udc_string_product_name[];
extern uint8_t udc_string_manufacturer_name[];

bool HID_enabled = false;

/**************************************************************************************************
** Set up HID interface
*/
void HID_init(void)
{
	HID_TC.CTRLA = 0;
	HID_TC.CTRLB = 0;
	HID_TC.CTRLC = 0;
	HID_TC.CNT = 0;
	HID_TC.PER = HID_TC_PER;
	HID_TC.INTCTRLA = 0;
	HID_TC.INTCTRLB = 0;
	HID_TC.INTFLAGS = TC0_OVFIF_bm;
}

/**************************************************************************************************
** Enable vendor interface, set default state
*/
bool HID_callback_enable(void)
{
	HID_enabled = true;
	return true;
}

/**************************************************************************************************
** Disable vendor interface
*/
bool HID_callback_disable(void)
{
	HID_enabled = false;
	return true;
}

/**************************************************************************************************
** Accept commands from host (KBUS)
*/
void HID_set_feature(uint8_t *report)
{
	uint8_t	response[UDI_HID_REPORT_OUT_SIZE];
	response[0] = report[0] | 0x80;	// response code = command code | 0x80
	memset(&response[1], 0, UDI_HID_REPORT_OUT_SIZE-1);
	
	switch(report[0])
	{
		case KCMD_LOOPBACK:
			memcpy(&response[1], &report[1], UDI_HID_REPORT_OUT_SIZE-1);
			break;

		case KCMD_READ_STRING:
			if (report[1] == KSTRING_DEVICE_NAME)
				strncpy_P((char *)&response[1], PSTR(USB_DEVICE_PRODUCT_NAME), UDI_HID_REPORT_OUT_SIZE-1);
			else if (report[1] == KSTRING_MANUFACTURER)
				strncpy_P((char *)&response[1], PSTR(USB_DEVICE_MANUFACTURE_NAME), UDI_HID_REPORT_OUT_SIZE-1);
			else if (report[1] == KSTRING_SERIAL_NUMBER)
				strncpy((char *)&response[1], (char *)USB_serial_number, UDI_HID_REPORT_OUT_SIZE-1);
			// otherwise empty string
			break;
	
		case KCMD_READ_VID_PID:
		{
			KCMD_VID_PID_OUTPUT_t *r = (KCMD_VID_PID_OUTPUT_t *)&response[1];
			r->vid = USB_DEVICE_VENDOR_ID;
			r->pid = USB_DEVICE_PRODUCT_ID;
			break;
		}
		
		default:
			response[0] = 0;
			break;
	}
	
	udi_hid_generic_send_report_in(response);
}

/**************************************************************************************************
** Send a latest report over HID
*/
void HID_send_report(void)
{
	static uint8_t last_rot = 0xFF;
	static bool rot_left = false;
	static bool rot_right = false;
	static bool inhibit = false;

	HID_REPORT_t	hid_report;
	
	if (!HID_enabled)
		return;
	
	memset(&hid_report, 0, sizeof(hid_report));
	
	if (!VEN_enabled)	// only send HID reports if not using high frequency interface
	{
		hid_report.x = 0;
		hid_report.y = 0;
		if (report.udlr_sscc & JOY_UP_bm)		hid_report.y = -127;
		if (report.udlr_sscc & JOY_DOWN_bm)		hid_report.y = 127;
		if (report.udlr_sscc & JOY_RIGHT_bm)	hid_report.x = 127;
		if (report.udlr_sscc & JOY_LEFT_bm)		hid_report.x = -127;

		hid_report.buttons = report.buttons;

		// rotary
		hid_report.rot = 11 - ((report.rot_mode & ROTARY_gm) >> ROTARY_gp);
		
		if (!rot_left && !rot_right && !inhibit)
		{
			if (last_rot == 0xFF)
				last_rot = (report.rot_mode & ROTARY_gm) >> ROTARY_gp;
			uint8_t new_rot = (report.rot_mode & ROTARY_gm) >> ROTARY_gp;

			// determine clockwise/anti-clockwise movement
			if ((last_rot != new_rot) && (new_rot != 0xF))
			{
				if ((last_rot > 9) && (new_rot < 3))
					rot_left = true;
				else if ((last_rot < 3) && (new_rot > 9))
					rot_right = true;
				else if (last_rot > new_rot)
					rot_right = true;
				else
					rot_left = true;

				last_rot = new_rot;
			
				// restart time
				HID_TC.CTRLA = 0;
				HID_TC.CNT = 0;
				HID_TC.INTFLAGS = TC0_OVFIF_bm;
				HID_TC.CTRLA = HID_TC_CLKSEL;
			}
		}
		
		// check for end of left/right button press
		if (HID_TC.INTFLAGS & TC0_OVFIF_bm)
		{
			if (inhibit)	// end of fake button press
			{
				HID_TC.CTRLA = 0;
				HID_TC.INTFLAGS = TC0_OVFIF_bm;
				inhibit = false;
			}
			else			// start inhibit phase
			{
				HID_TC.CTRLA = 0;
				HID_TC.CNT = 0;
				HID_TC.INTFLAGS = TC0_OVFIF_bm;
				HID_TC.CTRLA = HID_TC_CLKSEL;
				rot_left = false;
				rot_right = false;
				inhibit = true;
			}
		}
		
		if (rot_left)	hid_report.buttons |= HID_ROTATE_LEFT_BUTTON_bm;
		if (rot_right)	hid_report.buttons |= HID_ROTATE_RIGHT_BUTTON_bm;
	}
	
	udi_hid_generic_send_report_in((uint8_t *)&hid_report);
}
