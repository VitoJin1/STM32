#include "m3508.h"
#include "can.h"
#include "key.h"
#include "led.h"
#define ABS(x) ((x>0)? (x):(-x))
u8 mode=STANDBY_MODE;
/*
    mode selection 
    
hold   :   |speed|<10   min torque  
standby:    speed<-10   small torque
normal :    10<speed<3000 medium torque
protect:    speed>3000 high torque
*/
//PID_Regulator M3508_Current_PID=M3508_CURRENT_PID_Init;
//PID_Regulator M3508_Speed_PID=M3508_SPEED_PID_Init;
PID_Regulator M3508_Current_PID=M3508_CURRENT_PID_Init;
PID_Regulator M3508_Speed_PID=M3508_SPEED_PID_Init;
u8 Break_Protect=0;

/* low pass filter define                    */

void LPF_1orderRC_F_init(LPF_1orderRC_F *v)
{
    v->Vi=0.0;
    v->Vo_last=0.0;
    v->Vo=0.0;
    v->Fcutoff=30;
    v->Fs=1000;
}
float LPF_1orderRC_F_FUNC(LPF_1orderRC_F *v)
{
    float RC, Cof1,Cof2;
    RC=(float)1.0/2.0/PI/v->Fcutoff;
    Cof1=1/(1+RC*v->Fs);
    Cof2=RC*v->Fs/(1+RC*v->Fs);
    v->Vo=Cof1*v->Vi+Cof2*v->Vo_last;
    v->Vo_last=v->Vo;
    return v->Vo;
}
/*low pass filter define end */
LPF_1orderRC_F current_filter;
int16_t FuzzyGenerator(int16_t speed)
{
    int16_t result;
    if(speed<=-2000)
        result=0;
    else if(speed>-2000&&speed<0)
        result=0.5*speed+1000;
    else if(speed>=0&&speed<6000)
        result=0.5*speed+1000;
    else if(speed>=6000)
        result=4000;
    return result;
}

int16_t Ilimit(int16_t input, int16_t limit)
{
//    int result= ((input)>limit)? limit : input;
//    return (input>=0) ? result   
    if(input>limit)
        return limit;
    else if (input <-limit)
        return -limit;
    else 
        return input;
}
float Flimit(float input,float limit)
{
    if(input>limit)
        return limit;
    else if(input < -limit)
        return - limit;
    else 
        return input;
}
u8 speed_control_flag=0;

u8 torque_change_cnt=0;

void M3508_Torque_Change()
{
    if(Motor_Speed<10&&Motor_Speed>-10)
    {
        
          //M3508_Current_PID.err[0]=0;
            M3508_Current_PID.kp=0.8;
            M3508_Current_PID.ki=0.08;
            M3508_Current_PID.kd=0;
            M3508_Current_PID.componentI=0.0;
            M3508_Current_PID.componentPMax=6000;
            M3508_Current_PID.componentIMax=4000;
            M3508_Current_PID.componentDMax=400;
            M3508_Current_PID.outputMax=3000;
            M3508_Current_PID.slope=6000;

          //M3508_Speed_PID.err[0]=0.0;
            M3508_Speed_PID.kp=10;
            M3508_Speed_PID.ki=0;
            M3508_Speed_PID.kd=5;
            M3508_Speed_PID.componentI=0.0;
            M3508_Speed_PID.componentPMax=15000;
            M3508_Speed_PID.componentIMax=3000;
            M3508_Speed_PID.componentDMax=10000;
            M3508_Speed_PID.outputMax=3000; 
            M3508_Speed_PID.slope=8000;
            //M3508_Speed_PID.target=1000;
    }
    else if(Motor_Speed<-10)
    {
        M3508_Current_PID.err[0]=0;
            M3508_Current_PID.kp=0.8;
            M3508_Current_PID.ki=0.08;
            M3508_Current_PID.kd=0;
            M3508_Current_PID.componentI=0.0;
            M3508_Current_PID.componentPMax=6000;
            M3508_Current_PID.componentIMax=1600;
            M3508_Current_PID.componentDMax=0;
            M3508_Current_PID.outputMax=3200;
            M3508_Current_PID.slope=6000;

            M3508_Speed_PID.err[0]=0.0;
            M3508_Speed_PID.kp=10;
            M3508_Speed_PID.ki=0;
            M3508_Speed_PID.kd=5;
            M3508_Speed_PID.componentI=0.0;
            M3508_Speed_PID.componentPMax=15000;
            M3508_Speed_PID.componentIMax=3000;
            M3508_Speed_PID.componentDMax=10000;
            M3508_Speed_PID.outputMax=1000; 
            M3508_Speed_PID.slope=8000;
            //M3508_Speed_PID.target=1000;
    }
    else 
    {
        M3508_Current_PID.componentI=0.0;
        M3508_Speed_PID.componentI=0.0;
        M3508_Current_PID.outputMax=6000;
        M3508_Speed_PID.outputMax=6000;
    }
}    
void dectect_refference()
{

    switch(key_value)
    {
        case KEY0_PRES:
            mode=HOLD_MODE;
            M3508_Current_PID.err[0]=0;
            M3508_Current_PID.kp=7;
            M3508_Current_PID.ki=0.04;
            M3508_Current_PID.kd=0;
            M3508_Current_PID.componentI=0.0;
            M3508_Current_PID.componentPMax=10000;
            M3508_Current_PID.componentIMax=8000;//4000
            M3508_Current_PID.componentDMax=400;
            M3508_Current_PID.outputMax=12000;
            M3508_Current_PID.slope=200;
            //M3508_Current_PID.target=1000;
            M3508_Speed_PID.err[0]=0;
            M3508_Speed_PID.kp=7;
            M3508_Speed_PID.ki=0.02;
            M3508_Speed_PID.kd=0;
            M3508_Speed_PID.componentI=0.0;
            M3508_Speed_PID.componentPMax=8000;
            M3508_Speed_PID.componentIMax=3000;
            M3508_Speed_PID.componentDMax=8000;
            //M3508_Speed_PID.outputMax=FuzzyGenerator(M3508_Speed_PID.fdb); 
            M3508_Speed_PID.slope=200;
            M3508_Speed_PID.target=2500;
            LED_G=0;
            LED_Y=1;
            break;
            
        case KEY1_PRES:
            mode=PROTECT_MODE;
             M3508_Current_PID.err[0]=0;
            M3508_Current_PID.kp=7;
            M3508_Current_PID.ki=0.04;
            M3508_Current_PID.kd=0;
            M3508_Current_PID.componentI=0.0;
            M3508_Current_PID.componentPMax=10000;
            M3508_Current_PID.componentIMax=8000;//4000
            M3508_Current_PID.componentDMax=400;
            M3508_Current_PID.outputMax=12000;
            M3508_Current_PID.slope=200;
            
            M3508_Speed_PID.err[0]=0;
            M3508_Speed_PID.kp=7;
            M3508_Speed_PID.ki=0.02;
            M3508_Speed_PID.kd=0;
            M3508_Speed_PID.componentI=0.0;
            M3508_Speed_PID.componentPMax=8000;
            M3508_Speed_PID.componentIMax=3000;
            M3508_Speed_PID.componentDMax=8000;
            M3508_Speed_PID.outputMax=8000; 
            M3508_Speed_PID.slope=200;
            M3508_Speed_PID.target=6000;
            LED_Y=0;
            LED_G=1;
            break;
      
    }
}
void m3508_current_control()
{
    /*
    switch(key_value)
    {
        case KEY0_PRES:
            speed_control_flag=1;
            M3508_Current_PID.target= M3508_Current_PID.target+1000;
            if(M3508_Current_PID.target>3000)
                M3508_Current_PID.target=0;
            LED_G=0;
            LED_R=1;
            break;
        case KEY1_PRES:
            speed_control_flag=0;
            Break_Protect=0;
            M3508_Speed_PID.ref=0;
            //CAN_DJI_C620_DataSend(0,0,0,0);
            LED_R=0;
            break;
    }
    */
    //M3508_Speed_PID.ref=600;
    //M3508_Speed_PID.Calc(&M3508_Speed_PID);
    //M3508_Current_PID.ref=M3508_Speed_PID.output;

    M3508_Current_PID.Calc(&M3508_Current_PID);
    
    CAN_DJI_C620_DataSend((int)M3508_Current_PID.output,0,0,0);
    //CAN_DJI_C620_DataSend(0,0,0,0);
    
    //CAN_DJI_C620_DataSend(M3508_Current_PID.ref,0,0,0);
    //CAN_DJI_C620_DataSend(M3508_Speed_PID.output,0,0,0);
    //CAN_DJI_C620_DataSend(300,0,0,0);
}
void m3508_speed_control()
{
    if(mode==HOLD_MODE)
    {
        M3508_Speed_PID.outputMax=FuzzyGenerator(M3508_Speed_PID.fdb); 
        //M3508_Speed_PID.componentIMax=M3508_Speed_PID.outputMax*0.8;
    }
    M3508_Speed_PID.Calc(&M3508_Speed_PID);
    M3508_Current_PID.target=M3508_Speed_PID.output;
}
int16_t Refference_Generater(int16_t ref,int16_t target,float slope)
{
    int16_t output=ref;
    if(ref<target)
    {
        output=output+slope;
        if(output>target)
            output=target;
    }
    else if(ref>target)
    {
        output=output-slope;
        if(output<target)
            output=target;
    }
    else
        output=target;
    return output;
}

void CURRENT_PID_Calc(PID_Regulator * pid)
{
    
    pid->ref=Refference_Generater(pid->ref,pid->target,pid->slope);
    /*
    Motor_Current_Filter[Motor_Current_Filter_cnt]=Motor_Current;
    Motor_Current_Filter_cnt=(Motor_Current_Filter_cnt+1)%10;
    pid->fdb=(Motor_Current_Filter[0]+Motor_Current_Filter[1]+Motor_Current_Filter[2]+Motor_Current_Filter[3]+Motor_Current_Filter[4]+5+
    Motor_Current_Filter[5]+Motor_Current_Filter[6]+Motor_Current_Filter[7]+Motor_Current_Filter[8]+Motor_Current_Filter[9])/10;
    */
    current_filter.Vi=Motor_Current;
    pid->fdb=(int)LPF_1orderRC_F_FUNC(&current_filter);
   
    
    pid->err[1]=pid->ref-pid->fdb;
    if(ABS(pid->err[1])<30)
        pid->err[1]=0;
    pid->componentP=pid->kp*pid->err[1];
    pid->componentP=Flimit(pid->componentP,pid->componentPMax);
    
    
//    if(pid->outputlast>=0.5*pid->outputMax)
//    {
//        if(pid->err[1]<0)
//           pid->componentI=pid->componentI+pid->ki*pid->err[1];     
//    }
//    else if(pid->outputlast<=-0.5*pid->outputMax)
//    {
//        if(pid->err[1]>0)
//            pid->componentI=pid->componentI+pid->ki*pid->err[1];   
//    }
    pid->componentI=pid->componentI+pid->ki*pid->err[1];
    pid->componentI=Flimit(pid->componentI,pid->componentIMax);
    
    pid->componentD=pid->kd*(pid->err[1]-pid->err[0]);
    pid->componentD=Flimit(pid->componentD,pid->componentDMax);
    
    
    pid->output=(pid->componentP+pid->componentI+pid->componentD);
    pid->output=Flimit(pid->output,pid->outputMax);
    
    pid->outputlast=pid->output;
  
    /*
    if(ABS(pid->err[1])<=50)
    
    {
        pid->componentI=0;
        pid->output=0;
    }*/
    pid->err[0]=pid->err[1];
}
void SPEED_PID_Calc(PID_Regulator * pid)
{
    pid->ref=Refference_Generater(pid->ref,pid->target,pid->slope);
    pid->fdb=Motor_Speed;
    //Motor_Speed_Filter[Motor_Speed_Filter_cnt]=Motor_Speed;
    //Motor_Speed_Filter_cnt=(Motor_Speed_Filter_cnt+1)%5;
    //pid->fdb=(Motor_Speed_Filter[0]+Motor_Speed_Filter[1]+2+Motor_Speed_Filter[2]+Motor_Speed_Filter[3]+Motor_Speed_Filter[4])/5;
    pid->err[1]=pid->ref-pid->fdb;
    
    if(ABS(pid->err[1])<50)
        pid->err[1]=0;
    
    pid->componentP=pid->kp*pid->err[1];
    pid->componentP=Flimit(pid->componentP,pid->componentPMax);
    
        if(pid->outputlast>=0.8*pid->outputMax)
    {
       
        if(pid->err[1]<0)
           pid->componentI=pid->componentI+(pid->ki*pid->err[1]+pid->ki*pid->err[0])/2.0;     
    }
    else if(pid->outputlast<=-0.8*pid->outputMax)
    {
        
        if(pid->err[1]>0)
            pid->componentI=pid->componentI+(pid->ki*pid->err[1]+pid->ki*pid->err[0])/2.0;   
    }
    else
    {
         pid->componentI=pid->componentI+(pid->ki*pid->err[1]+pid->ki*pid->err[0])/2.0;   
    }
    //pid->componentI=pid->componentI+(pid->ki*pid->err[1]+pid->ki*pid->err[0])/2.0;
    pid->componentI=Flimit(pid->componentI,pid->componentIMax);
    pid->componentD=pid->kd*(pid->err[1]-pid->err[0]);
    pid->componentD=Flimit(pid->componentD,pid->componentDMax);
    pid->output=(pid->componentP+pid->componentI+pid->componentD);
    pid->output=Flimit(pid->output,pid->outputMax);
    //if(pid->output>pid->outputMax)
      //  pid->output=pid->outputMax;
    pid->err[0]=pid->err[1];
    pid->outputlast=pid->output;
}

