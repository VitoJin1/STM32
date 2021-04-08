#include "m3508.h"
#include "can.h"
#include "key.h"
#include "led.h"
#define ABS(x) ((x>0)? (x):(-x))
u8 mode=STANDBY_MODE;

PID_Regulator M3508_Current_PID=M3508_CURRENT_PID_Init;
PID_Regulator M3508_Speed_PID=M3508_SPEED_PID_Init;
PID_Regulator M3508_Position_PID=M3508_POSITION_PID_Init;

u8 Break_Protect=0;
int32_t total_angle=0;
int32_t circle_cnt=0;
int32_t offset_angle=0;
int32_t now_angle=0;
int32_t last_angle=0;
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
LPF_1orderRC_F output_filter;
LPF_1orderRC_F speed_filter;
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

void dectect_refference()
{

    switch(key_value)
    {
        case KEY0_PRES:
            offset_angle=(int32_t)Motor_Angle;
            //M3508_Position_PID.ref=offset_angle;
            last_angle=(int32_t)Motor_Angle;
            mode=HOLD_MODE;
            M3508_Current_PID.err[0]=0;
            M3508_Current_PID.kp=7;
            M3508_Current_PID.ki=0.04;
            M3508_Current_PID.kd=0;
            //M3508_Current_PID.componentI=0.0;
            M3508_Current_PID.componentPMax=16000;
            M3508_Current_PID.componentIMax=16000;//4000
            M3508_Current_PID.componentDMax=0;
            M3508_Current_PID.outputMax=16000;
            M3508_Current_PID.slope=2000;
            //M3508_Current_PID.target=1000;
        
            M3508_Speed_PID.err[0]=0;
            M3508_Speed_PID.kp=3;//5,6开始超过target震荡，7接近等幅振荡
            M3508_Speed_PID.ki=0.15;//0.3,0.025都会有大的漂移，漂移来自周期性负载变化，产生于电机本身，I项作出反应属于正常情况,0.3 I项振幅太大，0.03 I项变化平缓，但是上升时间较长
            M3508_Speed_PID.kd=0;//2.5
            //M3508_Speed_PID.componentI=0.0;
            M3508_Speed_PID.componentPMax=16000;
            M3508_Speed_PID.componentIMax=16000;
            M3508_Speed_PID.componentDMax=16000;
            M3508_Speed_PID.outputMax=16000;
            M3508_Speed_PID.slope=100;
            /*
            if(M3508_Speed_PID.target==0)
            M3508_Speed_PID.target=2000;
            else if(M3508_Speed_PID.target==2000)
                M3508_Speed_PID.target=0;*/
            //0.2,0.0,0.25静态效果好,动态差
            //0.05 0.0006,0.025静态不错，overshoot大
            //0.05,0.0025,0.3机械特性好软
            //0.15,0.05,0.15,响应速度超调都ok，有时稳态震荡，超过范围回不来
            //0.1,0.002,0.5 小型超调一直存在
            //0.15 0.0015 0.1 大范围移动ok，有5000超调
            //0.1 0.001 将积分饱和限制到50%最大输出时，有1000超调
            M3508_Position_PID.err[0]=0;
            M3508_Position_PID.kp=0.1;//0.15; 0.4发散震荡
            M3508_Position_PID.ki=0.001;//0.025;0.01,0.002
            M3508_Position_PID.kd=0.1;//0.1,0.25
            M3508_Position_PID.componentI=0.0;
            M3508_Position_PID.componentPMax=2000;
            M3508_Position_PID.componentIMax=2000;
            M3508_Position_PID.componentDMax=2000;
            M3508_Position_PID.outputMax=2000;
            M3508_Position_PID.slope=2000;
            
            if(M3508_Position_PID.target==0)
                M3508_Position_PID.target=8191*19;
            else if(M3508_Position_PID.target==8191*19)
                M3508_Position_PID.target=0;
            
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
            M3508_Current_PID.componentPMax=6000;
            M3508_Current_PID.componentIMax=4500;//4000
            M3508_Current_PID.componentDMax=400;
            M3508_Current_PID.outputMax=6000;
            M3508_Current_PID.slope=200;
            
            M3508_Speed_PID.err[0]=0;
            M3508_Speed_PID.kp=7;
            M3508_Speed_PID.ki=0.02;
            M3508_Speed_PID.kd=0;
            M3508_Speed_PID.componentI=0.0;
            M3508_Speed_PID.componentPMax=8000;
            M3508_Speed_PID.componentIMax=3000;
            M3508_Speed_PID.componentDMax=8000;
            M3508_Speed_PID.outputMax=6000;
            M3508_Speed_PID.slope=500;
            M3508_Speed_PID.target=0;
            LED_Y=0;
            LED_G=1;
            break;
    }
}
void m3508_current_control( )
{
    M3508_Current_PID.Calc(&M3508_Current_PID);
    output_filter.Vi=M3508_Current_PID.output;   
    CAN_DJI_C620_DataSend(LPF_1orderRC_F_FUNC(&output_filter),0,0,0);
}
//try to realize servo motor trapezoid speed control, the performance is quiet similar to normal method with a slope factor.
//slope factor should be large in inner loop and larger in outer loop
int trapezoid(int32_t speed ,int32_t target)
{
    float coffecient=8191.0/60.0/200.0;
    float acc=100.0;
    float break_distance=(float)speed*(float)speed/2.0/acc*coffecient;//always positive,acc=0.5acc
    if(speed<0)
        break_distance=-break_distance;
    
    if(target<=break_distance)
        return 0;
    else
    {
        return 2000;
       /*
        if (target>=0)
            return 2000;
        else if(target<0)
            return -2000;
        */
    }
}
void m3508_speed_control( )
{
   
    M3508_Speed_PID.Calc(&M3508_Speed_PID);
    M3508_Current_PID.target=M3508_Speed_PID.output;
}
void m3508_position_control( )
{
    M3508_Position_PID.Calc(&M3508_Position_PID);
    M3508_Speed_PID.target=M3508_Position_PID.output;
    //M3508_Speed_PID.target=trapezoid(M3508_Speed_PID.fdb,M3508_Position_PID.err[1]);
}

int32_t Refference_Generater(int32_t ref,int32_t target,float slope)
{
    int32_t output=ref;
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
    current_filter.Vi=Motor_Current;
    pid->fdb=(int)LPF_1orderRC_F_FUNC(&current_filter);
    pid->err[1]=pid->ref-pid->fdb;
    //ignore small error to avoid high freq shaking
    if(ABS(pid->err[1])<30)
        pid->err[1]=0;
    pid->componentP=pid->kp*pid->err[1];
    pid->componentP=Flimit(pid->componentP,pid->componentPMax);

    pid->componentI=pid->componentI+pid->ki*pid->err[1];
    pid->componentI=Flimit(pid->componentI,pid->componentIMax);
    
    pid->componentD=pid->kd*(pid->err[1]-pid->err[0]);
    pid->componentD=Flimit(pid->componentD,pid->componentDMax);
    pid->output=(pid->componentP+pid->componentI+pid->componentD);
    pid->output=Flimit(pid->output,pid->outputMax);
    pid->outputlast=pid->output;
    pid->err[0]=pid->err[1];
}
void SPEED_PID_Calc(PID_Regulator * pid)
{
    float K_Integral=0.0;
    pid->ref=Refference_Generater(pid->ref,pid->target,pid->slope);
    pid->fdb=Motor_Speed;
    pid->err[1]=pid->ref-pid->fdb;
    if(ABS(pid->err[1])<30)
        pid->err[1]=0;
    
    if(ABS(pid->err[1])<500)
    {
        K_Integral=1.0;
    }
    else
    {
        K_Integral=0.0;
    }
    pid->componentP=pid->kp*pid->err[1];
    pid->componentP=Flimit(pid->componentP,pid->componentPMax);
    pid->componentI=pid->componentI+pid->err[1];
    pid->componentI=Flimit(pid->componentI,pid->componentIMax);
    
    pid->componentD=pid->kd*(pid->err[1]-pid->err[0]);
    pid->componentD=Flimit(pid->componentD,pid->componentDMax);
    pid->output=(pid->componentP+pid->componentI+pid->componentD);
    pid->output=Flimit(pid->output,pid->outputMax);
    pid->err[0]=pid->err[1];
    pid->outputlast=pid->output;
}
void m3508_position_get( )
{
    now_angle=Motor_Angle;
    total_angle=now_angle+8192*circle_cnt-offset_angle;
    
    if((now_angle-last_angle)<-4096)
        circle_cnt++;
    else if((now_angle-last_angle)>4096)
        circle_cnt--;
    last_angle=now_angle;
}

void POSITION_PID_Calc(PID_Regulator * pid)
{
    float K_P=1.0;
    float K_I=1.0;
    pid->ref=Refference_Generater(pid->ref,pid->target,pid->slope);
    pid->fdb=total_angle;
    pid->err[1]=pid->ref-pid->fdb;
    //dead zoom ignore, with out these, output will shake because of componentI
    if(ABS(pid->err[1])<200)
    {
        pid->err[0]=pid->err[1];//doing these because we will use componentD
        return;
    }
    pid->componentP=pid->kp*pid->err[1];
    pid->componentP=Flimit(pid->componentP,pid->componentPMax);
    
    //when output reach a partical of outputMax, I component stop accumulate
    if(pid->outputlast>=0.25*pid->outputMax)
    {
       
    if(pid->err[1]<0)
           pid->componentI=pid->componentI+(pid->ki*pid->err[1]+pid->ki*pid->err[0])/2.0;     
    }
    else if(pid->outputlast<=-0.25*pid->outputMax)
    {
    if(pid->err[1]>0)
        pid->componentI=pid->componentI+(pid->ki*pid->err[1]+pid->ki*pid->err[0])/2.0;   
    }
    else
    {
         pid->componentI=pid->componentI+(pid->ki*pid->err[1]+pid->ki*pid->err[0])/2.0;   
    }
    
    //pid->componentI=K_I*pid->componentI;
    pid->componentI=Flimit(pid->componentI,pid->componentIMax);
    
    pid->componentD=pid->kd*(pid->err[1]-pid->err[0]);
    pid->componentD=Flimit(pid->componentD,pid->componentDMax);
    pid->output=(pid->componentP+pid->componentI+pid->componentD);
    pid->output=Flimit(pid->output,pid->outputMax);
    pid->err[0]=pid->err[1];
    pid->outputlast=pid->output;
}
