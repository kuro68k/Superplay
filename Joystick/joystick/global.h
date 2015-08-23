/*
 * global.h
 *
 * Created: 18/12/2014 21:47:07
 *  Author: MoJo
 */ 


#ifndef GLOBAL_H_
#define GLOBAL_H_


#include <stdbool.h>


#define NOP()	__asm__ __volatile__("nop")
#define	WDR()	__asm__ __volatile__("wdr")



#endif /* GLOBAL_H_ */