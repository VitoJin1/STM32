#include "capture.h"
#define ULTRA_H_LIMIT 1000
#define ULTRA_L_LIMIT 900
TIM_ICInitTypeDef  TIM1_ICInitStructure;
u16 TIM1CH1_CAPTURE_VAL;
u16 TIM1CH2_CAPTURE_VAL;
u16 TIM1CH3_CAPTURE_VAL;
u16 TIM1CH4_CAPTURE_VAL;
int Ultrasonic_buf[5];
u8 Ultrasonic_buf_cnt=0;
void UltraDistance_upload(int new_data)
{
    //printf("new data=%d\r\n",new_data);
    Ultrasonic_buf[Ultrasonic_buf_cnt]=new_data;
    Ultrasonic_buf_cnt=(Ultrasonic_buf_cnt+1)%5;
    Distance_Transform((int)((Ultrasonic_buf[0]+Ultrasonic_buf[1]+Ultrasonic_buf[2]+Ultrasonic_buf[3]+Ultrasonic_buf[4])/5));
}
void Trig_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOE,&GPIO_InitStructure); 
    GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14);
}
void TIM1_CH1_Cap_Init(u32 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOE,&GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);
    
    TIM_TimeBaseStructure.TIM_Prescaler=psc;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period=arr;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);
    
    TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
    TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);
    TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);

    TIM_Cmd(TIM1,DISABLE);
   
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
    NVIC_Init(&NVIC_InitStructure);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
    {
        //printf("overflow \r\n");
        
        UltraDistance_upload(0);
        //Distance_Transform(1700);
        TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising);
        TIM_Cmd(TIM1,DISABLE);
    }
    TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
}
void TIM1_CC_IRQHandler(void)
{
    /*
    if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)//time counter overflow
    {
        TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising);
        TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);
        printf ("over flow %lld\r\n",time_tick_1ms);
        TIM_Cmd(TIM1,DISABLE);
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
    */
    
    if(TIM_GetITStatus(TIM1,TIM_IT_CC1)!=RESET)
    {
        if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)==1)
     
        {
            //printf("up edge\r\n");
            TIM1CH1_CAPTURE_VAL=0;
            TIM_Cmd(TIM1,DISABLE);
            TIM_SetCounter(TIM1,0);
            TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);
            TIM_Cmd(TIM1,ENABLE);
            
        }
        else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)==0)
        
        {
            TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);
            UltraDistance_upload((int)(250+0.17f*TIM1CH1_CAPTURE_VAL));
            //printf("received %d\r\n",(int)(150+0.17f*TIM1CH1_CAPTURE_VAL));
            //printf("receive:%d\r\n",TIM1CH1_CAPTURE_VAL);
            //Distance_Transform((int)(250+0.17f*TIM1CH1_CAPTURE_VAL));
            //printf("received %f\r\n",(0.17f*(float)TIM1CH1_CAPTURE_VAL));
            TIM_Cmd(TIM1,DISABLE);
            TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising);
            
        }
        TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);
    }
    
//    if(TIM_GetITStatus(TIM1,TIM_IT_CC2)!=RESET)
//    {
//        if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)==1)
//        {
//            TIM1CH2_CAPTURE_VAL=0;
//            TIM_Cmd(TIM1,DISABLE);
//            TIM_SetCounter(TIM1,0);
//            TIM_OC2PolarityConfig(TIM1,TIM_ICPolarity_Falling);
//            TIM_Cmd(TIM1,ENABLE);
//        }
//        else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)==0)
//        {
//            TIM1CH2_CAPTURE_VAL=TIM_GetCapture2(TIM1);
//            TIM_Cmd(TIM1,DISABLE);
//            TIM_OC2PolarityConfig(TIM1,TIM_ICPolarity_Rising);
//        }
//        TIM_ClearITPendingBit(TIM1,TIM_IT_CC2);
//    }
}
void Distance_Transform(int input_value)
{
    //printf("received :%d\r\n",input_value);
    if(1==ultraHeightAlert&&input_value>=ULTRA_H_LIMIT)
        ultraHeightAlert=0;
    
    if(input_value<300)
        distance=1;
    else if(input_value>=300&&input_value<600)
        distance=2;
    else if(input_value>=600&&input_value<800)
        distance=3;
    else if(input_value>=800&&input_value<1000)
        distance=4;
    else
        distance=5;
}
