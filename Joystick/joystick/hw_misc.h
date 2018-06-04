/*
 * hw_misc.h
 *
 * Created: 18/12/2014 21:22:30
 *  Author: Paul Qureshi
 */


#ifndef HW_MISC_H_
#define HW_MISC_H_



#define DEBUG_RX_PIN_bm			PIN6_bm
#define DEBUG_TX_PIN_bm			PIN7_bm



/**************************************************************************************************
** Macros
*/

// port settings
#define DISABLE_DIGITAL_INPUT(A)	A = (A & ~PORT_ISC_gm) | PORT_ISC_INPUT_DISABLE_gc
#define ENABLE_PULLUP(A)			A = (A & ~PORT_OPC_gm) | PORT_OPC_PULLUP_gc
#define ENABLE_PULLDOWN(A)			A = (A & ~PORT_OPC_gm) | PORT_OPC_PULLDOWN_gc



/**************************************************************************************************
** External variables and functions
*/

extern uint8_t	HW_last_reset_status;


extern void		HW_init(void);
extern void		HW_reset_rtc(void);



#endif /* HW_MISC_H_ */