/*
 * io_table.h
 *
 */


#ifndef IO_TABLE_H_
#define IO_TABLE_H_


typedef struct
{
	PORT_t		*port;
	uint8_t		pin_mask;
} IO_PIN_t;


extern const __flash  IO_PIN_t	io_pin_table[256];



#endif /* IO_TABLE_H_ */