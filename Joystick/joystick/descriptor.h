/*
 * kbus.h
 *
 */

// UDI_HID_REPORT_SIZE defined in conf_usb.h

// 55 bytes
UDC_DESC_STORAGE udi_hid_generic_report_desc_t udi_hid_generic_report_desc = {
	{
		0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		0x09, 0x04,                    // USAGE (Joystick)
		0xa1, 0x01,                    // COLLECTION (Application)

		0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)	joystick
		0x09, 0x30,                    //   USAGE (X)
		0x09, 0x31,                    //   USAGE (Y)
		0x15, 0x81,                    //   LOGICAL_MINIMUM (-127)
		0x25, 0x7f,                    //   LOGICAL_MAXIMUM (127)
		0x75, 0x08,                    //   REPORT_SIZE (8)
		0x95, 0x02,                    //   REPORT_COUNT (2)
		0x81, 0x02,                    //   INPUT (Data,Var,Abs)

		0x05, 0x09,                    //   USAGE_PAGE (Button)				buttons
		0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
		0x29, 0x18,                    //   USAGE_MAXIMUM (Button 24)
		0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
		0x75, 0x01,                    //   REPORT_SIZE (1)
		0x95, 0x18,                    //   REPORT_COUNT (24)
		0x81, 0x02,                    //   INPUT (Data,Var,Abs)

		0x06, 0x00, 0xff,              //   USAGE_PAGE (Vendor Defined Page 1)
		0x09, 0x01,                    //   USAGE (Vendor Usage 1)
		0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
		0x75, 0x08,                    //   REPORT_SIZE (8)
		0x95, 0x40,                    //   REPORT_COUNT (64)
		0xb1, 0x02,                    //   FEATURE (Data,Var,Abs)

		0xc0                           // END_COLLECTION
	}
};

// simple, POV, 60 bytes
/*
UDC_DESC_STORAGE udi_hid_generic_report_desc_t udi_hid_generic_report_desc = {
	{
		0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		0x09, 0x04,                    // USAGE (Joystick)
		0xa1, 0x01,                    // COLLECTION (Application)
		0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)	joystick
		0x09, 0x30,                    //   USAGE (X)
		0x09, 0x31,                    //   USAGE (Y)
		0x15, 0x81,                    //   LOGICAL_MINIMUM (-127)
		0x25, 0x7f,                    //   LOGICAL_MAXIMUM (127)
		0x75, 0x08,                    //   REPORT_SIZE (8)
		0x95, 0x02,                    //   REPORT_COUNT (2)
		0x81, 0x02,                    //   INPUT (Data,Var,Abs)
		0x05, 0x09,                    //   USAGE_PAGE (Button)				buttons
		0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
		0x29, 0x10,                    //   USAGE_MAXIMUM (Button 16)
		0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
		0x75, 0x01,                    //   REPORT_SIZE (1)
		0x95, 0x10,                    //   REPORT_COUNT (16)
		0x81, 0x02,                    //   INPUT (Data,Var,Abs)
		0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)	POV switch
		0x09, 0x39,                    //   USAGE (Hat switch)
		0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		0x25, 0x0c,                    //   LOGICAL_MAXIMUM (12)
		0x35, 0x00,                    //   PHYSICAL_MINIMUM (0)
		0x46, 0x68, 0x01,              //   PHYSICAL_MAXIMUM (360)
		0x65, 0x14,                    //   UNIT (Eng Rot:Angular Pos)
		0x75, 0x08,                    //   REPORT_SIZE (8)
		0x95, 0x01,                    //   REPORT_COUNT (1)
		0x81, 0x02,                    //   INPUT (Data,Var,Abs)
		0xc0                           // END_COLLECTION
	}
};
*/