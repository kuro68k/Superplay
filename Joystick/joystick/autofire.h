/*
 * autofire.h
 *
 * Created: 03/06/2015 21:19:14
 *  Author: MoJo
 */ 


#ifndef AUTOFIRE_H_
#define AUTOFIRE_H_


#define AF_TC1				TCC0
#define AF_TC1_OVF_vect		TCC0_OVF_vect

#define AF_TC2				TCC1
#define AF_TC2_OVF_vect		TCC1_OVF_vect


// 30Hz
#define AF1_PER				0x208C
#define AF1_DIV				TC_CLKSEL_DIV64_gc

// 15Hz
#define AF2_PER				0x411A
#define AF2_DIV				TC_CLKSEL_DIV64_gc



extern uint16_t AF_get_mask(void);



#endif /* AUTOFIRE_H_ */