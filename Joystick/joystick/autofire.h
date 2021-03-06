/*
 * autofire.h
 *
 */


#ifndef AUTOFIRE_H_
#define AUTOFIRE_H_


#define AF_TC1				TCC0
#define AF_TC1_OVF_vect		TCC0_OVF_vect

#define AF_TC2				TCC1
#define AF_TC2_OVF_vect		TCC1_OVF_vect


// 30Hz
#define AF1_PER				0x208C
#define AF1_CLKSEL			TC_CLKSEL_DIV64_gc

// 15Hz
#define AF2_PER				0x411A
#define AF2_CLKSEL			TC_CLKSEL_DIV64_gc


#define AF_CLKMUL			8
#define AF_CLKMUL_MASK		0x07



extern uint16_t	AF_high_map;
extern uint16_t	AF_low_map;


extern void AF_init(void);
extern void AF_apply(void);



#endif /* AUTOFIRE_H_ */