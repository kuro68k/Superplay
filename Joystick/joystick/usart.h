/*
 * usart.h
 *
 * Created: 02/09/2015 09:27:40
 *  Author: Paul Qureshi
 */ 


#ifndef USART_H_
#define USART_H_


// 2Mbaud @ 16MHz
#define USART_BSEL				0
#define USART_BSCALE			0
#define USART_CLK2X				USART_CLK2X_bm


#define MAIN_USART				USARTD1
#define MAIN_RXC_vect			USARTD1_RXC_vect
#define MAIN_RX_DMA_TRIGSRC		DMA_CH_TRIGSRC_USARTD1_RXC_gc
#define MAIN_TX_DMA_TRIGSRC		DMA_CH_TRIGSRC_USARTD1_DRE_gc
#define AUX_USART				USARTC1
#define AUX_RXC_vect			USARTC1_RXC_vect
#define AUX_RX_DMA_TRIGSRC		DMA_CH_TRIGSRC_USARTC1_RXC_gc
#define AUX_TX_DMA_TRIGSRC		DMA_CH_TRIGSRC_USARTC1_DRE_gc

#define MAIN_RX_DMA_CH			DMA.CH0
#define AUX_RX_DMA_CH			DMA.CH1
#define MAIN_TX_DMA_CH			DMA.CH2
#define AUX_TX_DMA_CH			DMA.CH3

#define MAIN_RX_TC				TCD1
#define MAIN_RX_TC_OVF_vect		TCD1_OVF_vect
#define MAIN_RX_TC_CCA_vect		TCD1_CCA_vect
#define MAIN_RX_EVENT_CHMUX		EVSYS_CHMUX_PORTD_PIN6_gc
#define AUX_RX_TC				TCE0
#define AUX_RX_TC_OVF_vect		TCE0_OVF_vect
#define AUX_RX_TC_CCA_vect		TCE0_CCA_vect
#define AUX_RX_EVENT_CHMUX		EVSYS_CHMUX_PORTC_PIN6_gc

#define USART_TC_DIV			TC_TC0_CLKSEL_DIV256_gc
#define USART_TC_CCA			1		// 10.667us @ 48MHz


extern void USART_init(void);
extern void USART_run(void);


#endif /* USART_H_ */