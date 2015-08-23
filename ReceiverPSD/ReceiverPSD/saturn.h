/*
 * saturn.h
 *
 * Created: 01/05/2015 15:06:25
 *  Author: MoJo
 */ 


#ifndef SATURN_H_
#define SATURN_H_

#include "report.h"


/**************************************************************************************************
* Hardware
*/

#define SAT_PORT			PORTA
#define SAT_INT_VECT		PORTA_INT0_vect

#define SAT_TH_PIN_bm		PIN0_bm
#define SAT_TR_PIN_bm		PIN1_bm
#define SAT_TL_PIN_bm		PIN3_bm
#define SAT_D1_PIN_bm		PIN4_bm
#define SAT_D0_PIN_bm		PIN5_bm
#define SAT_D3_PIN_bm		PIN6_bm
#define SAT_D2_PIN_bm		PIN7_bm


// 00
#define SAT_Y_bm			(1<<4)
#define SAT_Z_bm			(1<<5)
#define SAT_R_bm			(1<<6)
#define SAT_X_bm			(1<<7)

// 01
#define SAT_C_bm			(1<<4)
#define SAT_B_bm			(1<<5)
#define SAT_ST_bm			(1<<6)
#define SAT_A_bm			(1<<7)

// 10
#define SAT_DN_bm			(1<<4)
#define SAT_UP_bm			(1<<5)
#define SAT_RT_bm			(1<<6)
#define SAT_LF_bm			(1<<7)

// 11
#define SAT_L_bm			(1<<6)



/**************************************************************************************************
* Public functions and variable
*/

extern void SAT_init(void);
extern void SAT_update(const REPORT_t * const report);



#endif /* SATURN_H_ */