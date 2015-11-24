/*
 * usb.c
 *
 * Created: 23/11/2015 09:17:32
 *  Author: Paul Qureshi
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include "asf.h"
#include "usb.h"

/// Based on: https://github.com/cjameshuff/m1k-fw/blob/master/src/main.c

static USB_MicrosoftCompatibleDescriptor microsoft_compatible_id_descriptor = {
	.dwLength = sizeof(USB_MicrosoftCompatibleDescriptor) +
				1*sizeof(USB_MicrosoftCompatibleDescriptor_Interface),
	.bcdVersion = 0x0100,
	.wIndex = 0x0004,
	.bCount = 1,
	.reserved = {0, 0, 0, 0, 0, 0, 0},
	.interfaces = {
		{
			.bFirstInterfaceNumber = 1,
			.reserved1 = 1,
			.compatibleID = "WINUSB\0\0",
			.subCompatibleID = {0, 0, 0, 0, 0, 0, 0, 0},
			.reserved2 = {0, 0, 0, 0, 0, 0},
		}
	}
};


static USB_MicrosoftExtendedPropertiesDescriptor microsoft_extended_properties_descriptor = {
	.dwLength = sizeof(USB_MicrosoftExtendedPropertiesDescriptor),
	.bcdVersion = 0x0100,
	.wIndex = 0x0005,
	.bCount = 1,
	.dwPropertySize = 136,
	.dwPropertyDataType = 7,
	.wPropertyNameLength = 21*2,
	.PropertyName = L"DeviceInterfaceGUIDs",
	.dwPropertyDataLength = 40*2,
	.PropertyData = L"{42314231-4231-4231-4231-423142314231}\0",
};

/**************************************************************************************************
** WCID configuration information
** Hooked into UDC via UDC_GET_EXTRA_STRING #define.
*/
bool usb_msft_string(void)
{
	uint8_t udi_msft_magic[] = "MSFT100";
/*
	struct extra_strings_desc_t{
		usb_str_desc_t header;
		le16_t string[sizeof(udi_msft_magic)];
	};
*/
	struct extra_strings_desc_t{
		usb_str_desc_t header;
		le16_t	string[7];
		uint8_t	vendor_code;
		uint8_t	padding;
	};

	static UDC_DESC_STORAGE struct extra_strings_desc_t extra_strings_desc = {
		.header.bDescriptorType = USB_DT_STRING,
	};

	uint8_t i;
	uint8_t *str;
	uint8_t str_lgt=0;

	if ((udd_g_ctrlreq.req.wValue & 0xff) == 0xEE) {
		str_lgt = sizeof(udi_msft_magic)-1;
		str = udi_msft_magic;
	}
	else {
		return false;
	}

	if (str_lgt!=0) {
		for( i=0; i<str_lgt; i++) {
			extra_strings_desc.string[i] = cpu_to_le16((le16_t)str[i]);
		}
		extra_strings_desc.vendor_code = 0x22;
		extra_strings_desc.padding = 0;
		//extra_strings_desc.header.bLength = 2+ (str_lgt)*2;
		extra_strings_desc.header.bLength = 18;
		udd_g_ctrlreq.payload_size = extra_strings_desc.header.bLength;
		udd_g_ctrlreq.payload = (uint8_t *) &extra_strings_desc;
	}

	// if the string is larger than request length, then cut it
	if (udd_g_ctrlreq.payload_size > udd_g_ctrlreq.req.wLength) {
		udd_g_ctrlreq.payload_size = udd_g_ctrlreq.req.wLength;
	}
	return true;
}

/**************************************************************************************************
** Handle device requests that the ASF stack doesn't
*/
bool usb_other_requests(void)
{
	uint8_t* ptr = 0;
	uint16_t size = 0;
	
	if (Udd_setup_type() == USB_REQ_TYPE_VENDOR)
	{
		//if (udd_g_ctrlreq.req.bRequest == 0x30)
		if (1)
		{
			if (udd_g_ctrlreq.req.wIndex == 0x04)
			{
				ptr = (uint8_t*)&microsoft_compatible_id_descriptor;
				size = (udd_g_ctrlreq.req.wLength);
				if (size > microsoft_compatible_id_descriptor.dwLength)
					size = microsoft_compatible_id_descriptor.dwLength;
			}
			else if (udd_g_ctrlreq.req.wIndex == 0x05)
			{
				ptr = (uint8_t*)&microsoft_extended_properties_descriptor;
				size = (udd_g_ctrlreq.req.wLength);
				if (size > microsoft_extended_properties_descriptor.dwLength)
					size = microsoft_extended_properties_descriptor.dwLength;
			}
			else
				return false;
		}
	}
	
	udd_g_ctrlreq.payload_size = size;
	if ( size == 0 )
	{
		udd_g_ctrlreq.callback = 0;
		udd_g_ctrlreq.over_under_run = 0;
	}
	else
		udd_g_ctrlreq.payload = ptr;
	
	return true;
}
