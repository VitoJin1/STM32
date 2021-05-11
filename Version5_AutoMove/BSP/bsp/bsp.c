#include "bsp.h"

u8 test_data[20][20];				//存放LCD ID字符串

void BSP_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数
    
    Valve_Control_Init();
	LED_Init();					 
    LargeCurrentSwitch();
    KEY_Init();
    LEDBand_Init();
    SetLEDBand(WsDat_Dark,WsDat_Dark);
    USART1_Init(115200);
    uart4_init(115200);
    uart3_init(115200);
    RS485_Init();//USART2  
    //Master_06_Slove(Cable_Motor_Left_Address,Cable_Motor_Speed_Address,0);
    delay_ms(100);
    //Master_06_Slove(Cable_Motor_Right_Address,Cable_Motor_Speed_Address,0);
   TIM2_PWM_Init(19999,83); 
   Roller_init();
    TIM8_PWM_Init(1000-1,84-1);//0.1hz
    //EXTIX_Init();
    Step_Motor_Dir_Init(); 
    Putter_Init();
    SetLEDBand(WsDat_Blue,WsDat_Blue);
    //TIM_SetCompare2(TIM8,500);
    TIM13_Init();//ultrasonic timer 
    //Beep_200ms();
    TIM14_Init();
    TIM12_Init();
//    OLED_Init();
//    OLED_ColorTurn(0);
//    OLED_DisplayTurn(0);
//    OLED_Clear();
//    OLED_ShowString(28,0,"WELBOT",24);
//    OLED_Refresh( );
    Ultrasonic1_Init( );
    //Fan_Control=1;
    CableMotor_Dir_Init();
    TIM10_Freq_Config( 0 );
    TIM11_Freq_Config( 0 );
    TIM4_PWM_Init(19999,84-1);
    Pump_Init();
    
//    ROLLER_Switch=1;
//    delay_ms(1000);

/*
    ProPLeft_Switch=1;
    delay_ms(1000);
    ProPRight_Switch=1;
    delay_ms(1000);*/
    Propeller_Init();
    Putter_IN1=0;
    Putter_IN2=0;
    LPF_1orderRC_F_init(&Prop_PID_Output_Filter);
    TIM3_Int_Init(999,83); 
}







