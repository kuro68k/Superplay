/**
 * \file
 *
 * \brief USB configuration file
 *
 * Copyright (c) 2009-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef _CONF_USB_H_
#define _CONF_USB_H_

#include "compiler.h"

/**
 * USB Device Configuration
 * @{
 */

//! Device definition (mandatory)
#define  USB_DEVICE_VENDOR_ID             0x8282
#define  USB_DEVICE_PRODUCT_ID            0x6899
#define  USB_DEVICE_MAJOR_VERSION         1
#define  USB_DEVICE_MINOR_VERSION         0
#define  USB_DEVICE_POWER                 100 // Consumption on VBUS line (mA)
#define  USB_DEVICE_ATTR                  (USB_CONFIG_ATTR_BUS_POWERED)
//		(USB_CONFIG_ATTR_SELF_POWERED)
// (USB_CONFIG_ATTR_BUS_POWERED)
// (USB_CONFIG_ATTR_REMOTE_WAKEUP|USB_CONFIG_ATTR_SELF_POWERED)
// (USB_CONFIG_ATTR_REMOTE_WAKEUP|USB_CONFIG_ATTR_BUS_POWERED)

//! USB Device string definitions (Optional)
#define  USB_DEVICE_MANUFACTURE_NAME      "KEIO"
#define  USB_DEVICE_PRODUCT_NAME          "SUPERPLAY Joystick"
// #define  USB_DEVICE_SERIAL_NAME           "12...EF" // Disk SN for MSC

/**
 * Device speeds support
 * @{
 */
//! To define a Low speed device
//#define  USB_DEVICE_LOW_SPEED

//! To authorize the High speed
#if (UC3A3||UC3A4)
//#define  USB_DEVICE_HS_SUPPORT
#endif
//@}

/**
 * USB Device Callbacks definitions (Optional)
 * @{
 */
// #define  UDC_VBUS_EVENT(b_vbus_high)      user_callback_vbus_action(b_vbus_high)
// #define  UDC_SOF_EVENT()                  user_callback_sof_action()
// #define  UDC_SUSPEND_EVENT()              user_callback_suspend_action()
// #define  UDC_RESUME_EVENT()               user_callback_resume_action()
//! Mandatory when USB_DEVICE_ATTR authorizes remote wakeup feature
// #define  UDC_REMOTEWAKEUP_ENABLE()        user_callback_remotewakeup_enable()
// #define  UDC_REMOTEWAKEUP_DISABLE()       user_callback_remotewakeup_disable()
//! When a extra string descriptor must be supported
//! other than manufacturer, product and serial string
// #define  UDC_GET_EXTRA_STRING()
//@}

/**
 * USB Device low level configuration
 * When only one interface is used, these configurations are defined by the class module.
 * For composite device, these configuration must be defined here
 * @{
 */
//! Control endpoint size
#define  USB_DEVICE_EP_CTRL_SIZE       64

//! Number of interfaces for this device
#define  USB_DEVICE_NB_INTERFACE       2 // 1 or more

//! Total endpoint used by all interfaces
//! Note:
//! It is possible to define an IN and OUT endpoints with the same number on XMEGA product only
//! E.g. MSC class can be have IN endpoint 0x81 and OUT endpoint 0x01
#define  USB_DEVICE_MAX_EP             4 // 0 to max endpoint requested by interfaces
//@}

//@}


/**
 * Configuration of HID Generic interface (if used)
 * @{
 */
//! Interface callback definition
#define  UDI_HID_GENERIC_ENABLE_EXT()		HID_callback_enable()
#define  UDI_HID_GENERIC_DISABLE_EXT()		HID_callback_disable()
#define  UDI_HID_GENERIC_REPORT_OUT(ptr)	HID_set_feature(ptr)
#define  UDI_HID_GENERIC_SET_FEATURE(f)		HID_set_feature(f)

#define  UDI_HID_REPORT_IN_SIZE             5
#define  UDI_HID_REPORT_OUT_SIZE            64
#define  UDI_HID_REPORT_FEATURE_SIZE        64
#define  UDI_HID_GENERIC_EP_SIZE            64

//#define  UDI_HID_REPORT_SIZE				(39+15)
#define  UDI_HID_REPORT_SIZE				60

/**
 * USB HID Generic low level configuration
 * In standalone these configurations are defined by the HID generic module.
 * For composite device, these configuration must be defined here
 * @{
 */
//! Endpoint numbers definition
#define  UDI_HID_GENERIC_EP_OUT   (2 | USB_EP_DIR_OUT)
#define  UDI_HID_GENERIC_EP_IN    (1 | USB_EP_DIR_IN)

//! Interface number
#define  UDI_HID_GENERIC_IFACE_NUMBER    0
//@}
//@}



/**
 * Configuration of Class Vendor interface (if used)
 * @{
 */

//! Interface callback definition
#define  UDI_VENDOR_ENABLE_EXT()			VEN_callback_enable()
#define  UDI_VENDOR_DISABLE_EXT()			VEN_callback_disable()
#define  UDI_VENDOR_SETUP_OUT_RECEIVED()	VEN_callback_setup_out_received()
#define  UDI_VENDOR_SETUP_IN_RECEIVED()		VEN_callback_setup_in_received()

//! endpoints size for full speed
#define  UDI_VENDOR_EPS_SIZE_INT_FS		64
#define  UDI_VENDOR_EPS_SIZE_BULK_FS	0
#define  UDI_VENDOR_EPS_SIZE_ISO_FS		0

//! endpoints size for high speed
//#define  UDI_VENDOR_EPS_SIZE_INT_HS    64
//#define  UDI_VENDOR_EPS_SIZE_BULK_HS  512
//#define  UDI_VENDOR_EPS_SIZE_ISO_HS    64

/**
 * USB Class Vendor low level configuration
 * In standalone these configurations are defined by the Class Vendor module.
 * For composite device, these configuration must be defined here
 * @{
 */
//! Endpoint numbers definition
#define  UDI_VENDOR_EP_INTERRUPT_IN  (3 | USB_EP_DIR_IN)
#define  UDI_VENDOR_EP_INTERRUPT_OUT (4 | USB_EP_DIR_OUT)
//#define  UDI_VENDOR_EP_BULK_IN       (3 | USB_EP_DIR_IN)
//#define  UDI_VENDOR_EP_BULK_OUT      (4 | USB_EP_DIR_OUT)
//#define  UDI_VENDOR_EP_ISO_IN        (5 | USB_EP_DIR_IN)
//#define  UDI_VENDOR_EP_ISO_OUT       (6 | USB_EP_DIR_OUT)


//! Interface number
#define  UDI_VENDOR_IFACE_NUMBER     1


/**
 * Description of Composite Device
 * @{
 */
#define UDI_COMPOSITE_DESC_T \
	udi_hid_generic_desc_t udi_hid_generic; \
	udi_vendor_desc_t udi_vendor;

//! USB Interfaces descriptor value for Full Speed
#define UDI_COMPOSITE_DESC_FS \
	.udi_hid_generic			= UDI_HID_GENERIC_DESC, \
	.udi_vendor					= UDI_VENDOR_DESC_FS

//! USB Interfaces descriptor value for High Speed
#define UDI_COMPOSITE_DESC_HS \
	.udi_hid_generic			= UDI_HID_GENERIC_DESC, \
	.udi_vendor					= UDI_VENDOR_DESC_HS

//! USB Interface APIs
#define UDI_COMPOSITE_API  \
	&udi_api_hid_generic,	\
	&udi_api_vendor

//@}


/**
 * USB Device Driver Configuration
 * @{
 */
//@}

#include "udi_hid_generic.h"
#include "udi_vendor.h"

/* Declaration of callbacks used by USB
*/
#include "hid.h"
#include "vendor.h"

#endif // _CONF_USB_H_
