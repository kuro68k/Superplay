#pragma once

#include "usb.h"

/// Copy data from program memory to the ep0 IN buffer
const uint8_t* usb_ep0_from_progmem(const uint8_t* addr, uint16_t size);


typedef union USB_EP_pair{
	union{
		struct{
			USB_EP_t out;
			USB_EP_t in;
		};
		USB_EP_t ep[2];
	};
} __attribute__((packed)) USB_EP_pair_t;

//extern USB_EP_pair_t *usb_xmega_endpoints;	// for FIFO mode
extern USB_EP_pair_t usb_xmega_endpoints[];
extern const uint8_t usb_num_endpoints;

/* FIFO mode
#define USB_ENDPOINTS(NUM_EP) \
	const uint8_t usb_num_endpoints = (NUM_EP); \
	struct { \
		uint8_t fifo_buffer[((NUM_EP)+1)*4]; \
		USB_EP_pair_t usb_xmega_endpoints[(NUM_EP)+1]; \
	} epptr_ram __attribute__((aligned(2))); \
	USB_EP_pair_t *usb_xmega_endpoints = epptr_ram.usb_xmega_endpoints;
*/

#define USB_ENDPOINTS(NUM_EP) \
	const uint8_t usb_num_endpoints = (NUM_EP); \
	USB_EP_pair_t usb_xmega_endpoints[(NUM_EP)+1] __attribute__((aligned(2)));
