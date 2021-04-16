#include "Ultrasonic.h"
/*
the ideal of simulate a car backing radar;
enable 1 second every m seconds;
@example:
 <= 0.5m distance:1s every 1 seconds;
 0.5m-1m distance:1s every 2 seconds;
 1m-1.5m distance:1s every 3 seconds;
 1.5m-2m distance:1s every 4 seconds;
 2m      distance:disable all the time; 
*/
int  distance =1;
int propeller_stop_flag=1;
int ultrasonic_beep_interval=8;
int ultraHeightAlert=1;

void Ultrasonic_Detect_Task(int ultrasonic_count)
{
//    if(distance>=0 && distance<2)
//        ultrasonic_beep_interval=8;
//    else if(distance>=2 && distance<4)
//        ultrasonic_beep_interval=4;
//    else if(distance>=4 && distance <6)
//        ultrasonic_beep_interval=2;
//    else if(distance>=6 && distance <=8)
//        ultrasonic_beep_interval=1;
//ultrasonic_beep_interval=(int)(distance/2);
    //printf("distance=%d\r\n",distance);
    if(distance<5)
    {
        propeller_stop_flag=1;
        if((ultrasonic_count%distance)==0)
        {
            Beep_200ms();
        }
    }
    else
    {
        propeller_stop_flag=0;
    }
}

void Ultrasonic1_Init(void)
{
    Trig_Init();
    TIM1_CH1_Cap_Init(10*1000-1,168-1);   //TIM1 freq 168MHz,10ms overflow 3.4m/2=1.7m vertical distance overflow 
}
void Trig1_Enable(void)
{ 
    Trig1=1;
    delay_us(20);
    Trig1=0;
    //printf("start:\r\n");
}
void Beep_200ms(void)
{
    TIM_SetCompare2(TIM8,500);
    TIM_SetCounter(TIM13,0);
    TIM_ClearITPendingBit(TIM13,TIM_IT_Update);
    TIM_Cmd(TIM13,ENABLE);
}
