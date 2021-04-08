#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "can.h"
#include "visual_scope.h"
#include "m3508.h"
/************************************************
//STM32F103ZE核心板
 PWM输出实验  

************************************************/

	
 int main(void)
{		
    float data_upload[8];
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    delay_init();
    uart_init(115200);	
    KEY_Init(); 
    LED_Init();	
    CAN_Config();
    LPF_1orderRC_F_init(&current_filter);
    TIM3_Int_Init(1000-1,72-1); //1ms 中断    
    
    while(1)
	{
        //real feedback from sensor
        
        //M3508_Current_PID.fdb=Motor_Current;
        //M3508_Speed_PID.fdb=Motor_Speed;
        
        key_value=KEY_Scan(1);
        
        data_upload[0]=M3508_Current_PID.ref;
        data_upload[1]=M3508_Current_PID.fdb;
        data_upload[2]=M3508_Current_PID.output;
        data_upload[3]=M3508_Speed_PID.ref;
        data_upload[4]=M3508_Speed_PID.fdb;
        data_upload[5]=M3508_Speed_PID.componentP;
        data_upload[6]=M3508_Speed_PID.componentI;
        data_upload[7]=M3508_Speed_PID.outputlast;
        //data_upload[7]=Motor_Current;
        vcan_sendware((uint8_t *)data_upload,sizeof(data_upload));
        
        //CAN_DJI_C620_DataSend(0,0,0,0);
        delay_ms(10);
        //LED_Y=!LED_Y;
        //printf("Angle=%d,Speed=%d,Current=%d,tempature=%d\r\n",Motor_Angle,Motor_Speed,Motor_Current,Motor_Temp);
    }	 
}

