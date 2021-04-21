#include "encoder.h"
void Encoder_Init(void)
{
    //TIM4_Init();//encoder1
    //TIM5_Init();//encoder2
    
}
void TIM4_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	/*- 正交编码器输入引脚 PB->6   PB->7 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                           

	/*- TIM4编码器模式配置 -*/
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = Encoder_left_count;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
                 
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性
	
	TIM_ICStructInit(&TIM_ICInitStructure);																																		//配置滤波器
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
		
	TIM4->CNT = 0;

	TIM_Cmd(TIM4, ENABLE);   //启动TIM4定时器
}

void TIM5_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   
	
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                           


	TIM_DeInit(TIM5);
	TIM_TimeBaseStructure.TIM_Period = Encoder_right_count;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);              
                 
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性
	
	TIM_ICStructInit(&TIM_ICInitStructure);																																		//配置滤波器
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	TIM5->CNT = 0;
	TIM_Cmd(TIM5, ENABLE);   
}
