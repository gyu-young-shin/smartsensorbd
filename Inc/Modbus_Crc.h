#ifndef	__MODBUS_RTU_H_
#define	__MODBUS_RTU_H_
#include "main.h"

#define		CRC_POLY_16			0xA001
#define		CRC_START_MODBUS	0xFFFF

uint16_t crc_modbus(uint8_t *input_str, uint16_t num_bytes);
uint16_t update_crc_16(uint16_t crc, uint8_t c);
void init_crc16_tab(void);


#endif
