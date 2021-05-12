#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//STM32F103ZE核心板
//LED驱动代码	   
						  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //LED0-->PB.5 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
    GPIO_SetBits(GPIOB,GPIO_Pin_12);						 //PB.5 输出高
    GPIO_SetBits(GPIOB,GPIO_Pin_13);
    GPIO_SetBits(GPIOB,GPIO_Pin_14);						 //PB.5 输出高
    GPIO_SetBits(GPIOB,GPIO_Pin_15);

}
void Signal_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

  
  	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //开启按键端口PA的时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //端口配置为上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//初始化端口
}
