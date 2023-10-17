#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>
#include "rs485.h"
#include "RS485_2.h"
#include "keyscan.h"

_Bool 		flg_485_send = 0;
_Bool 		urx1_comp;
_Bool 		flg_rs485_error = 0;


uint8_t		urx1_buf[URX1_LEN];
uint8_t		utx1_buf[UTX1_LEN];
uint8_t		utx1_tmp[UTX1_LEN];
uint8_t		utx1_send[UTX1_LEN];

uint8_t		tx1_restart = 1;
uint16_t 	urx1_count = 0;
uint16_t	p1_out = 0;
uint16_t	p1_in = 0;
uint16_t	urx1_tout;

uint8_t		comm_send_count = 0;

//==========================================================================================================
extern uint8_t		urx2_buf[URX2_LEN];
extern __IO uint16_t RegularConvData_Tab[7];
extern	UART_HandleTypeDef huart1;
extern uint16_t MakeCCITT16(uint8_t *data, uint32_t len);
extern int16_t dio_input;
extern uint8_t footmode;
extern uint8_t do_out1;
extern uint8_t flg_do_out1;
extern uint8_t do_out2;

//==========================================================================================================
void USART1_IRQ_Function(void)				// For debug 485
{	
	uint8_t data_char;
	
	uint32_t isrflags   = READ_REG(USART1->ISR);
	uint32_t cr1its     = READ_REG(USART1->CR1);
	uint32_t cr3its     = READ_REG(USART1->CR3);

	uint32_t errorflags;

	/* If no error occurs */
	errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF));
	if (errorflags == 0U)
	{
		/* UART in mode Receiver ---------------------------------------------------*/
		if (((isrflags & USART_ISR_RXNE) != 0U) && ((cr1its & USART_CR1_RXNEIE) != 0U))
		{
			data_char = USART1->RDR & 0x00FF;
		
			if((!urx1_comp) && (urx1_count < URX1_LEN))
			{
				urx1_buf[urx1_count++] = data_char;
				urx1_tout = 10;
			}
		}
		
		// Transmission
		if (((isrflags & USART_ISR_TXE) != 0U) && ((cr1its & USART_CR1_TXEIE) != 0U))
		{
			if(p1_out != p1_in) 							
			{
				USART1->TDR = utx1_buf[p1_out];
				p1_out++;
				p1_out %= UTX1_LEN; 
				tx1_restart = 0;
			}
			else 
			{
				tx1_restart = 1;
				CLEAR_BIT(USART1->CR1, USART_CR1_TXEIE);
				SET_BIT(USART1->CR1, USART_CR1_TCIE);
			}
		}
		
		if (((isrflags & USART_ISR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))		
		{
			RS485_DE_LOW;
			CLEAR_BIT(USART1->CR1, USART_CR1_TCIE);
		}
	}
	else
	{
		__HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_PEF | UART_CLEAR_FEF);
	}
}


//------------------------------------------------------------------------------------------
void Rs485_proc(void) 
{
	uint8_t i;
	uint16_t Rcv_Crc, cmp_Crc;
	
	if(flg_485_send)
	{
		flg_485_send = 0;

		utx1_send[0] = 0xB1;
		utx1_send[1] = RegularConvData_Tab[0] & 0xff;
		utx1_send[2] = RegularConvData_Tab[0]>>8  & 0xff;
		utx1_send[3] = RegularConvData_Tab[1] & 0xff;;
		utx1_send[4] = RegularConvData_Tab[1]>>8  & 0xff;;
		utx1_send[5] = RegularConvData_Tab[2] & 0xff;
		utx1_send[6] = RegularConvData_Tab[2]>>8  & 0xff;
		utx1_send[7] = RegularConvData_Tab[3] & 0xff;;
		utx1_send[8] = RegularConvData_Tab[3]>>8  & 0xff;;
		utx1_send[9] = RegularConvData_Tab[4] & 0xff;;
		utx1_send[10] = RegularConvData_Tab[4]>>8  & 0xff;;
		utx1_send[11] = RegularConvData_Tab[5] & 0xff;;
		utx1_send[12] = RegularConvData_Tab[5]>>8  & 0xff;;
		utx1_send[13] = RegularConvData_Tab[6] & 0xff;;
		utx1_send[14] = RegularConvData_Tab[6]>>8  & 0xff;;
		utx1_send[15] = urx2_buf[3];
		utx1_send[16] = urx2_buf[4];
		utx1_send[17] = urx2_buf[5];
		utx1_send[18] = urx2_buf[6];
		utx1_send[19] = urx2_buf[7];
		utx1_send[20] = urx2_buf[8];
		utx1_send[21] = dio_input;
		cmp_Crc = MakeCCITT16(utx1_send, 22);
		utx1_send[22] = (uint8_t)((cmp_Crc & 0xFF00) >> 8);
		utx1_send[23] = (uint8_t)(cmp_Crc & 0x00FF);
		
		for(i=0; i<24; i++)
			SendChar1(utx1_send[i]);
		
		//mprintf("Send Data = %x bldc1 = %d, bldc2=%d\n", utx1_send[1], COMM_BLDC1_ON, COMM_BLDC2_ON);
//		comm_send_count++;
//		if(comm_send_count > 10)
//		{
//			comm_send_count = 10;
//			flg_rs485_error = 1;
//		}

	}
	
	if(urx1_comp)
	{
		if(urx1_count > 4)
		{
			cmp_Crc = MakeCCITT16(urx1_buf, 5);
			Rcv_Crc= urx1_buf[5] <<8 | urx1_buf[6];
			if(cmp_Crc == Rcv_Crc)
			{
				if(urx1_buf[0] == 0xA1)
				{
					footmode= urx1_buf[1];
					flg_do_out1 = urx1_buf[2];
					do_out1 =urx1_buf[3];
					do_out2 =urx1_buf[4];
					flg_485_send =1 ;

				}
			}


		}
		urx1_count = 0;
		urx1_comp = 0;

	}
}

//------------------------------------------------------------------------------------------
void SendChar1(uint8_t send_c)
{
	uint16_t pin_temp;

	pin_temp = p1_in + 1;
	pin_temp %= UTX1_LEN;

	while(pin_temp == p1_out)
	{
		HAL_Delay(1);
	}
	
	utx1_buf[p1_in] = send_c;
	p1_in = pin_temp;

	if(tx1_restart) 
	{
		tx1_restart = 0;
		RS485_DE_HIGH;
		SET_BIT(USART1->CR1, USART_CR1_TXEIE);
	}
}

//------------------------------------------------------------------------------------------
void mprintf1(const char *format, ...)
{
    uint16_t i;
	__va_list	arglist;

	va_start(arglist, format);
    vsprintf((char *)utx1_tmp, format, arglist);
	va_end(arglist);
	i = 0;
	
	while(utx1_tmp[i] && (i < UTX1_LEN))	// Null 문자�? ?��?��?��?�� 개행문자�? 개행문자 + Carrige Return ?���? �??��
	{
		if(utx1_tmp[i] == '\n')
			SendChar1('\r');
		SendChar1(utx1_tmp[i++]);
	}
}

//------------------------------------------------------------------------------------------


