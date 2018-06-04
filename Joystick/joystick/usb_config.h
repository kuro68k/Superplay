/* usb_config.h
 *
 * Copyright 2018 Paul Qureshi
 *
 * USB stack configuration
 */

#ifndef USB_CONFIG_H_
#define USB_CONFIG_H_


/****************************************************************************************
* USB configuration
*/
// USB clock
#define USB_USE_PLL			// configure in usb_configure_clock() in usb_xmega.c
//#define USB_USE_RC32


// USB vendor and product IDs, version number
#define USB_VID				0x8282
#define USB_PID				0x6899

#define USB_VERSION_MAJOR	1
#define USB_VERSION_MINOR	0


// USB strings
#define USB_STRING_MANUFACTURER		"KEIO"
#define USB_STRING_PRODUCT			"SUPERPLAY"


// Generate a USB serial number from the MCU's unique identifiers. Can be
// disabled to save flash memory.
#define	USB_SERIAL_NUMBER


/****************************************************************************************
* Use Microsoft WCID descriptors
*/
#define	USB_WCID
#define USB_WCID_EXTENDED

#define WCID_REQUEST_ID			0x22
#define WCID_REQUEST_ID_STR		u"\x22"


/****************************************************************************************
* DFU (Device Firmware Update) run-time interface
*/
#define USB_DFU_RUNTIME

extern void	CCPWrite(volatile uint8_t *address, uint8_t value);
static inline void dfu_cb_enter_dfu_mode(void)
{
	*(uint32_t *)(INTERNAL_SRAM_START) = 0x4c4f4144;	// "LOAD"
	// watchdog reset gives USB time to send response
	asm("wdr");
	CCPWrite(&WDT.CTRL, WDT_WPER_128CLK_gc | WDT_ENABLE_bm | WDT_WCEN_bm);
}


/****************************************************************************************
* Enable HID, otherwise vendor specific bulk endpoints
*/
#define USB_HID
#define USB_HID_REPORT_SIZE		5
#define USB_HID_POLL_RATE_MS	1	// HID polling rate in milliseconds


// HID report descriptor
#if defined(USB_HID) && defined(HID_DECLARE_REPORT_DESCRIPTOR)
const __flash uint8_t hid_report_descriptor[] = {
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
};
_Static_assert(sizeof(hid_report_descriptor) <= USB_EP0_BUFFER_SIZE, "HID descriptor exceeds EP0 buffer size");
_Static_assert(USB_HID_REPORT_SIZE <= USB_EP0_BUFFER_SIZE, "HID report exceeds EP0 buffer size");
#endif	// defined(USB_HID) && defined(HID_DECLARE_REPORT_DESCRIPTOR)


// GET_REPORT handlers. *report is USB_MAX_PACKET_SIZE.
// Return number of bytes in report, or -1 if not supported
#include <hid.h>
static inline int16_t hid_cb_get_report_input(uint8_t *report, uint8_t report_id)
{
	memcpy(report, hid_report, sizeof(hid_report));
	return sizeof(hid_report);
}

static inline int16_t hid_cb_get_report_output(uint8_t *report, uint8_t report_id)
{
	return -1;
}

static inline int16_t hid_cb_get_report_feature(uint8_t *report, uint8_t report_id)
{
	return -1;
}

// SET_REPORT handlers
// Return true if report OK
static inline bool hid_cb_set_report_input(uint8_t *report, uint16_t report_length, uint8_t report_id)
{
	return false;
}

static inline bool hid_cb_set_report_output(uint8_t *report, uint16_t report_length, uint8_t report_id)
{
	return false;
}

static inline bool hid_cb_set_report_feature(uint8_t *report, uint16_t report_length, uint8_t report_id)
{
	return false;
}


#endif /* USB_CONFIG_H_ */
