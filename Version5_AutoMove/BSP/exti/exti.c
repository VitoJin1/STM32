#include "exti.h"
u8 STEPMOTOR_UP_LIMIT_FLAG = 0;
u8 STEPMOTOR_DOWN_LIMIT_FLAG = 0;
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource12);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource11);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line12 | EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
    EXTI_Init(&EXTI_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�3
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);//����
    
    /*clearify the pin statement when start up*/
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)
        STEPMOTOR_DOWN_LIMIT_FLAG=1;
    else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
        STEPMOTOR_DOWN_LIMIT_FLAG=0;
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
        STEPMOTOR_UP_LIMIT_FLAG=1;
    else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)
        STEPMOTOR_UP_LIMIT_FLAG=0;
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line11)==RESET)
    {
        delay_ms(10);
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0)
            STEPMOTOR_DOWN_LIMIT_FLAG=1;
        else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==1)
            STEPMOTOR_DOWN_LIMIT_FLAG=0;
       
    }
    if(EXTI_GetITStatus(EXTI_Line12)==RESET)
    {
        delay_ms(10);
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
            STEPMOTOR_UP_LIMIT_FLAG=1;
        else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)
            STEPMOTOR_UP_LIMIT_FLAG=0; 
    }
}



