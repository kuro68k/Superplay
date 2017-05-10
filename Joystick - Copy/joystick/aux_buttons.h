/*
 * aux_buttons.h
 *
 * Created: 15/04/2016 10:58:48
 *  Author: paul.qureshi
 */ 


#ifndef AUX_BUTTONS_H_
#define AUX_BUTTONS_H_


#define AB_DMA_CH			DMA.CH3

#define AB_PORT				PORTC
#define AB_OE_PIN_bm		PIN4_bm
#define AB_DAT_PIN_bm		PIN5_bm
#define AB_LAT_PIN_bm		PIN6_bm
#define AB_CLK_PIN_bm		PIN7_bm

#define AB_USART			USARTC1
#define	AB_USART_BSEL		1			// max 10MHz for NPIC6C596A
#define AB_USART_BSCALE		0


extern void AB_send_report(void);
extern void AB_init(void);


#endif /* AUX_BUTTONS_H_ */