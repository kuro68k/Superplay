/*
 * hid.c
 *
 */

#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include <asf.h>
#include "usb_device.h"

#include "report.h"
#include "hw_misc.h"
#include "kbus.h"
#include "hid.h"

bool HID_udc_resumed = false;	// set by ASF USB stack via callback (see conf_usb.h)

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
** Look for USB bus
*/
bool HID_check_for_bus(void)
{
	HW_reset_rtc();

	udc_start();
	udc_attach();

	do
	{
		if (HID_udc_resumed)
			return true;
	} while (RTC.CNT < 100);

	udc_detach();
	udc_stop();
	return false;
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
** Find rotary position. Returns -1 if none, or 0-11
*/
inline int8_t hid_find_rotary_pos(void)
{
	if (settings->rotary_num_positions > 0)
	{
		for (uint8_t i = 0; i < settings->rotary_num_positions; i++)
			if (input_matrix[LROTARY1 + i]) return i;
	}
	return -1;
}

/**************************************************************************************************
** Rotary buttons
*/
void hid_rotary_buttons(HID_REPORT_t *hid_report)
{
	static uint8_t last_rot = 0xFF;
	static bool rot_left = false;
	static bool rot_right = false;
	static bool inhibit = false;

	if (settings->rotary_enable_buttons)
	{
		if (!rot_left && !rot_right && !inhibit)
		{
			uint8_t new_rot = hid_find_rotary_pos();
			if (new_rot != -1)
			{
				if (last_rot == 0xFF)
					last_rot = new_rot;

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

		if (rot_left)	hid_report->buttons3 |= HID_ROTATE_LEFT_BUTTON_bm;
		if (rot_right)	hid_report->buttons3 |= HID_ROTATE_RIGHT_BUTTON_bm;
	}
}

/**************************************************************************************************
** Send a latest report over HID
*/
void HID_send_report(void)
{
	HID_REPORT_t	hid_report;

	memset(&hid_report, 0, sizeof(hid_report));

	hid_report.x = 0;
	hid_report.y = 0;
	if (input_matrix[LJOY_UP])		hid_report.y = -127;
	if (input_matrix[LJOY_DN])		hid_report.y = 127;
	if (input_matrix[LJOY_RT])		hid_report.x = 127;
	if (input_matrix[LJOY_LF])		hid_report.x = -127;

	if (input_matrix[LBUTTON1])		hid_report.buttons1 |= (1<<0);
	if (input_matrix[LBUTTON2])		hid_report.buttons1 |= (1<<1);
	if (input_matrix[LBUTTON3])		hid_report.buttons1 |= (1<<2);
	if (input_matrix[LBUTTON4])		hid_report.buttons1 |= (1<<3);
	if (input_matrix[LBUTTON5])		hid_report.buttons1 |= (1<<4);
	if (input_matrix[LBUTTON6])		hid_report.buttons1 |= (1<<5);
	if (input_matrix[LBUTTON7])		hid_report.buttons1 |= (1<<6);
	if (input_matrix[LBUTTON8])		hid_report.buttons1 |= (1<<7);
	if (input_matrix[LBUTTON9])		hid_report.buttons2 |= (1<<0);
	if (input_matrix[LBUTTON10])	hid_report.buttons2 |= (1<<1);
	if (input_matrix[LBUTTON11])	hid_report.buttons2 |= (1<<2);
	if (input_matrix[LBUTTON12])	hid_report.buttons2 |= (1<<3);
	if (input_matrix[LBUTTON13])	hid_report.buttons2 |= (1<<4);
	if (input_matrix[LBUTTON14])	hid_report.buttons2 |= (1<<5);
	if (input_matrix[LBUTTON15])	hid_report.buttons2 |= (1<<6);
	if (input_matrix[LBUTTON16])	hid_report.buttons2 |= (1<<7);

	//if (input_matrix[LSTART])			hid_report.buttons3 |= (1<<0);
	//if (input_matrix[LCOIN])			hid_report.buttons3 |= (1<<1);
	//if (input_matrix[LCONTROL])		hid_report.buttons3 |= (1<<2);
	//if (input_matrix[LUNUSED])		hid_report.buttons3 |= (1<<3);
	if (input_matrix[LMETA])		hid_report.buttons3 |= (1<<7);

	hid_rotary_buttons(&hid_report);

	udi_hid_generic_send_report_in((uint8_t *)&hid_report);
}
