#include "timer.h"
#include "led.h"
#include "usart.h"
#include "m3508.h"
#include "can.h"
#define M3508_Control_Loop 1//10ms control loop for m3508 motor


uint64_t time_tick_1ms=0;
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM3, ENABLE); 
							 
}

void TIM3_IRQHandler(void)  
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{
            time_tick_1ms++;
            TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		
            if(time_tick_1ms%500==0)
            {
                LED_B=!LED_B;
            }
            if(time_tick_1ms%300==0)
                dectect_refference(   ); 
            if(time_tick_1ms%3==0)
            {
                if(mode==HOLD_MODE)
                m3508_position_get();
            }
            
            if(time_tick_1ms%(M3508_Control_Loop*5)==0)//5 ms position contorl interrupt
            {
                m3508_position_control();
            } 
            if(time_tick_1ms%(M3508_Control_Loop*5)==0)// 5 ms speed control loop interrupt
            {
                m3508_speed_control(   );
            }
            
            if(time_tick_1ms%(M3508_Control_Loop)==0)// 1ms current loop interrupt
            {
                m3508_current_control(  );
            }
		}
}
