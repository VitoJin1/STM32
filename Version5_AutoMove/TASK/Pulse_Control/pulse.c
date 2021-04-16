#include "pulse.h"

struct TwoPointAutoMove AutoMove={{0,0},{0,0},0,600,0,0,0,&ClearData};//60s run all 

/*
TIM5 Init
parameter: u32 Cycle pwm output freq= 1MHz/Cycle
*/
void CableMotor_Dir_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_13);
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_14);
}
void TIM10_Freq_Config(u32 Cycle)//freq=1M/cycle
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM10);
    
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    if(Cycle==0)
        TIM_TimeBaseStructure.TIM_Period = 0;
    else
        TIM_TimeBaseStructure.TIM_Period = Cycle-1;                         
    TIM_TimeBaseStructure.TIM_Prescaler =168-1;                  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;          
    TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);                                       
 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          		  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_Pulse = Cycle/2;                    	
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      
 
    TIM_OC1Init(TIM10, &TIM_OCInitStructure);        				
    TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);               
    TIM_ARRPreloadConfig(TIM10, ENABLE); 
    TIM_Cmd(TIM10,ENABLE);
}

void TIM11_Freq_Config(u32 Cycle)
{
     GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM11);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    if(Cycle==0)
        TIM_TimeBaseStructure.TIM_Period = 0;
    else
        TIM_TimeBaseStructure.TIM_Period = Cycle-1;                         
    TIM_TimeBaseStructure.TIM_Prescaler =168-1;                  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;          
    TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure);                                       

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          		  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_Pulse = Cycle/2;                 	
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      

    TIM_OC1Init(TIM11, &TIM_OCInitStructure);        				
    TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);               
    TIM_ARRPreloadConfig(TIM11, ENABLE); 
    TIM_Cmd(TIM11,ENABLE);
}


void ClearData(struct TwoPointAutoMove *p)
{
    p->current_step=0;
    p->PointA[0]=0;
    p->PointA[1]=0;
    p->PointB[0]=0;
    p->PointB[1]=0;
    p->status=0;
    p->vel_left=0;
    p->vel_right=0;
}

