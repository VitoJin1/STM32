#include "IOInput.h"
u8 IOStatus[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void IOInit()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}