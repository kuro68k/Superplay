/*
 * global.h
 *
 * Created: 18/12/2014 21:47:07
 *  Author: Paul Qureshi
 */


#ifndef GLOBAL_H_
#define GLOBAL_H_


#include <stdbool.h>


#define HARDWARE_V1



#define NOP()	__asm__ __volatile__("nop")
#define	WDR()	__asm__ __volatile__("wdr")

#define LE_CHR(a,b,c,d)		( ((uint32_t)(a)<<24) | ((uint32_t)(b)<<16) | ((c)<<8) | (d) )


// compile time static assertions (http://www.pixelbeat.org/programming/gcc/static_assert.html)
// generate a compile time divide by zero if evaluated to false
#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
#define ct_assert(e) enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }



#endif /* GLOBAL_H_ */