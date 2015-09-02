/*
 * usart.h
 *
 * Created: 02/09/2015 09:27:40
 *  Author: Paul Qureshi
 */ 


#ifndef USART_H_
#define USART_H_


// 2Mbaud @ 16MHz
#define USART_BSEL			0
#define USART_BSCALE		0
#define USART_CLK2X			USART_CLK2X_bm


#define MAIN_USART			USARTD1
#define MAIN_RXC_vect		USARTD1_RXC_vect
#define MAIN_DMA_TRIGSRC	DMA_CH_TRIGSRC_USARTD1_DRE_gc
#define AUX_USART			USARTC1
#define AUX_RXC_vect		USARTC1_RXC_vect
#define AUX_DMA_TRIGSRC		DMA_CH_TRIGSRC_USARTC1_DRE_gc

#define MAIN_DMA_CH			DMA.CH0
#define AUX_DMA_CH			DMA.CH1


extern void USART_init(void);
extern void USART_run(void);


#endif /* USART_H_ */