#ifndef	__RS485_H
#define	__RS485_H

#define	URX1_LEN		32
#define	UTX1_LEN		32
#define ERR_485COMM			0x0100			// 485 통신 불량 에러

void Rs485_proc(void);
void USART1_IRQ_Function(void);				// For debug 485
void SendChar1(unsigned char send_c);
void mprintf1(const char *format, ...);					// For RS485 Debug

#endif
