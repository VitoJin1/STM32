#include "Step_motor_Control.h"
//void Step_Motor_Control(int dir, int distance)//dir:1 means up -1 means down,distance controled by time count
//{
//    
//}

void Step_Motor_Dir_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		 //IO口速度为50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
    GPIO_ResetBits(GPIOE, GPIO_Pin_4);
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}
void Step_Motor_Move(int dir)//1 means up -1 means down 
{
    
    if(dir>0)
    {
        
        STEP_MOTOR_DIR=0;
        delay_ms(5);
        TIM_SetCompare3(TIM3,500);
    }
    else if (dir<0)
    {   
        
        STEP_MOTOR_DIR=1;
        delay_ms(5);
        TIM_SetCompare3(TIM3,500);
    }
    else if (dir==0)
    {
        
        delay_ms(5);
        TIM_SetCompare3(TIM3,0);
    }
}

void Step_Motor_Control(void)
{ 
    if(MODE==MANUAL_MODE||MODE==AUTO_MODE)
    {
    if(Command.Arm==2)
    {
        LED_Y=0;
        LED_G=1;
        LED_R=1;
         GPIO_SetBits(GPIOB, GPIO_Pin_8);
        //Putter_IN1=1;
        delay_ms(10);
         GPIO_ResetBits(GPIOB, GPIO_Pin_14);
        //Putter_IN2=0;
    }
        
    else if(Command.Arm==0)
    {
        LED_Y=1;
        LED_G=0;
        LED_R=1;
        //Putter_IN1=0;
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
        delay_ms(10);
        GPIO_SetBits(GPIOB, GPIO_Pin_14);
        //Putter_IN2=1;
    }
    else    
    {
        LED_Y=1;
        LED_G=1;
        LED_R=0;
        //Putter_IN1=0;
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
        delay_ms(10);
        //Putter_IN2=0;
        GPIO_ResetBits(GPIOB, GPIO_Pin_14);
    }
    }
    else if(MODE==CALI_MODE)
    {
        LED_Y=1;
        LED_G=1;
        LED_R=1;
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
        delay_ms(10);
        GPIO_ResetBits(GPIOB, GPIO_Pin_14);
    }
    
    /*
    if(MODE==MANUAL_MODE||MODE==AUTO_MODE)
    {
        
    if (Command.Arm==0&&STEPMOTOR_DOWN_LIMIT_FLAG==0)
        Step_Motor_Move(-1);
    else if (Command.Arm==2&&STEPMOTOR_UP_LIMIT_FLAG==0)
        Step_Motor_Move(1); 
    else 
        Step_Motor_Move(0);
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
    else 
    {
        Step_Motor_Move(0);   
    }  */
}
void Valve_Control_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	 //ENABLE PE clock
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		 //IO口速度为50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);//E3=0
}
void Valve_Control(void)
{
//    if(Command.Solenoid_Switch==2)
//        propeller_stop_flag=1;
//    else 
//        propeller_stop_flag=0;

    if(MODE==MANUAL_MODE||MODE==AUTO_MODE)
    {
    if(Command.Solenoid_Switch==1)
        GPIO_SetBits(GPIOE, GPIO_Pin_3);
    else if(Command.Solenoid_Switch==0)
        GPIO_ResetBits(GPIOE, GPIO_Pin_3);
    }
    else if(MODE==CALI_MODE)
    {
        GPIO_ResetBits(GPIOE, GPIO_Pin_3);
    }
    
    
}
