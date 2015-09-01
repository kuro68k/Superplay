/*
 * kbus.h
 *
 * Created: 01/09/2015 10:25:16
 *  Author: paul.qureshi
 */ 


#ifndef KBUS_H_
#define KBUS_H_


#define KCMD_LOOPBACK				0x50
#define KCMD_READ_STRING			0x51
#define KCMD_READ_VID_PID			0x52
#define KCMD_READ_DESCRIPTOR		0x53
#define KCMD_START_REPORTING		0x54
#define KCMD_STOP_REPORTING			0x55
#define KCMD_SET_AUTOFIRE_RATE		0x56
#define KCMD_READ_AUTOFIRE_RATE		0x57
#define KCMD_SET_AUTOFIRE_MAP		0x58
#define KCMD_READ_AUTOFIRE_MAP		0x59
#define KCMD_READ_REPORT			0x5A
#define KCMD_ENTER_BOOTLOADER		0x5B


#define KSTRING_DEVICE_NAME			0
#define KSTRING_MANUFACTURER		1
#define KSTRING_SERIAL_NUMBER		3

typedef struct 
{
	uint16_t	vid;
	uint16_t	pid;
} KCMD_VID_PID_OUTPUT_t;



#endif /* KBUS_H_ */