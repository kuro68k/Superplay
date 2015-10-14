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
	KBUS_PACKET_t response;
	KBUS_process_command((KBUS_PACKET_t *)report, &response);
	udi_hid_generic_send_report_in((uint8_t *)&response);
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
		if (logical_inputs[LJOY_UP])		hid_report.y = -127;
		if (logical_inputs[LJOY_DN])		hid_report.y = 127;
		if (logical_inputs[LJOY_RT])		hid_report.x = 127;
		if (logical_inputs[LJOY_LF])		hid_report.x = -127;

		if (logical_inputs[LBUTTON1])		hid_report.buttons1 |= (1<<0);
		if (logical_inputs[LBUTTON2])		hid_report.buttons1 |= (1<<1);
		if (logical_inputs[LBUTTON3])		hid_report.buttons1 |= (1<<2);
		if (logical_inputs[LBUTTON4])		hid_report.buttons1 |= (1<<3);
		if (logical_inputs[LBUTTON5])		hid_report.buttons1 |= (1<<4);
		if (logical_inputs[LBUTTON6])		hid_report.buttons1 |= (1<<5);
		if (logical_inputs[LBUTTON7])		hid_report.buttons1 |= (1<<6);
		if (logical_inputs[LBUTTON8])		hid_report.buttons1 |= (1<<7);
		if (logical_inputs[LBUTTON9])		hid_report.buttons2 |= (1<<0);
		if (logical_inputs[LBUTTON10])		hid_report.buttons2 |= (1<<1);
		if (logical_inputs[LBUTTON11])		hid_report.buttons2 |= (1<<2);
		if (logical_inputs[LBUTTON12])		hid_report.buttons2 |= (1<<3);
		if (logical_inputs[LBUTTON13])		hid_report.buttons2 |= (1<<4);
		if (logical_inputs[LBUTTON14])		hid_report.buttons2 |= (1<<5);
		if (logical_inputs[LBUTTON15])		hid_report.buttons2 |= (1<<6);
		if (logical_inputs[LBUTTON16])		hid_report.buttons2 |= (1<<7);
		
		if (logical_inputs[LSTART])			hid_report.buttons3 |= (1<<0);
		if (logical_inputs[LCOIN])			hid_report.buttons3 |= (1<<1);
		if (logical_inputs[LCONTROL])		hid_report.buttons3 |= (1<<2);
		if (logical_inputs[LUNUSED])		hid_report.buttons3 |= (1<<3);

		// rotary
		/*
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
		*/
	}
	
	udi_hid_generic_send_report_in((uint8_t *)&hid_report);
}
