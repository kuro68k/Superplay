/*
 * usb.h
 *
 * Created: 23/11/2015 09:17:42
 *  Author: Paul Qureshi
 */ 


#ifndef USB_H_
#define USB_H_


/// Microsoft WCID descriptor
typedef struct USB_MicrosoftCompatibleDescriptor_Interface {
	uint8_t bFirstInterfaceNumber;
	uint8_t reserved1;
	uint8_t compatibleID[8];
	uint8_t subCompatibleID[8];
	uint8_t reserved2[6];
} __attribute__((packed)) USB_MicrosoftCompatibleDescriptor_Interface;

typedef struct USB_MicrosoftCompatibleDescriptor {
	uint32_t dwLength;
	uint16_t bcdVersion;
	uint16_t wIndex;
	uint8_t bCount;
	uint8_t reserved[7];
	USB_MicrosoftCompatibleDescriptor_Interface interfaces[];
} __attribute__((packed)) USB_MicrosoftCompatibleDescriptor;

typedef struct USB_MicrosoftExtendedPropertiesDescriptor {
	uint32_t	dwLength;
	uint16_t	bcdVersion;
	uint16_t	wIndex;
	uint16_t	bCount;
	uint32_t	dwPropertySize;
	uint32_t	dwPropertyDataType;
	uint16_t	wPropertyNameLength;
	wchar_t		PropertyName[21];
	uint32_t	dwPropertyDataLength;
	wchar_t		PropertyData[40];
} __attribute__((packed)) USB_MicrosoftExtendedPropertiesDescriptor;


extern bool usb_msft_string(void);
extern bool usb_other_requests(void);


#endif /* USB_H_ */