#include "main.h"
#include "rs485.h"
//===================================================================
extern _Bool 		flg_485_send;
extern _Bool 		sync_gsen2 ;
extern _Bool 		run_gsen2;
extern _Bool 		urx2_comp;
extern uint16_t	gsen2_timeout;
extern uint16_t	urx2_tout;
extern	_Bool 		urx1_comp;
extern	uint16_t	urx1_tout;

uint8_t callback_1ms = 0;
uint8_t callback_10ms = 0;
uint8_t callback_100ms = 0;
uint8_t callback_300ms = 0;
uint8_t callback_500ms = 0;
uint8_t callback_1min = 0;

volatile uint32_t millis_count = 0;

//===================================================================
void MyTimer_Handler(void)
{
	callback_1ms++;
	millis_count++;

	if(callback_1ms >= 10)
	{
		callback_1ms = 0;
		callback_10ms++;

	}

	if(urx1_tout)					// For RS485
	{
		urx1_tout--;
		if(urx1_tout == 0)
			urx1_comp = 1;
	}


	if(urx2_tout)
	{
		urx2_tout--;
		if(urx2_tout == 0)
			urx2_comp = 1;
	}
    if(gsen2_timeout)
    	gsen2_timeout--;
	
}

void Timer_Proc(void)     
{
    if(callback_10ms >= 10)			// Every 100ms
    {
        callback_10ms = 0;
        callback_100ms++;
		callback_300ms++;
        callback_500ms++;
	
		sync_gsen2= 1;
		run_gsen2= 1;
		
    }

	if(callback_300ms == 3)
	{
		callback_300ms = 0;
//		flg_485_send = 1;

	}
	
    if(callback_500ms == 5)      // 0.5SEC
    {
        callback_500ms = 0;

    }

    if(callback_100ms == 10)      // 1SEC
    {
    	callback_100ms = 0;

	}

    if(callback_1min >= 60)   // 1Min
    {
        callback_1min = 0;
		
		
    }
}
//----------------------------------------------------------------------------
