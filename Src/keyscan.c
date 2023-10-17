/*
 * keyscan.c
 *
 *  Created on: Apr 27, 2023
 *      Author: sgy00
 */
#include "main.h"
#include "keyscan.h"

uint8_t footmode = 0;
int16_t dio_input = 0;
uint8_t prekey_01 = 0 ;
uint8_t prekey_02 = 0 ;
uint8_t prekey_03 = 0 ;
uint8_t do_out1 =0;
uint8_t do_out2=0;
uint8_t flg_do_out1 =0;

unsigned long debounce_timer = 0;    // 디바운스 타이머
unsigned long debounce_delay = 50;    // 디바운스 지연 시간 (밀리초 단위)


extern uint32_t millis_count;

void keyprocess(void)
{
	if(footmode == 0xff)
	{
		if(DIO_IN1 != prekey_01)
		{
			if (millis_count - debounce_timer >= debounce_delay)
			{
				prekey_01 =DIO_IN1;

				if(DIO_IN1)
				{
					if(dio_input)
						dio_input =0;
					else
						dio_input =1;
				}

			}

			// 디바운스 타이머 업데이트
			debounce_timer = millis_count;

		}

		if(flg_do_out1)
			dio_input= 0;

	}
	else
	{
		dio_input = 0;
	}


	// if(DIO_IN2 != prekey_02)
	// {
	// 	if(DIO_IN2)
	// 	{
	// 		DO_OUT1_HIGH;
	// 	}
	// 	else
	// 	{
	// 		DO_OUT1_LOW;
	// 	}

	// 	prekey_02 =DIO_IN2;
	// }

	// if(DIO_IN3 != prekey_03)
	// {
	// 	if(DIO_IN3)
	// 	{
	// 		DO_OUT2_HIGH;
	// 	}
	// 	else
	// 	{
	// 		DO_OUT2_LOW;
	// 	}

	// 	prekey_03 =DIO_IN3;
	// }

}


void oputputprocess(void)
{
	if(do_out1)
	{
		DO_OUT1_HIGH;
	}
	else
	{
		DO_OUT1_LOW;
	}

	if(do_out2)
	{
		DO_OUT2_HIGH;
	}
	else
	{
		DO_OUT2_LOW;
	}

}
