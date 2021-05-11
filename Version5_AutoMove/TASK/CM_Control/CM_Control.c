#include "CM_Control.h"

int Safety_Flag=0x0000;
char str4[10];
char str5[10];
char str3[10];
char str6[10];
char str1[10];
char str2[10];
u8 Cable_Switch=0;// Cable Control Switch, 0 means left, 1 means right
int64_t Length_Left_now =0;
int64_t Length_Right_now=0;
int64_t Length_Left_set=Initial_Left;
int64_t Length_Right_set=Initial_Right;
int64_t x_move=0;
int64_t y_move=0;
int64_t Last_Left_set=Initial_Left;
int64_t Last_Right_set=Initial_Right;
int64_t left_side=0;
int64_t right_side=0;
int accumulate_hor=0;//horizantional speed command
int accumulate_vel=0;//vertical speed command


//PID Init Part*************************************************
PID_Regulator PROP_OUTER_PID=PROP_ANGLE_LOOP_PID;
PID_Regulator PROP_INNER_PID=PROP_GYRO_LOOP_PID;
u8 ManualBalancePressed = 0; 

int rand_left;
int rand_right;

float rand_number_left;
float rand_number_right;
int calc_count=0;
int start_time=0;
u8 encoder_clear_flag=1;
u8 Auto_Back_Flag=0;
volatile int Last_Vel_Left=0;
volatile int Last_Vel_Right=0;
int output_l=0;
int output_r=0;

LPF_1orderRC_F Prop_PID_Output_Filter;

/*record how much pulse sent ,remember to divide by 6 when using, inorder to be persuit percision */

int64_t left_total_pulse_cnt_times6=0;
int64_t right_total_pulse_cnt_times6=0;

u8 LastLeftDir=2;//init value , later on 0 means LEFT_Dir=0 1 means LEFT_Dir =1
u8 LastRightDir=2;//init value , laster on 0 means RIGHT_Dir =0 1 means RIGHT_Dir =1



//cable motor speed control 
/*
parameter : vel1 ,vel2 range: [-3000,3000]
return :none 
*/
void CableMotorSpeedControl(int32_t LVel, int32_t RVel){
    
    LVel=ilimit(LVel,-2500,2500);
    RVel=ilimit(RVel,-2500,2500);
    
    if(LVel>=0){
        if(1!=LastLeftDir){
            LEFT_Dir    = 1;
            LastLeftDir = 1;
            delay_us(2);
        }
    }
    else if(LVel<0){
        if(0!=LastLeftDir){
            LEFT_Dir    = 0;
            LastLeftDir = 0;
            delay_us(2);
        }
    }
    if(RVel>=0){
        if(0!=LastRightDir){
            RIGHT_Dir   =0;
            LastRightDir =0;
            delay_us(2);
        }
    }
    else if(RVel<0){
        if(1!=LastRightDir){
            RIGHT_Dir=1;
            LastRightDir=1;
            delay_us(2);
        }
    }
    
    if(ABS(LVel)<0.1){    //hardware connection error , TIM11 left  | TIM10 right
        TIM11_Freq_Config(0);
    }
    else    {
        TIM11_Freq_Config(1000000*60/(ABS(LVel)*PN008));//left pwm output
        
    }
    left_total_pulse_cnt_times6-=LVel;
    
    if(ABS(RVel)<0.1)
        TIM10_Freq_Config(0);
    else
        TIM10_Freq_Config(1000000*60/(ABS(RVel)*PN008));//middle is 
    right_total_pulse_cnt_times6-=RVel;
}

//**************************************************************
void CableMotorControlLoop(void)
{
    start_time=time_tick_1ms;
    
    volatile int Wind_Left =0;
    volatile int Wind_Right=0;
  
    float pre_Left=0.0;
    float pre_Right=0.0;
    
    if((MODE==MANUAL_MODE)||MODE==CALI_MODE)
    {
        
         Wind_Left =Command.Left_Y_Speed*Cable_Manual_Wind_Speed;
         Wind_Right=Command.Right_Y_Speed*Cable_Manual_Wind_Speed;
         
         //if((((fabs(left_realtime_power)>=Modbus_Max_Power)||(fabs(right_realtime_power)>=Modbus_Max_Power))&&(Wind_Left>=0||Wind_Right>=0))||(left_realtime_temp>=Modbus_Max_Temp)||(right_realtime_temp>=Modbus_Max_Temp) )
         //if( (fabs(left_realtime_power)>=Modbus_Max_Power) || (fabs(right_realtime_power)>=Modbus_Max_Power) ) 
         if((Safety_Flag!=0)&&((Wind_Left>0)||(Wind_Right>0)))
         {
             Wind_Left=0;
             Wind_Right=0;
         }
         
//         printf("left=%d    ",Wind_Left);
//         printf("right=%d",Wind_Right);
//         printf("\r\n");
         
    }
    //else if(Command.Manual_Mode==0&&Command.Auto_Mode==1&&Command.CALI_MODE==0)
    else if(MODE==AUTO_MODE)
    {
         pre_Left =(-Command.Right_X_Speed*sqrt(2)/2.0+Command.Right_Y_Speed*sqrt(2)/2.0);
         pre_Right=(Command.Right_X_Speed*sqrt(2)/2.0+Command.Right_Y_Speed*sqrt(2)/2.0);//-sqrt(2)
         //left is bigger
         if(fabs(Command.Right_X_Speed)-fabs(Command.Right_Y_Speed)>=0.001){
             if(fabs(pre_Left)-fabs(pre_Right)>=0.001){
                 Wind_Left=fabs(Command.Right_X_Speed)*Cable_Wind_Speed;
                 Wind_Right=fabs(pre_Right/pre_Left)*Wind_Left;
             }                 
             else{
                 Wind_Right=fabs(Command.Right_X_Speed)*Cable_Wind_Speed;
                 Wind_Left=fabs(pre_Left/pre_Right)*Wind_Right;
             }
         }
         //right is bigger
         else{
             if(fabs(pre_Left)-fabs(pre_Right)>=0.001){
                 Wind_Left=fabs(Command.Right_Y_Speed)*Cable_Wind_Speed;
                 Wind_Right=fabs(pre_Right/pre_Left)*Wind_Left;
             }
             else{
                 Wind_Right=fabs(Command.Right_Y_Speed)*Cable_Wind_Speed;
                 Wind_Left=fabs(pre_Left/pre_Right)*Wind_Right;
             }
         }
         
         if((Safety_Flag!=0)&&((pre_Left>0)||(pre_Right>0))){
             Wind_Left=0;
             Wind_Right=0;
         }
         if(pre_Left<0)
             Wind_Left=-Wind_Left;         
         if(pre_Right<0)
             Wind_Right=-Wind_Right;
    }
    
    if( (ABS(Wind_Left)<=0.1) && (ABS(Wind_Right)<=0.1) ){
        if(Command.Pose==0)
        {
            AutoMove.clear(&AutoMove);
            //TIM11_Freq_Config(0);
            //TIM10_Freq_Config(0);
        }
        else if(Command.Pose==1){
            switch(AutoMove.status)
            {
                case 0:{
                    AutoMove.PointA[0]=left_total_pulse_cnt_times6;
                    AutoMove.PointA[1]=right_total_pulse_cnt_times6;
                    AutoMove.status=1;
                    break;
                }
                /* comes from B to A*/
                case 2:case 3:case 4:{
                    if(AutoMove.current_step<AutoMove.total_step){
                        AutoMove.current_step++;
                        if( (AutoMove.current_step>=(0*AutoMove.total_step/3) && AutoMove.current_step<(1*AutoMove.total_step/3))\
                        ||  (AutoMove.current_step>=(2*AutoMove.total_step/3) && AutoMove.current_step<(3*AutoMove.total_step/3)) ){
                            AutoMove.vel_left=(AutoMove.PointB[0]-AutoMove.PointA[0])/6/(AutoMove.total_step/3);
                            AutoMove.vel_right=(AutoMove.PointB[1]-AutoMove.PointA[1])/6/(AutoMove.total_step/3);
                        }
                        else if(AutoMove.current_step>=(1*AutoMove.total_step/3) && AutoMove.current_step<(2*AutoMove.total_step/3) ){
                            AutoMove.vel_left =(AutoMove.PointA[0]-AutoMove.PointB[0])/6/(AutoMove.total_step/3);
                            AutoMove.vel_right=(AutoMove.PointA[1]-AutoMove.PointB[1])/6/(AutoMove.total_step/3);
                        }
                        left_total_pulse_cnt_times6=left_total_pulse_cnt_times6+AutoMove.vel_left*PN008/600;
                        
//                        if(AutoMove.vel_left>=0){
//                            LEFT_Dir=1;
//                            delay_us(1);
//                            
//                        }
//                        else if(AutoMove.vel_left<0){
//                            LEFT_Dir=0;
//                            delay_us(1);
//                        }
                        if(AutoMove.vel_left>=0){
                            if(1!=LastLeftDir){
                                LEFT_Dir    = 1;
                                LastLeftDir = 1;
                                delay_us(2);
                            }
                        }
                        else if(AutoMove.vel_left<0){
                            if(0!=LastLeftDir){
                                LEFT_Dir    = 0;
                                LastLeftDir = 0;
                                delay_us(2);
                            }
                        }
                       if(AutoMove.vel_right>=0){
                            if(0!=LastRightDir){
                                RIGHT_Dir   =0;
                                LastRightDir =0;
                                delay_us(2);
                            }
                        }
                        else if(AutoMove.vel_right<0){
                            if(1!=LastRightDir){
                                RIGHT_Dir=1;
                                LastRightDir=1;
                                delay_us(2);
                            }
                        }
                        
                        
                        AutoMove.vel_left=LIMIT_MIN_MAX(AutoMove.vel_left,-400,400);
                        TIM11_Freq_Config( 1000000/(ABS(AutoMove.vel_left))/10 );
                        
                        
//                         if(AutoMove.vel_right>=0){
//                            RIGHT_Dir=0;
//                            delay_us(1);
//                        }
//                        else if(AutoMove.vel_right<0){
//                            RIGHT_Dir=1;
//                            delay_us(1);
//                        }
                         
                        right_total_pulse_cnt_times6=right_total_pulse_cnt_times6+AutoMove.vel_right*PN008/600;
                        AutoMove.vel_right=LIMIT_MIN_MAX(AutoMove.vel_right,-400,400);
                        TIM10_Freq_Config( 1000000/(ABS(AutoMove.vel_right))/10 ); 
                    }
                    
                    
                    
                    else if( AutoMove.current_step==AutoMove.total_step ){
                        AutoMove.status=3;
                        TIM10_Freq_Config(0);
                        TIM11_Freq_Config(0);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        else if(Command.Pose==2){
            switch(AutoMove.status)
            {
                case 0:{
                    AutoMove.clear(&AutoMove);
                    break;
                }
                case 1:{
                    AutoMove.PointB[0]=left_total_pulse_cnt_times6;
                    AutoMove.PointB[1]=right_total_pulse_cnt_times6;
                    AutoMove.status=2;
                    break;
                }
                /*move from A to B */
                case 2:case 3:case 4:{
                    if(AutoMove.current_step>0) {
                        AutoMove.current_step--;
                        //AutoMove.vel_left=(AutoMove.PointA[0]-AutoMove.PointB[0])*10/PN008/AutoMove.total_step;
                        if( (AutoMove.current_step>(0*AutoMove.total_step/3) && AutoMove.current_step<=(1*AutoMove.total_step/3))\
                        ||  (AutoMove.current_step>(2*AutoMove.total_step/3) && AutoMove.current_step<=(3*AutoMove.total_step/3)) ){
                            AutoMove.vel_left  = (AutoMove.PointA[0]-AutoMove.PointB[0])/6/(AutoMove.total_step/3);
                            AutoMove.vel_right = (AutoMove.PointA[1]-AutoMove.PointB[1])/6/(AutoMove.total_step/3);
                        }
                        else if(AutoMove.current_step>(1*AutoMove.total_step/3) && AutoMove.current_step<=(2*AutoMove.total_step/3) ){
                            AutoMove.vel_left  = (AutoMove.PointB[0]-AutoMove.PointA[0])/6/(AutoMove.total_step/3);
                            AutoMove.vel_right = (AutoMove.PointB[1]-AutoMove.PointA[1])/6/(AutoMove.total_step/3);
                        }
                        
                        if(AutoMove.vel_left>=0){
                            if(1!=LastLeftDir){
                                LEFT_Dir    = 1;
                                LastLeftDir = 1;
                                delay_us(2);
                            }
                        }
                        else if(AutoMove.vel_left<0){
                            if(0!=LastLeftDir){
                                LEFT_Dir    = 0;
                                LastLeftDir = 0;
                                delay_us(2);
                            }
                        }
                        if(AutoMove.vel_right>=0){
                            if(0!=LastRightDir){
                                RIGHT_Dir   =0;
                                LastRightDir =0;
                                delay_us(2);
                            }
                        }
                        else if(AutoMove.vel_right<0){
                            if(1!=LastRightDir){
                                RIGHT_Dir=1;
                                LastRightDir=1;
                                delay_us(2);
                            }
                        }
//                        if(AutoMove.vel_left>=0){
//                            LEFT_Dir=1;
//                            delay_us(1);
//                        }
//                        else if(AutoMove.vel_left<0){
//                            LEFT_Dir=0;
//                            delay_us(1);
//                        }
                        left_total_pulse_cnt_times6=left_total_pulse_cnt_times6+AutoMove.vel_left*PN008/600;
                        AutoMove.vel_left=LIMIT_MIN_MAX(AutoMove.vel_left,-400,400);
                        TIM11_Freq_Config( 1000000/(ABS(AutoMove.vel_left))/10 );
                        //printf("%d\r\n,3",1000000/(ABS(AutoMove.vel_left))/10);
                        
//                        if(AutoMove.vel_right>=0){
//                            RIGHT_Dir=0;
//                            delay_us(1);
//                        }
//                        else if(AutoMove.vel_right<0){
//                            RIGHT_Dir=1;
//                            delay_us(1);
//                        }
                        right_total_pulse_cnt_times6=right_total_pulse_cnt_times6+AutoMove.vel_right*PN008/600;
                        AutoMove.vel_right=LIMIT_MIN_MAX(AutoMove.vel_right,-400,400);
                        TIM10_Freq_Config( 1000000/(ABS(AutoMove.vel_right))/10 );
                    }
                    else if(AutoMove.current_step==0){
                        AutoMove.status=4;
                        TIM10_Freq_Config(0);
                        TIM11_Freq_Config(0);
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
    if(AutoMove.status!=2&&AutoMove.status!=3&&AutoMove.status!=4)
        CableMotorSpeedControl(Wind_Left,Wind_Right);
   // printf("%d  %d \r\n",AutoMove.status,AutoMove.current_step);
}




void PropPIDInnnerCalc(PID_Regulator *pid){
    pid->err[1]      =pid->ref-pid->fdb;

    pid->componentP  =pid->kp*pid->err[1];
    pid->componentI +=pid->ki*pid->err[1];
    pid->componentD  =pid->kd*(pid->err[1]-pid->err[0]);
    Prop_PID_Output_Filter.Vi      =pid->componentP+pid->componentI+pid->componentD+pid->ref*PROP_GYRO_FEEDFORD;
    pid->output = LPF_1orderRC_F_FUNC(&Prop_PID_Output_Filter);
    LIMIT_MIN_MAX(pid->output,-pid->outputMax,pid->outputMax);
    //pid->output      = pid->componentP+pid->componentI+pid->componentD+pid->ref*PROP_GYRO_FEEDFORD;     
    //pid->output      = flimit(pid->output,-pid->outputMax,pid->outputMax);
    pid->outputLast=pid->output;
    pid->err[0]      =pid->err[1];
}

void PropPIDOutterCalc(PID_Regulator *pid){
    pid->err[1]      =pid->ref-pid->fdb;
    if(ABS(pid->err[1])<0.01f)
    {
        pid->err[0]=pid->err[1];
        return;
    }
    pid->componentP  =pid->kp*pid->err[1];
    pid->componentI +=pid->ki*pid->err[1];
    pid->componentD  =pid->kd*(pid->err[1]-pid->err[0]);
    pid->output      =pid->componentP+pid->componentI+pid->componentD;
    pid->output      =flimit(pid->output,-pid->outputMax,pid->outputMax);
    pid->outputLast  =pid->output;
    pid->err[0]      =pid->err[1];
}




float outer_last_data=0;
int outer_zero_cnt=0;
void PID_OUTER_Clear()
{
    PROP_OUTER_PID.fdb=0;
    outer_last_data=0;
    outer_zero_cnt=0;
    
}
void PID_OUTER_LOOP( )
{
    //2021/5/7 modified try to write in another way
    /*
    if(Z_EulerAngle-outer_last_data<-pi){
        outer_zero_cnt++;
        PROP_OUTER_PID.fdb=2*pi*(float)outer_zero_cnt+Z_EulerAngle;
    }
    else if(Z_EulerAngle-outer_last_data>pi){
        outer_zero_cnt--;
        PROP_OUTER_PID.fdb=2*pi*(float)outer_zero_cnt+Z_EulerAngle;
    }
    else {
        if(PROP_OUTER_PID.fdb>pi)
            PROP_OUTER_PID.fdb=PROP_OUTER_PID.fdb+(Z_EulerAngle-outer_last_data);
        else if(PROP_OUTER_PID.fdb<-pi)
            PROP_OUTER_PID.fdb=PROP_OUTER_PID.fdb+(Z_EulerAngle-outer_last_data);
        else if((PROP_OUTER_PID.fdb==pi)||(PROP_OUTER_PID.fdb==-pi))
            return;
        else  
            //return;
            PROP_OUTER_PID.fdb=Z_EulerAngle;
    }*/
    if     (Z_EulerAngle - outer_last_data< -pi ) {
        outer_zero_cnt++;
    }
    else if(Z_EulerAngle - outer_last_data>  pi ) {
        outer_zero_cnt--;
    }
    
    PROP_OUTER_PID.fdb=2*pi*(float)outer_zero_cnt+Z_EulerAngle;
    
    //PROP_OUTER_PID.ref=X_Euler_reference+IMU_Shift;
    
    outer_last_data=Z_EulerAngle;
    
    PROP_OUTER_PID.ref=Z_Euler_reference-IMU_Shift;
    
    //PROP_OUTER_PID.fdb=Z_EulerAngle;
    
    PROP_OUTER_PID.Calc(&PROP_OUTER_PID);
}

void PID_INNER_LOOP(){
    if(Command.ACRO_Switch==0){
        if(ManualBalancePressed==1){
            Z_Euler_reference     =Z_EulerAngle;
            PROP_OUTER_PID.output =0.0;  //try to avoid a shock when changing control method;
            ManualBalancePressed  =0;
            IMU_Shift             =0.0;
            outer_zero_cnt        =0;
        }
        PROP_INNER_PID.ref=PROP_OUTER_PID.output;
    }
    else if(Command.ACRO_Switch==1){
        PROP_INNER_PID.ref=Command.Left_X_Speed*(-0.5);
        ManualBalancePressed=1;
    }
    PROP_INNER_PID.fdb=Z_Gyro;
    PROP_INNER_PID.Calc(&PROP_INNER_PID);
}

void Safety_Check_Loop(float left_power, u16 left_temp, float right_power,u16 right_temp)
{
    //printf("check %f %d %f %d\r\n",left_power,left_temp,right_power,right_temp);
    if(fabs(left_power)>=Modbus_Max_Power)
    {
        Safety_Flag=Safety_Flag|0x1000;
        //LED_Y=!LED_Y;
        SetLEDBand(WsDat_Red,last_LEDBanddata_right);
    }
    if(left_temp>=Modbus_Max_Temp)
    {
        Safety_Flag=Safety_Flag|0x0100;
        SetLEDBand(WsDat_Pink,last_LEDBanddata_right);
    }
    if(fabs(right_power)>=Modbus_Max_Power)
    {
        Safety_Flag=Safety_Flag|0x0010;
        SetLEDBand(last_LEDBanddata_left,WsDat_Red);
    }
    if(right_temp>=Modbus_Max_Temp)
    {
        Safety_Flag=Safety_Flag|0x0001;
        SetLEDBand(last_LEDBanddata_left,WsDat_Pink);
    }
    if(Safety_Flag!=0)
    {
        TIM_Cmd(TIM14,ENABLE);
    }
        
}



void LPF_1orderRC_F_init(LPF_1orderRC_F *v)
{
    v->Vi=0.0;
    v->Vo_last=0.0;
    v->Vo=0.0;
    v->Fcutoff=30;
    v->Fs=100;
}

float LPF_1orderRC_F_FUNC(LPF_1orderRC_F *v)
{
    float RC, Cof1,Cof2;
    RC=(float)1.0f/2.0f/PI/v->Fcutoff;
    Cof1=1/(1+RC*v->Fs);
    Cof2=RC*v->Fs/(1+RC*v->Fs);
    v->Vo=Cof1*v->Vi+Cof2*v->Vo_last;
    v->Vo_last=v->Vo;
    return v->Vo;
}
