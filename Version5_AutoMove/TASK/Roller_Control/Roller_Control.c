#include "Roller_Control.h"
u8 roller_init_flag=0;
u8 roller_initing_flag=0;
int roller_init_signal_cnt=0;
int roller_change_cnt=0;
int roller_last_state=0;
void Roller_ESC_Init_Detect()//detect init signal when not finish init
{
    if((MODE==MANUAL_MODE)&&(Command.Left_X_Speed>=0.9f)&&(Command.Right_X_Speed<=-0.9f))
    {
        roller_init_signal_cnt++;
        LED_B=0;
    }
    else
    {
        roller_init_signal_cnt=0;
       
        
    }
    if(roller_init_signal_cnt>=4&&roller_initing_flag==0)
    {
         LED_B=1;
        Roller_init();
       
    }
        
}
void Roller_init(void)
{   
    u8 i;  
    roller_initing_flag=1;
    TIM_SetCompare1(TIM2,18500);    
    for(i=0;i<Roller_init_delay;i++)
    {
        delay_ms(50);
    } 
    TIM_SetCompare1(TIM2,18000);
    for(i=0;i<Roller_init_delay;i++)
    {
        delay_ms(50);
    }                                              
    
    TIM_SetCompare1(TIM2,19000);     
    for(i=0;i<Roller_init_delay;i++)
    {
    delay_ms(50);
    } 
      TIM_SetCompare1(TIM2,18500);
    roller_init_flag=1;
    roller_initing_flag=0;
}
void Pump_Control(void){
    if(MODE==MANUAL_MODE||MODE==AUTO_MODE){
        if(Command.Pump_Switch==1)
        TIM_SetCompare2(TIM4,18250);
        else if(Command.Pump_Speed==0)
            TIM_SetCompare2(TIM4,19000);
    }
    else if(MODE==CALI_MODE){
        TIM_SetCompare2(TIM4,19000);
    }
}

void Roller_Motor_Control(void){
    if(prop_safe_start==0)
    {
        if(roller_last_state!=Command.Roller_Switch)
        {
            if(roller_change_cnt==0)
                TIM_Cmd(TIM12,ENABLE);
            roller_change_cnt++;
            roller_last_state=Command.Roller_Switch;
        }
        if(roller_change_cnt>=4)
            prop_safe_start=1;        
    }
    if(MODE==MANUAL_MODE||MODE==AUTO_MODE)
    {
    if(roller_init_flag==1&&roller_initing_flag==0)
    {
        if(Command.Roller_Switch==0)
        { 
            TIM_SetCompare1(TIM2,18500+(float)(Command.Roller_Speed/2.0f));
        }
        else if(Command.Roller_Switch==1)
        {
            TIM_SetCompare1(TIM2,18500-(float)(Command.Roller_Speed/2.0f));
        }
    }
    else 
    {
        TIM_SetCompare1(TIM2,18500);
    }
    }
    else if(MODE==CALI_MODE)
    {
        TIM_SetCompare1(TIM2,18500);    
    }
    
}
