/*
 * kbus.h
 *
 */


#ifndef KBUS_H_
#define KBUS_H_


/**************************************************************************************************
** KBUS packets
*/

#define KBUS_PACKET_DATA_SIZE			62
typedef struct
{
	uint8_t		command;
	uint8_t		length;
	uint8_t		data[KBUS_PACKET_DATA_SIZE];
} KBUS_PACKET_t;

#define KBUS_FRAME_START				0x55


/**************************************************************************************************
** KBUS commands
*/

#define KCMD_LOOPBACK				0x50
#define KCMD_READ_STRING			0x51
#define KCMD_READ_VID_PID			0x52
//#define KCMD_READ_DESCRIPTOR		0x53
//#define KCMD_START_REPORTING		0x54
//#define KCMD_STOP_REPORTING			0x55
//#define KCMD_SET_AUTOFIRE_RATE		0x56
//#define KCMD_READ_AUTOFIRE_RATE		0x57
//#define KCMD_SET_AUTOFIRE_MAP		0x58
//#define KCMD_READ_AUTOFIRE_MAP		0x59
#define KCMD_READ_REPORT			0x5A
#define KCMD_ENTER_BOOTLOADER		0x5B


#define KSTRING_DEVICE_NAME			0
#define KSTRING_MANUFACTURER		1
#define KSTRING_SERIAL_NUMBER		3



/**************************************************************************************************
** Public variables and functions
*/

extern void KBUS_process_command(const KBUS_PACKET_t *cmd, KBUS_PACKET_t *res);


#endif /* KBUS_H_ */