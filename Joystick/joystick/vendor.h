/*
 * vendor.h
 *
 * Created: 04/04/2015 16:10:51
 *  Author: MoJo
 */ 


#ifndef VENDOR_H_
#define VENDOR_H_


extern bool VEN_callback_enable(void);
extern bool VEN_callback_disable(void);
extern bool VEN_callback_setup_out_received(void);
extern bool VEN_callback_setup_in_received(void);


extern bool VEN_enabled;



#endif /* VENDOR_H_ */