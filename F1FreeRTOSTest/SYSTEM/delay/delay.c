#include "delay.h"

#include "FreeRTOS.h"					
#include "task.h"


//////////////////////////////////////////////////////////////////////////////////  

static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数
	
	

extern void xPortSysTickHandler(void);//declare systick function to provide tick for RTOS system
//modified systick function to support FreeRTOS
void SysTick_Handler(void)
{
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED){
        xPortSysTickHandler();
    }
}
			   
//init delay function
//in order to compatible with FreeRTOS, change SYSTICK to AHB frequency.
void delay_init(){
    u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); //use HCLK 72Mhz as systick clock
	fac_us =SystemCoreClock/1000000;				 //us factor, when counted from 0-fac_us,means we reached 1us, hear we have 72 counts means 1us
    reload =SystemCoreClock/1000000;                 //counts when reach 1s ,unit is M
    reload*=1000000/configTICK_RATE_HZ;              //reload = reload(72)*1M/1000---1ms reload- init value is 72k
    fac_ms =1000/configTICK_RATE_HZ;                 //minnal count factor,hear we get 1
    SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;         //enable systick interrupt
    SysTick->LOAD =reload;                           //reload 72k when reach 1ms
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;          //start systick
}

/*
delay n us, input value range 0-2^32/fac_us@72
will not use task scheduling(no FreeRTOS)
*/
void delay_us(u32 nus){
    u32 ticks;
    u32 told,tnow,tcnt=0;
    u32 reload=SysTick->LOAD;                        //get the relaod value set in delay_init()                        
    ticks=nus*fac_us;                                //needed ticks to count,in this case means nus*72k
    told=SysTick->VAL;                               //told means initial value, 
    while(1){
        tnow=SysTick->VAL;                           //current systick value
        if(tnow!=told){                              //counting not finished yet
            if(tnow<told)tcnt+=told-tnow;            //systick reload happened
            else tcnt+=reload-tnow+told;             //systick counting normally
            told=tnow;
            if(tcnt>=ticks)break;       
        }
    }
}
/*
delay n ms, input range 0-2^32
will not use task scheduling(no FreeRTOS)
*/
void delay_xms(u32 nms){
    u32 i;
    for(i=0;i<nms;i++)delay_us(1000);
}

/*
nms delay, 
will use task scheduling(FreeRTOS)
*/
void delay_ms(u32 nms){
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED){     //OS already started
        if(nms>=fac_ms){
            vTaskDelay(nms/fac_ms);
        }
        nms%=fac_ms;
    }
    delay_us((u32)(nms*1000));
}