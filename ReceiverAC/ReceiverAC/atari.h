/*
 * atari.h
 *
 * Created: 04/05/2016 15:39:20
 *  Author: paul.qureshi
 */ 


#ifndef ATARI_H_
#define ATARI_H_


#define AC_PORT					PORTC
#define AC_JOY_UP_PIN_bm		PIN0_bm
#define AC_JOY_DOWN_PIN_bm		PIN1_bm
#define AC_JOY_LEFT_PIN_bm		PIN2_bm
#define AC_JOY_RIGHT_PIN_bm		PIN3_bm


extern void AC_update(REPORT_t *report);


#endif /* ATARI_H_ */