/*
 * global.h
 *
 * Created: 18/12/2014 21:47:07
 *  Author: Paul Qureshi
 */ 


#ifndef GLOBAL_H_
#define GLOBAL_H_


#include <stdbool.h>


#define NOP()	__asm__ __volatile__("nop")
#define	WDR()	__asm__ __volatile__("wdr")


// compile time static assertions (http://www.pixelbeat.org/programming/gcc/static_assert.html)
// generate a compile time divide by zero if evaluated to false
#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
#define ct_assert(e) enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }



#endif /* GLOBAL_H_ */