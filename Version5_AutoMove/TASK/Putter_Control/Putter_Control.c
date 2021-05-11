#include "Putter_Control.h"
u8 Putter_Value=1;
void Putter_Init()
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
    GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
    
    /*
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_14;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_14);
    */
    /*
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOE,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOE,GPIO_Pin_3);
    */
}
void FanControl(u8 controlvalue)
{
    if(controlvalue>0)
    {
        Putter_IN3=1;
        Putter_IN4=0;
    }
    else if(controlvalue==0)
    {
        Putter_IN3=0;
        Putter_IN4=0;
    }    
}
void FanControlLoop()
{
    if(MODE==MANUAL_MODE||MODE==AUTO_MODE)
    {
        FanControl(1);
    }
    else 
    {
        FanControl(0);
    }
}
void fun_test(u8 mode,short signal)
{
    if(mode==MANUAL_MODE||mode==AUTO_MODE)
    {
        
        if(signal==2){
        Putter_IN1=1;
        delay_ms(5);
        Putter_IN2=0;
        }
        else if(signal==1){   
        delay_ms(5);
        Putter_IN1=0;
        delay_ms(5);
        Putter_IN2=1;
        }
        else
        {
            Putter_IN1=0;
            delay_ms(5);
            Putter_IN2=0;
           
        }
    }
    else if(mode== CALI_MODE)
    {
     
        Putter_IN1=0;
        delay_ms(5);
        Putter_IN2=0;
    }
}
void Putter_Control_Loop()
{
    /*
    if(MODE==MANUAL_MODE||MODE==AUTO_MODE)
    {
        //if(0==Command.Arm&&0==STEPMOTOR_DOWN_LIMIT_FLAG)
        if(remote_data.screw_switch>300&&remote_data.screw_switch<400)
        {
            LED_Y=0;
            LED_R=1;
            LED_G=1;
            delay_ms(10);
            //Putter_IN1=1;
            delay_ms(10);
            //Putter_IN2=0;
            //printf("exterd\r\n");
        }
        //else if(2==Command.Arm&&0==STEPMOTOR_UP_LIMIT_FLAG)
        else if(remote_data.screw_switch>1580&&remote_data.screw_switch<1680)
        {
            LED_Y=1;
            LED_R=0; 
            LED_G=1;
            delay_ms(10);
            //Putter_IN1=0;
            delay_ms(10);
            //Putter_IN2=1;
            //printf("retract\r\n");
        }
        else
        {
            LED_Y=1;
            LED_R=1;
            LED_G=0;
           //printf("stop\r\n");
            //Putter_IN1=0;
            //Putter_IN2=0;
        }
    }
    else
    {
        LED_Y=1;
            LED_R=1;
            LED_G=1;
      //printf("end\r\n");
        //Putter_IN1=0;
        //Putter_IN2=0;
    }*/
    if(MODE==MANUAL_MODE||MODE==AUTO_MODE)
    {
    if(Command.Arm==2)
    {
        LED_Y=0;
        LED_G=1;
        LED_R=1;
        Putter_IN1=1;
        delay_ms(10);
        Putter_IN2=0;
    }
        
    else if(Command.Arm==0)
    {
        LED_Y=1;
        LED_G=0;
        LED_R=1;
        Putter_IN1=0;
        delay_ms(10);
        Putter_IN2=1;
    }
    else    
    {
        LED_Y=1;
        LED_G=1;
        LED_R=0;
        Putter_IN1=0;
        delay_ms(10);
        Putter_IN2=0;
    }
    }
    else if(MODE==CALI_MODE)
    {
        LED_Y=1;
        LED_G=1;
        LED_R=1;
        Putter_IN1=0;
        delay_ms(10);
        Putter_IN2=0;
    }
}
