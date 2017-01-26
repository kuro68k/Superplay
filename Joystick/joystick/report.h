/*
 * report.h
 *
 */


#ifndef REPORT_H_
#define REPORT_H_

#include "config.h"


extern uint8_t	input_matrix[256];

extern void RPT_refresh_input_matrix(void);
extern void RPT_refresh_leds(void);
extern void RPT_generate_report(uint8_t *buffer);


#endif /* REPORT_H_ */