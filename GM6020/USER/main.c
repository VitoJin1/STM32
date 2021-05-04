#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "can.h"



int main(void)
 {		
 	u8 t=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	    	 
	uart_init(115200);	
    CAN_Config();     
  while(1)
	{
		CAN_DJI_C620_DataSend(1000,0,0,0);
		delay_ms(500);
		//CAN_DJI_C620_DataSend(0,0,0,0);
	}	 
} 

