#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "H_bridge.h"


int main(void)
{		
 	u8 i=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	    
	uart_init(115200);	
    HBridgeIOInit();  
    
    
    
    while(1){
    for(i=0;i<8;i++){
        if(ValveLastStatus[i]!=ValveStatus[i]){
            if(ValveStatus[i]==0){
                HbridgeSetting(i,-1);
            }
            else if(ValveStatus[i]==1){
                HbridgeSetting(i,1);
            }
            ValveLastStatus[i]=ValveStatus[i];
            delay_ms(200);
        }
        HbridgeSetting(i,0);
    }
	}	 
} 

