/*
 * hw_misc.h
 *
 * Created: 18/12/2014 21:22:30
 *  Author: MoJo
 */ 


#ifndef HW_MISC_H_
#define HW_MISC_H_



// PORTA
#define JOY_UP_PIN_bm			PIN0_bm
#define JOY_DOWN_PIN_bm			PIN1_bm
#define JOY_LEFT_PIN_bm			PIN2_bm
#define JOY_RIGHT_PIN_bm		PIN3_bm
#define START_PIN_bm			PIN4_bm
#define COIN_PIN_bm				PIN5_bm
#define AUTO_LOW_5_PIN_bm		PIN6_bm
#define AUTO_LOW_6_PIN_bm		PIN6_bm

// PORTB
#define BUTTON_1_PIN_bm			PIN0_bm
#define BUTTON_2_PIN_bm			PIN1_bm
#define BUTTON_3_PIN_bm			PIN2_bm
#define BUTTON_4_PIN_bm			PIN3_bm
#define BUTTON_5_PIN_bm			PIN4_bm
#define BUTTON_6_PIN_bm			PIN5_bm
#define BUTTON_7_PIN_bm			PIN6_bm
#define BUTTON_8_PIN_bm			PIN7_bm

// PORTC
#define AUTO_LOW_1_PIN_bm		PIN0_bm
#define AUTO_LOW_2_PIN_bm		PIN1_bm
#define AUTO_LOW_3_PIN_bm		PIN2_bm
#define AUTO_LOW_4_PIN_bm		PIN3_bm
// pin 4 SPI/GPIO
// pin 5 SPI/GPIO
#define DEBUG_RX_PIN_bm			PIN6_bm
#define DEBUG_TX_PIN_bm			PIN7_bm

// PORTD
#define AUTO_HIGH_1_PIN_bm		PIN0_bm
#define AUTO_HIGH_2_PIN_bm		PIN1_bm
#define AUTO_HIGH_3_PIN_bm		PIN2_bm
#define AUTO_HIGH_4_PIN_bm		PIN3_bm
#define AUTO_HIGH_5_PIN_bm		PIN4_bm
#define AUTO_HIGH_6_PIN_bm		PIN5_bm
#define USB_UI_RX_PIN_bm		PIN6_bm
#define USB_UI_TX_PIN_bm		PIN7_bm

// PORTE
#define ROTARY_1_PIN_bm			PIN0_bm
#define ROTARY_2_PIN_bm			PIN1_bm
#define ROTARY_3_PIN_bm			PIN2_bm
#define ROTARY_4_PIN_bm			PIN3_bm
#define ROTARY_5_PIN_bm			PIN4_bm
#define ROTARY_6_PIN_bm			PIN5_bm
#define ROTARY_7_PIN_bm			PIN6_bm
#define ROTARY_8_PIN_bm			PIN7_bm

// PORTF
#define ROTARY_9_PIN_bm			PIN0_bm
#define ROTARY_10_PIN_bm		PIN1_bm
#define ROTARY_11_PIN_bm		PIN2_bm
#define ROTARY_12_PIN_bm		PIN3_bm
//#define MODE_8_bm				PIN4_bm
#define MODE_4_bm				PIN5_bm
#define MODE_4AF_bm				PIN6_bm
#define CONTROL_PIN_bm			PIN7_bm



/**************************************************************************************************
** Macros
*/

// port settings
#define DISABLE_DIGITAL_INPUT(A)	A = (A & ~PORT_ISC_gm) | PORT_ISC_INPUT_DISABLE_gc
#define ENABLE_PULLUP(A)			A = (A & ~PORT_OPC_gm) | PORT_OPC_PULLUP_gc
#define ENABLE_PULLDOWN(A)			A = (A & ~PORT_OPC_gm) | PORT_OPC_PULLDOWN_gc

// wait for NVM to wake up for reading the signature row
#define HW_wait_for_sig_row()	do {} while ((NVM.STATUS & NVM_NVMBUSY_bm) == NVM_NVMBUSY_bm);



/**************************************************************************************************
** External variables and functions
*/

extern uint8_t	HW_last_reset_status;


extern void		HW_init(void);
extern void		HW_CCPWrite(volatile uint8_t *address, uint8_t value)		__attribute__((nonnull));



#endif /* HW_MISC_H_ */