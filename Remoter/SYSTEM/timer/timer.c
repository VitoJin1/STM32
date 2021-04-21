#include "timer.h"
#include "IOInput.h"
#include "led.h"
int64_t time_tick_1ms;
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

void ReadStatus(void)
{
    if(A0Status==1)
        IOStatus[0]=1;
    else if(A0Status==0)
        IOStatus[0]=0;
    
    if(A1Status==1)
        IOStatus[1]=1;
    else if(A1Status==0)
        IOStatus[1]=0;
    
    if(A2Status==1){
        IOStatus[2]=1;
        
    }
    else if(A2Status==0){
        IOStatus[2]=0;
        
    }
    
    if(A3Status==1){
        IOStatus[3]=1;
        
    }
    else if(A3Status==0){
        IOStatus[3]=0;
       
    }
    
    if(A4Status==1){
        IOStatus[4]=1;
       
    }
    else if(A4Status==0){
        IOStatus[4]=0;
       
    }
    
    if(A5Status==1){
        IOStatus[5]=1;
       
    }
    else if(A5Status==0){
        IOStatus[5]=0;
      
    }
    
    if(A6Status==1)
        IOStatus[6]=1;
    else if(A6Status==0)
        IOStatus[6]=0;
    
    if(A7Status==1)
        IOStatus[7]=1;
    else if(A7Status==0)
        IOStatus[7]=0;
    
    if(A8Status==1)
        IOStatus[8]=1;
    else if(A8Status==0)
        IOStatus[8]=0;
    
    if(A11Status==1)
        IOStatus[9]=1;
    else if(A11Status==0)
        IOStatus[9]=0;
    
    if(A12Status==1)
        IOStatus[10]=1;
    else if(A12Status==0)
        IOStatus[10]=0;
    
    if(A15Status==1)
        IOStatus[11]=1;
    else if(A15Status==0)
        IOStatus[11]=0;
    
    if(B8Status==1)
        IOStatus[12]=1;
    else if(B8Status==0)
        IOStatus[12]=0;
    
    if(B9Status==1)
        IOStatus[13]=1;
    else if(B9Status==0)
        IOStatus[13]=0;
}
void TIM3_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
		{
            
            if(time_tick_1ms%100==0)
            {
                ReadStatus();
            }
            TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); 
            
            time_tick_1ms++;
		}
}
