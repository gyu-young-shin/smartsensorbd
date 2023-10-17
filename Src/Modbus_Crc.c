#include "main.h"
#include "modbus_crc.h"

_Bool		crc_tab16_init = 0;

uint16_t	crc_tab16[256];

void init_crc16_tab(void);
//================================================================
uint16_t crc_modbus(uint8_t *input_str, uint16_t num_bytes) 
{
	uint16_t i, crc;
	uint8_t *ptr;

	if(!crc_tab16_init) 
		init_crc16_tab();

	crc = CRC_START_MODBUS;
	ptr = input_str;

	if (ptr != NULL) 
	{
		for(i=0; i<num_bytes; i++) 
		{
			crc = (crc >> 8) ^ crc_tab16[ (crc ^ (uint16_t) *ptr++) & 0x00FF ];
		}
	}
	
	return crc;

}  /* crc_modbus */
//--------------------------------------------------------------------------
uint16_t update_crc_16(uint16_t crc, uint8_t c) 
{
	uint16_t tmp;
	uint16_t short_c;

	short_c = 0x00ff & (uint16_t) c;

	if(!crc_tab16_init) 
		init_crc16_tab();

	tmp =  crc ^ short_c;
	crc = (crc >> 8) ^ crc_tab16[tmp & 0xff];

	return crc;

}  /* update_crc_16 */

/*
 * static void init_crc16_tab( void );
 *
 * For optimal performance uses the CRC16 routine a lookup table with values
 * that can be used directly in the XOR arithmetic in the algorithm. This
 * lookup table is calculated by the init_crc16_tab() routine, the first time
 * the CRC function is called.
 */

void init_crc16_tab(void) 
{
	uint16_t i;
	uint16_t j;
	uint16_t crc;
	uint16_t c;

	for(i=0; i<256; i++) 
	{
		crc = 0;
		c   = i;

		for(j=0; j<8; j++) 
		{
			if((crc ^ c) & 0x0001) 
				crc = (crc >> 1) ^ CRC_POLY_16;
			else                      
				crc = crc >> 1;

			c = c >> 1;
		}

		crc_tab16[i] = crc;
	}

	crc_tab16_init = 1;

}  /* init_crc16_tab */

