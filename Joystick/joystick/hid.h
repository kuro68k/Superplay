/*
 * hid.h
 *
 * Created: 04/04/2015 16:10:43
 *  Author: Paul Qureshi
 */


#ifndef HID_H_
#define HID_H_


typedef struct
{
	int8_t		x, y;
	uint8_t		buttons1;
	uint8_t		buttons2;
	uint8_t		buttons3;
} HID_REPORT_t;


#define HID_ROTATE_LEFT_BUTTON_bm		(1<<5)
#define HID_ROTATE_RIGHT_BUTTON_bm		(1<<6)

#define HID_TC							TCF0
#define HID_TC_CLKSEL					TC_CLKSEL_DIV64_gc
#define HID_TC_PER						0x1387	// 20ms


extern void HID_init(void);
extern void HID_set_feature(uint8_t *report);
extern void HID_send_report(void);


#endif /* HID_H_ */