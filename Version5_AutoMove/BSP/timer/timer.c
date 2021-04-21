#include "timer.h"

uint64_t time_tick_1ms;
int time_tick_1s=1;
float FIFO_TEST[16][2]={{-500,0}, {-500,-500}, {-1000,-500}, {-1000,0},   {-1500,0},  {-1500,-500},  {0,-500},
                        {0,0} ,  {500,0},     {500,-500}  , {1000,-500}, {1000,0},   {1500,0},      {1500,-500},
                        {0,-500},{0,0}};
u8 FIFO_FINISH =1;

u8 FIFO_Count=0;
u8 SYSTEM_ERROR=0x00;
u8 ROBOT_UNLOCK=0;                        
void DisConnect_Check (void)
{
   
    if((time_tick_1ms-time_tick_LORA)>1500)
    {
        LED_R=0;
        
        SYSTEM_ERROR=SYSTEM_ERROR|0x01;//last bit set to 1
        //printf("blocked");
        //LED_BLUE=!LED_BLUE;
    }
    else
    {
        //if((ROBOT_UNLOCK==1)&&(Safety_Flag==0))
            //SetLEDBand(WsDat_Green,WsDat_Green);
        LED_R=1;
       
        SYSTEM_ERROR=SYSTEM_ERROR&0xFE;
    }
}
void TIM4_PWM_Init(u16 arr,u16 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
	GPIO_Init(GPIOB,&GPIO_InitStructure);              
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=arr;  
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  
 
    TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能
	TIM_Cmd(TIM4, ENABLE);  //使能TIM14							  
    TIM_SetCompare2(TIM4,18000);   
}
void Pump_Init(void){
    u8 i=0;
    TIM_SetCompare2(TIM4,18000);
    for(i=0;i<30;i++)
    delay_ms(50);
    TIM_SetCompare2(TIM4,19000);
    for(i=0;i<30;i++)
    delay_ms(50);
}
void TIM2_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_TIM2); 
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
	GPIO_Init(GPIOB,&GPIO_InitStructure);              
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=arr;  
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  
 
    TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPE使能
	TIM_Cmd(TIM2, ENABLE);  //使能TIM14							  
    TIM_SetCompare1(TIM2,18450);   
    // TIM_SetCompare3(TIM2,19000);
    // TIM_SetCompare4(TIM2,19000); 
}
void TIM8_PWM_Init(u32 arr,u32 psc)
{
    	 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8); 
    
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
	GPIO_Init(GPIOC,&GPIO_InitStructure);              
     
	TIM_TimeBaseStructure.TIM_Prescaler=psc; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_SetCompare2(TIM8,0);
    TIM_ARRPreloadConfig(TIM8,ENABLE);
    TIM_CtrlPWMOutputs(TIM8,ENABLE);
	TIM_Cmd(TIM8, ENABLE);  
}
void TIM3_Int_Init(u16 arr,u16 psc)
{
	
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    TIM_SetCompare3(TIM3,0);
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
}

int Unlock_signal_cnt=0;
int calc_cnt[4]={0,0,0,0};

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
       
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
        
        //accumulate_hor=accumulate_hor+Command.Left_X_Speed;
        //accumulate_vel=accumulate_vel+Command.Right_Y_Speed;
//        if(COMMUNICATION_FLAG==1)
//        {
         if(((time_tick_1ms+97)%SBUS_Updata_Loop==0)){    
             Remote_to_Message();
             //printf("received %d %d %d %d %d %d %d %d %d %d %d \r\n", remote_data.ch0, remote_data.ch1, remote_data.ch2, remote_data.ch3,remote_data.balance_switch,remote_data.mode_switch,remote_data.prop_speed,remote_data.roller_speed,remote_data.screw_switch,remote_data.valve_switch,remote_data.roller_switch);
            /*
            printf("decode=%f %f %f %f %d %d %d %d %d %d %d %d %d \r\n",Command.Right_X_Speed, Command.Right_Y_Speed, Command.Left_X_Speed,Command.Left_Y_Speed,\
            Command.Roller_Speed,Command.Left_Prop_Speed,Command.Right_Prop_Speed,\
            Command.Solenoid_Switch,Command.Mode,Command.Arm,Command.Roller_Switch,Command.Balance_Switch,Command.Pose);*/
            //printf("mode=%d arm=%d \r\n",Command.Mode,Command.Arm);
        }
        //remember to bring it to unlock loop 2020/10/5
        if(((time_tick_1ms+469)%Ultrasonic_detect_Loop==0)&&prop_safe_start==0)
        {
            //Ultrasonic_Detect_Task(time_tick_1s);
            //printf("this time is %lld\r\n",time_tick_1ms);
            Trig1_Enable();
            time_tick_1s++;
        }
//          if(time_tick_1ms%1000==0)
//            {
//                printf("decode=%f %f %f %f ",Command.Right_X_Speed, Command.Right_Y_Speed, Command.Left_X_Speed,Command.Left_Y_Speed);
//                printf("total=%d,cnt=%d\r\n",lora_receive_cnt,Unlock_signal_cnt);
//            }
        if(ROBOT_UNLOCK==1)
        {
           // if((time_tick_1ms+267)%500==0)
                //printf("cnt=IMU:%d, OUTER:%d,INNER:%d,CALC:%d\r\n",calc_cnt[0],calc_cnt[1],calc_cnt[2],calc_cnt[3]);
//                LEDBand_ShowSignal(left_realtime_power,left_realtime_temp,right_realtime_power,right_realtime_temp);
            if((time_tick_1ms+39)%Balance_check_Loop==0)
                    Balance_Check();
            if(Balance_Stable==1)
            {
                if((time_tick_1ms+131)%IMU_Shift_Calibration_Loop==0)
                    IMU_Shift_Cali( );
                if((time_tick_1ms+53)%PROP_OUTER_LOOP==0)
                {
                    PID_OUTER_LOOP( );    
                }
                if((time_tick_1ms+78)%PROP_INNER_LOOP==0)
                {
                    PID_INNER_LOOP( );
                }
            }   
            if((time_tick_1ms+69)%IMU_Updata_Loop==0)
            {
                IMU_Decode(IMU_DATA_Cache);
            }
            
            if(time_tick_1ms%Cable_Control_Loop==0)
            {       
                CableMotorControlLoop( );  
            }
          
            
            
            if(((time_tick_1ms+436)%500==0)&&(Safety_Flag==0))
                Safety_Check_Loop(left_realtime_power,left_realtime_temp,right_realtime_power,right_realtime_temp);
           
            /*
            if((time_tick_1ms+12)%Step_Motor_Control_Loop==0)
           {
                
              fun_test(MODE,Command.Arm);
               //Putter_Control_Loop( );
               //FanControlLoop();
               // Step_Motor_Control();
           }*/
           if((time_tick_1ms+24)%Propeller_Control_Loop==0)
               Propeller_Control( );
            if((time_tick_1ms+37)%Roller_Control_Loop==0){
                Roller_Motor_Control( );
                Pump_Control();
            }
           if((time_tick_1ms+333)%Valve_Control_Loop==0)
               Valve_Control(); 
        }
        else
        {
            if((time_tick_1ms+34)%Unlock_Robot_Loop==0)
        {
            Unlock_Robot_Detect();
        }
        }
       /*if((time_tick_1ms+17)%Encoder_Read_Loop==0)
           encoder_read();*/

       if((time_tick_1ms+784)%Command_Disconnect_Loop==0)
           DisConnect_Check();
        
    //}
           
       time_tick_1ms++;
    
	
	
    }
}

void Target_Update(void)
{
    Absolute_Calc( FIFO_TEST[FIFO_Count][0],FIFO_TEST[FIFO_Count][1],Initial_C,Initial_Left,Initial_Right,Rotate_Matrix);
    FIFO_Count++;
    FIFO_Count=FIFO_Count%16;
}

void Unlock_Robot_Detect(void)
{
   if((Command.Left_X_Speed>0.9f)&&(Command.Right_X_Speed<-0.9f))
      Unlock_signal_cnt++; 
   else
       Unlock_signal_cnt=0;
   if(Unlock_signal_cnt>=20)
   {
       if(Command.Arm==0&&Command.Mode==MANUAL_MODE&&Command.Balance_Switch==0&&Command.Solenoid_Switch==0&&Command.Left_Prop_Speed==0&&Command.Right_Prop_Speed==0&&Command.Roller_Speed==0&&Command.Pose==0&&Command.Pump_Speed==0)
       {
           LED_B=0;
           LED_Y=1;
           //SetLEDBand(WsDat_Green,WsDat_Green);
           ROBOT_UNLOCK=1;//unlock successful
       }
       else
       {
           LED_Y=0;
           LED_B=1;
           //SetLEDBand(WsDat_Org,WsDat_Org);
           Unlock_signal_cnt=0;
       }
   }
}

void TIM12_Init(void)// 2s gap for manual start propeller
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  
	
    TIM_TimeBaseInitStructure.TIM_Period = 2*2000-1; 	
	TIM_TimeBaseInitStructure.TIM_Prescaler=42000-1; 
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStructure);
	TIM_ClearITPendingBit(TIM12,TIM_IT_Update);
	TIM_ITConfig(TIM12,TIM_IT_Update,ENABLE); 
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_BRK_TIM12_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM12,DISABLE); 
}

void TIM8_BRK_TIM12_IRQHandler(void)
{                                                                   
	if(TIM_GetITStatus(TIM12,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM12,TIM_IT_Update);// clear interrupt flag 
        roller_change_cnt=0;
	}
    
}
