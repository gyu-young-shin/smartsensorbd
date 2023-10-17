#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>
#include "RS485_2.h"
#include "modbus_crc.h"

_Bool 		sync_gsen2 = 0;
_Bool 		run_gsen2 = 0;

_Bool 		urx2_comp = 0;
uint8_t		urx2_buf[URX2_LEN];
uint8_t		utx2_buf[UTX2_LEN];
uint8_t		utx2_tmp[UTX2_LEN];

_Bool		tx2_restart = 1;
uint16_t 	urx2_count;
uint16_t	p2_out = 0;
uint16_t	p2_in = 0;
uint16_t	urx2_tout;

uint8_t		utx2_sendbuf[10];
uint16_t		gsen2_buf[6];
uint16_t		gsen2_data[6];
uint8_t		buf_sel2 = 0;
uint16_t	idx_gsen2 = 0;

uint16_t	gsen2_timeout = 0;
//--------------------------------------------------------------------------------------------------------------
extern 	UART_HandleTypeDef huart2;

//==========================================================================================================
void USART2_IRQ_Function(void)				// For debug 232
{	
	uint8_t data_char;
	
	uint32_t isrflags   = READ_REG(USART2->ISR);
	uint32_t cr1its     = READ_REG(USART2->CR1);
	uint32_t cr3its     = READ_REG(USART2->CR3);

	uint32_t errorflags;

	/* If no error occurs */
	errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE));
	if(errorflags == RESET)
	{
		// Receive
		if (((isrflags & USART_ISR_RXNE) != 0U) && ((cr1its & USART_CR1_RXNEIE) != 0U))
		{
			data_char = USART2->RDR & 0x00FF;
		
			if((!urx2_comp) && (urx2_count < URX2_LEN))
			{
				urx2_buf[urx2_count++] = data_char;
				urx2_tout = 2;
			}
		}

		// Transmission
		if (((isrflags & USART_ISR_TXE) != 0U) && ((cr1its & USART_CR1_TXEIE) != 0U))
		{
			if(p2_out != p2_in)
			{
				USART2->TDR = utx2_buf[p2_out];
				p2_out++;
				p2_out %= UTX2_LEN;
				tx2_restart = 0;
			}
			else
			{
				tx2_restart = 1;
				CLEAR_BIT(USART2->CR1, USART_CR1_TXEIE);
				SET_BIT(USART2->CR1, USART_CR1_TCIE);
			}
		}

		if (((isrflags & USART_ISR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
		{
			GSEN_DE2_LOW;
			CLEAR_BIT(USART2->CR1, USART_CR1_TCIE);
		}
	}
	else
	{
		__HAL_UART_CLEAR_FLAG(&huart2, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_PEF | UART_CLEAR_FEF);
	}
}

//---------------------------------------------------------------------------------------
void GSen485_2_Proc(void)
{
	uint32_t i;
	uint16_t make_crc;

//	mprintf("GSen485_2 Proc Start.. !!\n");

	

	if((sync_gsen2 == 0) && (run_gsen2 == 0))
	{
		return ;
	}
	else
	{
		sync_gsen2 = 0;
		run_gsen2 = 0;

		if(urx2_comp || (urx2_count >= 17))
		{

			if((urx2_count == 17) && (urx2_buf[0] == 0x50))
			{
				//memcpy(&gsen2_buf[idx_gsen2][0], &urx2_buf[3], 12);
				gsen2_buf[0] = urx2_buf[3] << 8 | urx2_buf[4];
				gsen2_buf[1] = urx2_buf[5] << 8 | urx2_buf[6];
				gsen2_buf[2] = urx2_buf[7] << 8 | urx2_buf[8];
				gsen2_buf[3] = urx2_buf[9] << 8 | urx2_buf[10];
				gsen2_buf[4]= urx2_buf[11] << 8 | urx2_buf[12];
				gsen2_buf[5] = urx2_buf[13] << 8 | urx2_buf[14];

				gsen2_timeout = 50;		// 50ms

				utx2_sendbuf[0] = 0x50;				// Device Address
				utx2_sendbuf[1] = 0x03;				// Function Code
				utx2_sendbuf[2] = 0x00;				// Register Address High
				utx2_sendbuf[3] = 0x34;				// Register Address Low    Acceleration
				utx2_sendbuf[4] = 0x00;				// Register Data Count High
				utx2_sendbuf[5] = 0x06;				// Register DAta Count Low    Acceleration X, Y, Z

				make_crc = crc_modbus(utx2_sendbuf, 6);
				utx2_sendbuf[6] = (make_crc & 0x00FF);	// CRC-16 Low Byte
				utx2_sendbuf[7] = (make_crc >> 8);		// CRC-16 High Byte

				for(i=0; i<8; i++)
					SendChar2(utx2_sendbuf[i]);
			}

			urx2_count = 0;
			urx2_comp = 0;

		}
		else
		{
			if(gsen2_timeout == 0)
			{

				idx_gsen2 = 0;

				utx2_sendbuf[0] = 0x50;				// Device Address
				utx2_sendbuf[1] = 0x03;				// Function Code
				utx2_sendbuf[2] = 0x00;				// Register Address High
				utx2_sendbuf[3] = 0x34;				// Register Address Low    Acceleration
				utx2_sendbuf[4] = 0x00;				// Register Data Count High
				utx2_sendbuf[5] = 0x06;				// Register DAta Count Low    Acceleration X, Y, Z

				make_crc = crc_modbus(utx2_sendbuf, 6);
				utx2_sendbuf[6] = (make_crc & 0x00FF);	// CRC-16 Low Byte
				utx2_sendbuf[7] = (make_crc >> 8);		// CRC-16 High Byte

				for(i=0; i<8; i++)
					SendChar2(utx2_sendbuf[i]);
			}
		}
	}

}
//---------------------------------------------------------------------------------------
void SendChar2(uint8_t send_c) 
{
	uint16_t pin_temp;

	pin_temp = p2_in + 1;
	pin_temp %= UTX2_LEN;

	while(pin_temp == p2_out)
	{
		HAL_Delay(1);
	}
	utx2_buf[p2_in] = send_c;
	p2_in = pin_temp;
	if(tx2_restart) 
	{
		tx2_restart = 0;
		GSEN_DE2_HIGH;
		SET_BIT(USART2->CR1, USART_CR1_TXEIE);
	}
}
//---------------------------------------------------------------------------------------
void mprintf2(const char *format, ...)
{
    uint16_t i;
	__va_list	arglist;

	va_start(arglist, format);
	vsprintf((char *)utx2_tmp, format, arglist);
	va_end(arglist);
	i = 0;

	while(utx2_tmp[i] && (i < UTX2_LEN))				// Null ????? ?????? ??????? ?????? + Carrige Return ???? ???
	{
		if(utx2_tmp[i] == '\n')
			SendChar2('\r');
		SendChar2(utx2_tmp[i++]);
	}
}

