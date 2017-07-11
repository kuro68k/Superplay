/*
 * report.h
 *
 */


#ifndef REPORT_H_
#define REPORT_H_


extern uint8_t	input_matrix[256];

extern void RPT_decode_kbus_matrix(uint8_t *buffer);
extern void RPT_refresh_input_matrix(void);


#endif /* REPORT_H_ */