#include "Propeller_Control.h"
u8 prop_init_flag=0;
u8 prop_initing_flag=0;
int init_signal_cnt=0;
u8 Balance_Flag=3;//3 means not receive anything
float Z_Euler_reference=0.0;
float Z_Euler_Last_data=0.0;
int Balance_stable_cnt=0;
u8 Balance_Stable=0;
float temp_l=0.0;
float temp_r=0.0;
u8 prop_safe_start=0;
float flimit(float input,float min, float max)
{
    
    if(input<min)
        return min;
    else if(input>max)
        return max;
    else 
        return input;
}
int ilimit(int input,int min, int max)
{
   
    if(input<min)
        return min;
    else if(input>max)
        return max;
    else 
        return input;
}
void Balance_Check(void)
{
    Balance_Flag=Command.Balance_Switch;
    if(Balance_Stable==0)
    {
        if(Balance_Flag==2)
        {
            Z_Euler_reference=Z_EulerAngle;
            
            if(fabs(Z_Euler_reference-Z_Euler_Last_data)<0.1)
                Balance_stable_cnt++;
            else
                Balance_stable_cnt=0;
            if(Balance_stable_cnt>=10)
            {
                Balance_Stable=1;
                LED_G=0;
                
            }
            Z_Euler_Last_data=Z_Euler_reference;
        }
        else if(Balance_Flag==0)
        {
            PID_OUTER_Clear();
            PROP_INNER_PID.output=0;
            PROP_OUTER_PID.output=0;
            Balance_stable_cnt=0;
            IMU_Shift=0.0;
        }
    }
    else if(Balance_Stable==1)
    {
        if(Balance_Flag==0)
        {
            PID_OUTER_Clear();
            Balance_Stable=0;
            
        }
    }
}
    
void ESC_Init_Detect()//detect init signal when not finish init
{
    if((MODE==MANUAL_MODE)&&(Command.Left_X_Speed>=0.9f)&&(Command.Right_X_Speed<=-0.9f))
    {
        init_signal_cnt++;
       
    }
    else
    {
        init_signal_cnt=0;
    }
    if(init_signal_cnt>=10&&prop_initing_flag==0)
    { 
       Propeller_Init(); 
    }
}
void Propeller_Control(void)
{
    if(prop_safe_start==0)
    {
        if(Command.Left_Prop_Speed==0&&Command.Right_Prop_Speed==0&&Command.Balance_Switch==0&&ultraHeightAlert==0)
            prop_safe_start=1;
    }
    calc_cnt[3]++;
    float prop_speed_left=0;
    float prop_speed_right=0;
    float prop_PID_output_l=0.0;
    float prop_PID_output_r=0.0;
    if(Balance_Stable==1)
    {
        prop_PID_output_l=200-PROP_INNER_PID.output;
        prop_PID_output_l=flimit(prop_PID_output_l,0,750);
        
        prop_PID_output_r=200+PROP_INNER_PID.output;
        prop_PID_output_r=flimit(prop_PID_output_r,0,750);
        
        
    }
    else
    {
        prop_PID_output_l=0.0;
        prop_PID_output_r=0.0;
    }
    
    if((MODE==MANUAL_MODE||MODE==AUTO_MODE)&&prop_safe_start==1)
    {
            prop_speed_left=Command.Left_Prop_Speed;
            prop_speed_right=Command.Right_Prop_Speed; 
            temp_l=prop_speed_left+prop_PID_output_l;
            temp_r=prop_speed_right+prop_PID_output_r;
            temp_l=flimit(temp_l,0,750.0);
            temp_r=flimit(temp_r,0,750.0);
            //printf("L:%f R:%f \r\n",temp_l,temp_r);
            TIM_SetCompare3(TIM2,19000-temp_l);
            TIM_SetCompare4(TIM2,19000-temp_r);
    }

    else if (MODE==CALI_MODE)
    {
        PID_OUTER_Clear();
        TIM_SetCompare3(TIM2,19000);

        TIM_SetCompare4(TIM2,19000); 
    }
}
void Propeller_Init(void)
{   
    u8 i; 
    prop_initing_flag=1;
   
    TIM_SetCompare3(TIM2,18000);
    //delay_ms(10);
    TIM_SetCompare4(TIM2,18000);
     for(i=0;i<30;i++)
{
    delay_ms(50);
}     
       
    TIM_SetCompare3(TIM2,19000);
    //delay_ms(10);
    TIM_SetCompare4(TIM2,19000);
    for(i=0;i<30;i++)
{
    delay_ms(50);
}   
    prop_init_flag=1;
    prop_initing_flag=0;
}


//void Prop_PID_Calc(struct PID_Regulator *pid)
//{
//    
//    //float ramp_step=limit/Cable_Control_Loop;
//    
//    pid->outputLast=pid->output;//calcu speed
//    pid->err[1]=pid->ref-pid->fdb;
//    if (fabs(pid->err[1])<=1)
//        pid->err[1]=0;
//    if(fabs(pid->err[1])>=200.0)
//    {
//        pid->componentKi=0;
//    }
//    else if(fabs(pid->err[1])<=1.0)
//        pid->componentKi=0;
//    else
//        pid->componentKi=pid->componentKi+pid->err[1];
//   
//    if(fabs(pid->componentKi)>=pid->componentKiMax)
//    {
//        if(pid->componentKi>0)
//        pid->componentKi=pid->componentKiMax;
//        else
//            pid->componentKi=-pid->componentKiMax;
//    }
//    pid->componentKd=pid->err[1]-pid->err[0];
//    pid->output=pid->kp*pid->err[1]+pid->ki*pid->componentKi+pid->kd*pid->componentKd;
//    
//    
//    if (pid->err[1]>0&&(fabs(pid->output)>pid->outputMax))
//        pid->output=pid->outputMax;
//    else if(pid->err[1]<0&&(fabs(pid->output)>pid->outputMax))
//        pid->output=-pid->outputMax;
//    
//    pid->err[0]=pid->err[1];  
//}

//void Prop_PID_Calc(struct PID_Regulator *pid)
//{
//    
//    pid->ref=0;
//    pid->fdb=X_EulerAngle;
//    pid->err[1]=pid->ref-pid->fdb;
//    pid->output=0.5f*pid->err[1];
    //float ramp_step=limit/Cable_Control_Loop;
    
//    pid->outputLast=pid->output;//calcu speed
//    pid->err[1]=pid->ref-pid->fdb;
//    if (fabs(pid->err[1])<=1)
//        pid->err[1]=0;
//    if(fabs(pid->err[1])>=200.0)
//    {
//        pid->componentKi=0;
//    }
//    else if(fabs(pid->err[1])<=1.0)
//        pid->componentKi=0;
//    else
//        pid->componentKi=pid->componentKi+pid->err[1];
//   
//    if(fabs(pid->componentKi)>=pid->componentKiMax)
//    {
//        if(pid->componentKi>0)
//        pid->componentKi=pid->componentKiMax;
//        else
//            pid->componentKi=-pid->componentKiMax;
//    }
//    pid->componentKd=pid->err[1]-pid->err[0];
//    pid->output=pid->kp*pid->err[1]+pid->ki*pid->componentKi+pid->kd*pid->componentKd;
//    
//    
//    if (pid->err[1]>0&&(fabs(pid->output)>pid->outputMax))
//        pid->output=pid->outputMax;
//    else if(pid->err[1]<0&&(fabs(pid->output)>pid->outputMax))
//        pid->output=-pid->outputMax;
//    
//    pid->err[0]=pid->err[1];  
//}
float IMU_Shift=0.0;
void IMU_Shift_Cali(void)
{
    if((Command.Left_X_Speed!=0))
    {
        if(MODE==AUTO_MODE)
            IMU_Shift=IMU_Shift+0.02*Command.Left_X_Speed;
        else if((MODE==CALI_MODE)||(MODE==MANUAL_MODE))
            IMU_Shift=0;
            
    }
    if(Balance_Stable==0)
        IMU_Shift=0;
}
