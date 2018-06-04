/* descriptors.c
 *
 * Copyright 2018 Paul Qureshi
 *
 * USB descriptors and handlers
 */

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stddef.h>
#define HID_DECLARE_REPORT_DESCRIPTOR
#include "usb.h"
#include "usb_xmega.h"
#include "dfu.h"
#include "xmega.h"
#undef HID_DECLARE_REPORT_DESCRIPTOR

#ifdef USB_HID
USB_ENDPOINTS(1);
#else
USB_ENDPOINTS(2);
#endif


/**************************************************************************************************
* USB Device descriptor
*/
const __flash USB_DeviceDescriptor_t device_descriptor = {
	.bLength				= sizeof(USB_DeviceDescriptor_t),
	.bDescriptorType		= USB_DTYPE_Device,

	.bcdUSB                 = 0x0200,
#ifdef USB_HID
	.bDeviceClass           = USB_CSCP_NoDeviceClass,
#else
	.bDeviceClass           = USB_CSCP_VendorSpecificClass,
#endif
	.bDeviceSubClass        = USB_CSCP_NoDeviceSubclass,
	.bDeviceProtocol        = USB_CSCP_NoDeviceProtocol,

	.bMaxPacketSize0        = USB_EP0_MAX_PACKET_SIZE,
	.idVendor               = USB_VID,
	.idProduct              = USB_PID,
	.bcdDevice              = (USB_VERSION_MAJOR << 8) | (USB_VERSION_MINOR),

	.iManufacturer          = 0x01,
	.iProduct               = 0x02,
#ifdef USB_SERIAL_NUMBER
	.iSerialNumber          = 0x03,
#else
	.iSerialNumber          = 0x00,
#endif

	.bNumConfigurations     = 1
};


/**************************************************************************************************
* USB configuration descriptor
*/
typedef struct {
	USB_ConfigurationDescriptor_t	Config;
	USB_InterfaceDescriptor_t		Interface0;
#ifdef USB_HID
	USB_HIDDescriptor_t				HIDDescriptor;
	USB_EndpointDescriptor_t		HIDInEndpoint;
#else
	USB_EndpointDescriptor_t		DataInEndpoint;
	USB_EndpointDescriptor_t		DataOutEndpoint;
#endif
#ifdef USB_DFU_RUNTIME
	USB_InterfaceDescriptor_t		DFU_intf_runtime;
	DFU_FunctionalDescriptor_t		DFU_desc_runtime;
#endif
} ConfigDesc_t;

_Static_assert(sizeof(ConfigDesc_t) <= USB_EP0_BUFFER_SIZE, "Configuration descriptor exceeds EP0 buffer size");


/**************************************************************************************************
* USB configuration descriptor
*/
const __flash ConfigDesc_t configuration_descriptor = {
	.Config = {
		.bLength = sizeof(USB_ConfigurationDescriptor_t),
		.bDescriptorType = USB_DTYPE_Configuration,
		.wTotalLength  = sizeof(ConfigDesc_t),
#ifdef USB_DFU_RUNTIME
		.bNumInterfaces = 2,
#else
		.bNumInterfaces = 1,
#endif
		.bConfigurationValue = 1,
		.iConfiguration = 0,
		.bmAttributes = USB_CONFIG_ATTR_BUSPOWERED,
		.bMaxPower = USB_CONFIG_POWER_MA(100)
	},
#ifdef USB_HID
	.Interface0 = {
		.bLength = sizeof(USB_InterfaceDescriptor_t),
		.bDescriptorType = USB_DTYPE_Interface,
		.bInterfaceNumber = 0,
		.bAlternateSetting = 0,
		.bNumEndpoints = 1,
		.bInterfaceClass = USB_CSCP_HIDClass,
		.bInterfaceSubClass = USB_CSCP_HIDNoSubclass,
		.bInterfaceProtocol = USB_CSCP_HIDNoProtocol,
		.iInterface = 0
	},
	.HIDDescriptor = {
		.bLength = sizeof(USB_HIDDescriptor_t),
		.bDescriptorType = USB_DTYPE_HID,
		.bcdHID = 0x0111,
		.bCountryCode = 0,
		.bNumDescriptors = 1,
		.bReportDescriptorType = USB_DTYPE_Report,
		.wDescriptorLength = sizeof(hid_report_descriptor),
	},
	.HIDInEndpoint = {
		.bLength = sizeof(USB_EndpointDescriptor_t),
		.bDescriptorType = USB_DTYPE_Endpoint,
		.bEndpointAddress = 0x81,
		.bmAttributes = (USB_EP_TYPE_INTERRUPT),
		.wMaxPacketSize = 64,
		.bInterval = USB_HID_POLL_RATE_MS
	},
#else
	.Interface0 = {
		.bLength = sizeof(USB_InterfaceDescriptor_t),
		.bDescriptorType = USB_DTYPE_Interface,
		.bInterfaceNumber = 0,
		.bAlternateSetting = 0,
		.bNumEndpoints = 2,
		.bInterfaceClass = USB_CSCP_VendorSpecificClass,
		.bInterfaceSubClass = 0x00,
		.bInterfaceProtocol = 0x00,
		.iInterface = 0
	},
	.DataInEndpoint = {
		.bLength = sizeof(USB_EndpointDescriptor_t),
		.bDescriptorType = USB_DTYPE_Endpoint,
		.bEndpointAddress = 0x81,
		.bmAttributes = (USB_EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
		.wMaxPacketSize = 64,
		.bInterval = 0x00
	},
	.DataOutEndpoint = {
		.bLength = sizeof(USB_EndpointDescriptor_t),
		.bDescriptorType = USB_DTYPE_Endpoint,
		.bEndpointAddress = 0x2,
		.bmAttributes = (USB_EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
		.wMaxPacketSize = 64,
		.bInterval = 0x00
	},
#endif
#ifdef USB_DFU_RUNTIME
	.DFU_intf_runtime = {
		.bLength = sizeof(USB_InterfaceDescriptor_t),
		.bDescriptorType = USB_DTYPE_Interface,
		.bInterfaceNumber = 1,
		.bAlternateSetting = 0,
		.bNumEndpoints = 0,
		.bInterfaceClass = DFU_INTERFACE_CLASS,
		.bInterfaceSubClass = DFU_INTERFACE_SUBCLASS,
		.bInterfaceProtocol = DFU_INTERFACE_PROTOCOL_RUNTIME,
		.iInterface = 0x10
	},
	.DFU_desc_runtime = {
		.bLength = sizeof(DFU_FunctionalDescriptor_t),
		.bDescriptorType = DFU_DESCRIPTOR_TYPE,
		.bmAttributes = (DFU_ATTR_CANDOWNLOAD_bm | DFU_ATTR_WILLDETACH_bm),
		.wDetachTimeout = 0,
		.wTransferSize = APP_SECTION_PAGE_SIZE,
		.bcdDFUVersion = 0x0101
	},
#endif
};


/**************************************************************************************************
 *	USB strings
 */
#define	CONCAT(a, b)	a##b
#define	USTRING(s)		CONCAT(u, s)

const __flash USB_StringDescriptor_t language_string = {
	.bLength = USB_STRING_LEN(1),
	.bDescriptorType = USB_DTYPE_String,
	.bString = {USB_LANGUAGE_EN_US},
};

const __flash USB_StringDescriptor_t manufacturer_string = {
	.bLength = USB_STRING_LEN(USB_STRING_MANUFACTURER),
	.bDescriptorType = USB_DTYPE_String,
	.bString = USTRING(USB_STRING_MANUFACTURER)
};

const __flash USB_StringDescriptor_t product_string = {
	.bLength = USB_STRING_LEN(USB_STRING_PRODUCT),
	.bDescriptorType = USB_DTYPE_String,
	.bString = USTRING(USB_STRING_PRODUCT)
};

_Static_assert(sizeof(language_string) <= USB_EP0_BUFFER_SIZE, "Language string exceeds EP0 buffer size");
_Static_assert(sizeof(manufacturer_string) <= USB_EP0_BUFFER_SIZE, "Language string exceeds EP0 buffer size");
_Static_assert(sizeof(product_string) <= USB_EP0_BUFFER_SIZE, "Language string exceeds EP0 buffer size");

#ifdef USB_DFU_RUNTIME
const __flash USB_StringDescriptor_t dfu_runtime_string = {
	.bLength = USB_STRING_LEN("Runtime"),
	.bDescriptorType = USB_DTYPE_String,
	.bString = u"Runtime"
};
_Static_assert(sizeof(dfu_runtime_string) <= USB_EP0_BUFFER_SIZE, "DFU runtime string exceeds EP0 buffer size");
#endif // USB_DFU_RUNTIME


/**************************************************************************************************
 *	Optional serial number
 */
#ifdef USB_SERIAL_NUMBER
/*
USB_StringDescriptor serial_string = {
	.bLength = 22*2,
	.bDescriptorType = USB_DTYPE_String,
	.bString = u"0123456789ABCDEFGHIJKL"
};*/

void byte2char16(uint8_t byte, __CHAR16_TYPE__ *c)
{
	*c++ = (byte >> 4) < 10 ? (byte >> 4) + '0' : (byte >> 4) + 'A' - 10;
	*c = (byte & 0xF) < 10 ? (byte & 0xF) + '0' : (byte & 0xF) + 'A' - 10;

	// this version uses less flash memory
	//*c++ = 'A' + (byte >> 4);
	//*c = 'A' + (byte & 0xF);
}

void generate_serial(void)
{
	USB_StringDescriptor_t *serial_string = (USB_StringDescriptor_t *)ep0_buf_in;
	serial_string->bDescriptorType = USB_DTYPE_String;
	serial_string->bLength = 22*2;

	__CHAR16_TYPE__ *c = (__CHAR16_TYPE__ *)&serial_string->bString;
	uint8_t idx = offsetof(NVM_PROD_SIGNATURES_t, LOTNUM0);
	for (uint8_t i = 0; i < 6; i++)
	{
		byte2char16(NVM_read_production_signature_byte(idx++), c);
		c += 2;
	}
	byte2char16(NVM_read_production_signature_byte(offsetof(NVM_PROD_SIGNATURES_t, WAFNUM)), c);
	c += 2;
	idx = offsetof(NVM_PROD_SIGNATURES_t, COORDX0);
	for (uint8_t i = 0; i < 4; i++)
	{
		byte2char16(NVM_read_production_signature_byte(idx++), c);
		c += 2;
	}
}

_Static_assert((22*2) <= USB_EP0_BUFFER_SIZE, "Serial number string exceeds EP0 buffer size");
#endif


/**************************************************************************************************
 *	Optional Microsoft WCID stuff
 */
#ifdef USB_WCID
const __flash USB_StringDescriptor_t msft_string = {
	.bLength = 18,
	.bDescriptorType = USB_DTYPE_String,
	.bString = u"MSFT100" WCID_REQUEST_ID_STR
};
_Static_assert(sizeof(msft_string) <= USB_EP0_BUFFER_SIZE, "MSFT WCID string exceeds EP0 buffer size");

const __flash USB_MicrosoftCompatibleDescriptor_t msft_compatible = {
	.dwLength = sizeof(USB_MicrosoftCompatibleDescriptor_t) +
				1*sizeof(USB_MicrosoftCompatibleDescriptor_Interface_t),
	.bcdVersion = 0x0100,
	.wIndex = 0x0004,
	.bCount = 1,
	.reserved = {0, 0, 0, 0, 0, 0, 0},
	.interfaces = {
		{
#ifdef USB_HID
			.bFirstInterfaceNumber = 1,		// WCID only needed for the DFU interface
#else
			.bFirstInterfaceNumber = 0,		// WCID covers both interfaces
#endif
			.reserved1 = 0x01,
			.compatibleID = "WINUSB\0\0",
			.subCompatibleID = {0, 0, 0, 0, 0, 0, 0, 0},
			.reserved2 = {0, 0, 0, 0, 0, 0},
		},
	}
};
_Static_assert(sizeof(msft_compatible) <= USB_EP0_BUFFER_SIZE, "MSFT compatible descriptor exceeds EP0 buffer size");

#ifdef USB_WCID_EXTENDED
/*
#ifndef USB_DFU_RUNTIME
// example of one extended property (WinUSB GUID) for single interface (no DFU)
// (DeviceInterfaceGUID)
typedef struct {
	uint32_t dwLength;
	uint16_t bcdVersion;
	uint16_t wIndex;
	uint16_t wCount;
	uint32_t dwPropLength;
	uint32_t dwType;
	uint16_t wNameLength;
	wchar_t name[20];
	uint32_t dwDataLength;
	wchar_t data[39];
} __attribute__((packed)) USB_MicrosoftExtendedPropertiesDescriptor_t;

const __flash USB_MicrosoftExtendedPropertiesDescriptor_t msft_extended = {
	.dwLength = sizeof(USB_MicrosoftExtendedPropertiesDescriptor_t),
	.bcdVersion = 0x0100,
	.wIndex = 0x0005,
	.wCount = 1,
	.dwPropLength = 132,
	.dwType = 1,
	.wNameLength = 40,
	.name = L"DeviceInterfaceGUID\0",
	.dwDataLength = 78,
	.data = L"{42314231-5A81-49F0-BC3D-A4FF138216D7}\0",
};
#else
// example of one extended property (WinUSB GUID) for multiple interfaces (DFU runtime)
// (DeviceInterfaceGUIDs)
// See "important note 2" at https://github.com/pbatard/libwdi/wiki/WCID-Devices#Defining_a_Device_Interface_GUID_or_other_device_specific_properties
typedef struct {
	uint32_t dwLength;
	uint16_t bcdVersion;
	uint16_t wIndex;
	uint16_t wCount;
	uint32_t dwPropLength;
	uint32_t dwType;

	uint16_t wNameLength;
	wchar_t name[21];
	uint32_t dwDataLength;
	wchar_t data[40];
} __attribute__((packed)) USB_MicrosoftExtendedPropertiesDescriptor_t;

const __flash USB_MicrosoftExtendedPropertiesDescriptor_t msft_extended = {
	.dwLength = sizeof(USB_MicrosoftExtendedPropertiesDescriptor_t),
	.bcdVersion = 0x0100,
	.wIndex = 0x0005,
	.wCount = 1,
	.dwPropLength = 136,
	.dwType = 7,
	.wNameLength = 42,
	.name = L"DeviceInterfaceGUIDs\0",
	.dwDataLength = 80,
	.data = L"{42314231-5A81-49F0-BC3D-A4FF138216D7}\0\0",
};
#endif
*/

// example of two extended properties
typedef struct {
	uint32_t dwLength;
	uint16_t bcdVersion;
	uint16_t wIndex;
	uint16_t wCount;

	uint32_t dwPropLength;
	uint32_t dwType;
	uint16_t wNameLength;
	wchar_t name[21];
	uint32_t dwDataLength;
	wchar_t data[40];

	uint32_t dwPropLength2;
	uint32_t dwType2;
	uint16_t wNameLength2;
	wchar_t name2[6];
	uint32_t dwDataLength2;
	wchar_t data2[10];
} __attribute__((packed)) USB_MicrosoftExtendedPropertiesDescriptor_t;

const __flash USB_MicrosoftExtendedPropertiesDescriptor_t msft_extended = {
	.dwLength = sizeof(USB_MicrosoftExtendedPropertiesDescriptor_t),
	.bcdVersion = 0x0100,
	.wIndex = 0x0005,
	.wCount = 2,

	.dwPropLength = 136,
	.dwType = 1,
	.wNameLength = 42,
	.name = L"DeviceInterfaceGUIDs\0",
	.dwDataLength = 80,
	.data = L"{42314231-5A81-49F0-BC3D-A4FF138216D7}\0\0",

	.dwPropLength2 = 14 + (6*2) + (10*2),
	.dwType2 = 1,
	.wNameLength2 = 6*2,
	.name2 = L"Label\0",
	.dwDataLength2 = 10*2,
	.data2 = L"SUPERPLAY\0",
};

_Static_assert(sizeof(msft_extended) <= USB_EP0_BUFFER_SIZE, "MSFT extended descriptor exceeds EP0 buffer size");
#endif // USB_WCID_EXTENDED

void handle_msft_compatible(void)
{
	uint32_t address = 0;
	uint16_t size = 0;

	uint8_t cmd_backup = NVM.CMD;
	NVM.CMD = 0;

#ifdef USB_WCID_EXTENDED
	if (usb_setup.wIndex == 0x0005) {
		address = pgm_get_far_address(msft_extended);
		size    = pgm_read_dword_far(address + offsetof(USB_MicrosoftExtendedPropertiesDescriptor_t, dwLength));
	} else
#endif
	if (usb_setup.wIndex == 0x0004) {
		address = pgm_get_far_address(msft_compatible);
		size    = pgm_read_dword_far(address + offsetof(USB_MicrosoftCompatibleDescriptor_t, dwLength));
	} else {
		return usb_ep0_stall();
	}
	if (size > usb_setup.wLength) {
		size = usb_setup.wLength;
	}

	memcpy_PF(ep0_buf_in, address, size);
	NVM.CMD = cmd_backup;

	usb_ep0_in(size);
	usb_ep0_out();
}
#endif // USB_WCID


/**************************************************************************************************
 *	USB descriptor request handler
 */
uint16_t usb_handle_descriptor_request(uint8_t type, uint8_t index) {
	uint32_t address = 0;
	uint16_t size = 0;

	uint8_t cmd_backup = NVM.CMD;
	NVM.CMD = 0;

	switch (type)
	{
		case USB_DTYPE_Device:
			address = pgm_get_far_address(device_descriptor);
			size    = sizeof(USB_DeviceDescriptor_t);
			break;
		case USB_DTYPE_Configuration:
			address = pgm_get_far_address(configuration_descriptor);
			size    = sizeof(ConfigDesc_t);
			break;
#ifdef USB_HID
		case USB_DTYPE_HID:
			address = pgm_get_far_address(configuration_descriptor.HIDDescriptor);
			size	= sizeof(USB_HIDDescriptor_t);
			break;
		case USB_DTYPE_Report:
			address = pgm_get_far_address(hid_report_descriptor);
			size    = sizeof(hid_report_descriptor);
			break;
#endif
		case USB_DTYPE_String:
			switch (index)
			{
				case 0x00:
					address = pgm_get_far_address(language_string);
					break;
				case 0x01:
					address = pgm_get_far_address(manufacturer_string);
					break;
				case 0x02:
					address = pgm_get_far_address(product_string);
					break;
#ifdef USB_SERIAL_NUMBER
				case 0x03:
					generate_serial();
					return sizeof(USB_StringDescriptor_t) + (22*2);
#endif
#ifdef USB_DFU_RUNTIME
				case 0x10:
					address = pgm_get_far_address(dfu_runtime_string);
					break;
#endif
#ifdef USB_DFU_MODE
				case 0x10:
					address = pgm_get_far_address(dfu_flash_string);
					break;
				case 0x11:
					address = pgm_get_far_address(dfu_eeprom_string);
					break;
#endif
#ifdef USB_WCID
				case 0xEE:
					address = pgm_get_far_address(msft_string);
					break;
#endif

				default:
					return 0;
			}
			size = pgm_read_byte_far(address + offsetof(USB_StringDescriptor_t, bLength));
			break;
	}

	memcpy_PF(ep0_buf_in, address, size);
	NVM.CMD = cmd_backup;
	return size;
}


/**************************************************************************************************
 *	Set USB configuration
 */
bool usb_cb_set_configuration(uint8_t config) {
	if (config <= 1) {
		return true;
	} else {
		return false;
	}
}
