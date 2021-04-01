#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "IOInput.h"
#include "timer.h"


int main(void)
{		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init( );	  
    IOInit();  	
	uart_init(115200);	
    TIM3_Int_Init(999,71);    
    delay_ms(2000);
  while(1)
	{
		UartSendData2Lora(IOStatus);
        delay_ms(500);
	}	 
} 

