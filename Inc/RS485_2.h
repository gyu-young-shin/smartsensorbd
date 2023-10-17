#ifndef	__GSEN4852_H
#define	__GSEN4852_H


#define	URX2_LEN	512
#define	UTX2_LEN	512

void GSen485_2_Proc(void);
void USART2_IRQ_Function(void);				// For debug 232

void SendChar2(unsigned char send_c);
//void mprintf2(const char *format, ...);				// For RS232 Debug

#endif
