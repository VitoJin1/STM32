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

PID_Regulator CM_LEFT_PID=CM_LEFT_PID_Init;
PID_Regulator CM_RIGHT_PID=CM_RIGHT_PID_Init;

PID_Regulator CM_LEFT_PID_AUTO=CM_LEFT_PID_AUTO_Init;
PID_Regulator CM_RIGHT_PID_AUTO=CM_RIGHT_PID_AUTO_Init;


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

/*record how much pulse sent ,remember to divide by 6 when using, inorder to be persuit percision */

int64_t left_total_pulse_cnt_times6=0;
int64_t right_total_pulse_cnt_times6=0;


//cable motor speed control 
/*
parameter : vel1 ,vel2 range: [-3000,3000]
return :none 
*/
void CableMotorSpeedControl(int32_t LVel, int32_t RVel){
    
    LVel=ilimit(LVel,-3000,3000);
    RVel=ilimit(RVel,-3000,3000);
    if(LVel>=0){
        LEFT_Dir=0;
        delay_us(1);
    }
    else if(LVel<0){
        LEFT_Dir=1;
        delay_us(1);
    }
    if(RVel>=0){
        RIGHT_Dir=1;
        delay_us(1);
    }
    else if(RVel<0){
        RIGHT_Dir=0;
        delay_us(1);
    }
    
    if(ABS(LVel)<0.1){    //hardware connection error , TIM11 left  | TIM10 right
        TIM11_Freq_Config(0);
    }
    else    
        TIM11_Freq_Config(1000000*60/(ABS(LVel)*PN008));//left pwm output
    
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
    int right_tight_offset=0;
    volatile int Wind_Left =0;
    volatile int Wind_Right=0;
  
    float pre_Left=0.0;
    float pre_Right=0.0;
    
    if((MODE==MANUAL_MODE)||MODE==CALI_MODE)
    {
        right_tight_offset=0;
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
         if(fabs(Command.Right_X_Speed)-fabs(Command.Right_Y_Speed)>=0.001)//left is bigger
         {

             if(fabs(pre_Left)-fabs(pre_Right)>=0.001)
             {
                 Wind_Left=fabs(Command.Right_X_Speed)*Cable_Wind_Speed;
                 Wind_Right=fabs(pre_Right/pre_Left)*Wind_Left;
             }                 
             else
             {
                 Wind_Right=fabs(Command.Right_X_Speed)*Cable_Wind_Speed;
                 Wind_Left=fabs(pre_Left/pre_Right)*Wind_Right;
             }
         }
         else//right is bigger
         {
             if(fabs(pre_Left)-fabs(pre_Right)>=0.001){
                 Wind_Left=fabs(Command.Right_Y_Speed)*Cable_Wind_Speed;
                 Wind_Right=fabs(pre_Right/pre_Left)*Wind_Left;
             }
             else{
                 Wind_Right=fabs(Command.Right_Y_Speed)*Cable_Wind_Speed;
                 Wind_Left=fabs(pre_Left/pre_Right)*Wind_Right;
             }
         }
         if((Safety_Flag!=0)&&((pre_Left>0)||(pre_Right>0)))
         {
             Wind_Left=0;
             Wind_Right=0;
         }
         if(pre_Left<0)
             Wind_Left=-Wind_Left;         
         if(pre_Right<0)
             Wind_Right=-Wind_Right;
    }
    /*
    if (Cable_Switch==0){
        Cable_Switch=1;
        Master_06_Slove(Cable_Motor_Left_Address,Cable_Motor_Speed_Address,CW*(Wind_Left));
        output_l=CW*(Wind_Left);
    }
    else if (Cable_Switch==1){
        Cable_Switch=0;
        Master_06_Slove(Cable_Motor_Right_Address,Cable_Motor_Speed_Address,CCW*(Wind_Right));
        output_r=CCW*(Wind_Right);
    } */
   
    
    if( (ABS(Wind_Left)<=0.1) && (ABS(Wind_Right)<=0.1) ){
        if(Command.Pose==0)
        {
            AutoMove.clear(&AutoMove);
            TIM11_Freq_Config(0);
            TIM10_Freq_Config(0);
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
                case 2:case 4:{
                    if(AutoMove.current_step<AutoMove.total_step){
                        AutoMove.current_step++;
                        
                        left_total_pulse_cnt_times6=left_total_pulse_cnt_times6+AutoMove.vel_left*PN008/600;
                        AutoMove.vel_left=(AutoMove.PointA[0]-AutoMove.PointB[0])/6/AutoMove.total_step;
                        TIM11_Freq_Config( 1000000/(ABS(AutoMove.vel_left))/10 );
                        
                        AutoMove.vel_right=(AutoMove.PointA[1]-AutoMove.PointB[1])/6/AutoMove.total_step;
                        right_total_pulse_cnt_times6=right_total_pulse_cnt_times6+AutoMove.vel_right*PN008/600;
                        TIM10_Freq_Config( 1000000/(ABS(AutoMove.vel_right))/10 );
                        
                    }
                    else if(AutoMove.current_step==AutoMove.total_step)
                    {
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
                case 3:{
                    if(AutoMove.current_step>0) {
                        AutoMove.current_step--;
                        //AutoMove.vel_left=(AutoMove.PointA[0]-AutoMove.PointB[0])*10/PN008/AutoMove.total_step;
                        AutoMove.vel_left=(AutoMove.PointA[0]-AutoMove.PointB[0])/6/AutoMove.total_step;
                        left_total_pulse_cnt_times6=left_total_pulse_cnt_times6+AutoMove.vel_left*PN008/600;
                        TIM11_Freq_Config( 1000000/(ABS(AutoMove.vel_left))/10 );
                       
                        AutoMove.vel_right=(AutoMove.PointA[1]-AutoMove.PointB[1])/6/AutoMove.total_step;
                        right_total_pulse_cnt_times6=right_total_pulse_cnt_times6+AutoMove.vel_right*PN008/600;
                        TIM10_Freq_Config( 1000000/(ABS(AutoMove.vel_right))/10 );
                         
                    }
                    else if(AutoMove.current_step==0)
                    {
                        AutoMove.status=4;
                    TIM10_Freq_Config(0);
                        TIM11_Freq_Config(0);}
                    break;
                }
                default:
                    break;
            }
        }
    }
    if(AutoMove.status!=2&&AutoMove.status!=3&&AutoMove.status!=4)
        CableMotorSpeedControl(Wind_Left,Wind_Right);
       
    
    printf("%d  %d \r\n",AutoMove.status,AutoMove.current_step);
}
void PID_Loop(void)
{
    CM_LEFT_PID.fdb=Left_length_fdb;
    CM_RIGHT_PID.fdb=Right_length_fdb;
    //Absolute_Calc( Command.x_vel,Command.y_vel,Initial_C,Initial_Left,Initial_Right,Rotate_Matrix);
    //CM_LEFT_PID.ref=Command.x_vel;
    //CM_RIGHT_PID.ref=Command.y_vel;
    
    /*
    if(fabs(CM_LEFT_PID.ref)>fabs(CM_RIGHT_PID.ref))
    {
        CM_RIGHT_PID.outputMax=CM_LEFT_OUTPUTMAX*(fabs(CM_RIGHT_PID.ref)/fabs(CM_LEFT_PID.ref));
        //CM_RIGHT_PID.ramp=CM_LEFT_RAMP*(fabs(CM_RIGHT_PID.ref)/fabs(CM_LEFT_PID.ref));
    }
    else if(fabs(CM_LEFT_PID.ref)<fabs(CM_RIGHT_PID.ref))
    {
        CM_LEFT_PID.outputMax=CM_RIGHT_OUTPUTMAX*(fabs(CM_LEFT_PID.ref)/fabs(CM_RIGHT_PID.ref));
        //CM_LEFT_PID.ramp=CM_RIGHT_RAMP*(fabs(CM_LEFT_PID.ref)/fabs(CM_RIGHT_PID.ref));
    }
    else if (CM_LEFT_PID.ref==0&&fabs(CM_RIGHT_PID.ref))
    {
        CM_LEFT_PID.outputMax=CM_LEFT_OUTPUTMAX;
        CM_RIGHT_PID.outputMax=CM_RIGHT_OUTPUTMAX;
    }
    */
     CM_LEFT_PID.Calc(&CM_LEFT_PID);
     CM_RIGHT_PID.Calc(&CM_RIGHT_PID);   
    if (Cable_Switch==0)
    {
        //Master_06_Slove(Cable_Motor_Left_Address,Cable_Motor_Speed_Address,CCW*CM_LEFT_PID.output);
        SlaveAddress=Cable_Motor_Left_Address;
        StartAddress=Cable_Motor_Speed_Address;
        ValueOrLength=CCW*CM_LEFT_PID.output;
        Cable_Switch=1;
    }
    else if (Cable_Switch==1)
    {
        //Master_06_Slove(Cable_Motor_Right_Address,Cable_Motor_Speed_Address,CW*CM_RIGHT_PID.output);
        SlaveAddress=Cable_Motor_Right_Address;
        StartAddress=Cable_Motor_Speed_Address;
        ValueOrLength=CW*CM_RIGHT_PID.output;
        Cable_Switch=0;
    }
    Master_06_Slove(SlaveAddress,StartAddress,ValueOrLength);
    
}
void Absolute_Calc(float Left_Right,float Up_Down,long c, long left,long right,double theta)
{
    float left_target=0.0;
    float right_target=0.0;
    h2=h-Up_Down;
    if(h2<Dead_Zone_Top&&Up_Down>0)
        Up_Down=0;
    if((left_side+x_move)<Dead_Zone_Side&&left_side<0)    
        Left_Right=0;
    if ((Initial_C-left_side-x_move)<Dead_Zone_Side&&Left_Right>0)
        Left_Right=0;
    Length_Left_set=RT_Angle_Side(h2,(left_side+Left_Right));//danger!!! Up_Down should < h  Left_Right<min(left_side,right_side)
    Length_Right_set=RT_Angle_Side(h2,fabs(right_side-Left_Right));//uint :mm
    CM_LEFT_PID.ref=Length_Left_set-left;
    CM_RIGHT_PID.ref=Length_Right_set-right;
    
    left_target=CM_LEFT_PID.ref-CM_LEFT_PID.fdb;
    right_target=CM_RIGHT_PID.ref-CM_RIGHT_PID.fdb;
    if(fabs(left_target)>fabs(right_target))
    {
        CM_RIGHT_PID.outputMax=CM_LEFT_OUTPUTMAX*(fabs(right_target)/fabs(left_target));
        CM_LEFT_PID.outputMax=CM_LEFT_OUTPUTMAX;
    }
    else if(fabs(left_target)<fabs(right_target))
    {
        CM_LEFT_PID.outputMax=CM_RIGHT_OUTPUTMAX*(fabs(left_target)/fabs(right_target));
        CM_RIGHT_PID.outputMax=CM_RIGHT_OUTPUTMAX;
    }

}
void Movement_Calc(float  Left_Right,float  Up_Down,long c, long left,long right,double theta)//absolute control decode 2019/6/17 add rotate matrix theta(clockwise:-, uint:rand)
{
   
  
    //Rotate Matrix
    //Left_Right=Left_Right*cos(theta)+Up_Down*sin(theta);    
    //Up_Down=Left_Right*sin(theta)+Up_Down*cos(theta);    
    Last_Left_set=Length_Left_set;
    Last_Right_set=Length_Right_set;
    if(h2<Dead_Zone_Top&&Up_Down>0)
        Up_Down=0;
    if((left_side+x_move)<Dead_Zone_Side&&left_side<0)    
        Left_Right=0;
    if ((Initial_C-left_side-x_move)<Dead_Zone_Side&&Left_Right>0)
        Left_Right=0;
    x_move=x_move+Left_Right;
    y_move=y_move+Up_Down;
    h2=h-y_move;//up means positive
    Length_Left_set=RT_Angle_Side(h2,(left_side+x_move));//danger!!! Up_Down should < h  Left_Right<min(left_side,right_side)
    Length_Right_set=RT_Angle_Side(h2,ABS(right_side-x_move));//uint :mm
//    if((float)h2/Length_Left_set<0.2)
//    {
//        Length_Left_set=Last_Left_set;
//        Length_Right_set=Last_Right_set;
//    }
    
    accumulate_hor=0;
    accumulate_vel=0;
}

void PID_Calc(PID_Regulator *pid)
{
    
    //float ramp_step=limit/Cable_Control_Loop;
    pid->outputLast=pid->output;//calcu speed
    pid->err[1]=pid->ref-pid->fdb;
    if (fabs(pid->err[1])<=1)
        pid->err[1]=0;
    if(fabs(pid->err[1])>=200.0)
    {
        pid->componentKi=0;
    }
    else if(fabs(pid->err[1])<=1.0)
        pid->componentKi=0;
    else
        pid->componentKi=pid->componentKi+pid->err[1];
   
    if(fabs(pid->componentKi)>=pid->componentKiMax)
    {
        if(pid->componentKi>0)
        pid->componentKi=pid->componentKiMax;
        else
            pid->componentKi=-pid->componentKiMax;
    }
    pid->componentKd=pid->err[1]-pid->err[0];
    pid->output=pid->kp*pid->err[1]+pid->ki*pid->componentKi+pid->kd*pid->componentKd;
    
    
    if (pid->err[1]>0&&(fabs(pid->output)>pid->outputMax))
        pid->output=pid->outputMax;
    else if(pid->err[1]<0&&(fabs(pid->output)>pid->outputMax))
        pid->output=-pid->outputMax;
    
    pid->err[0]=pid->err[1];
    //give up PID
//    pid->err[1]=pid->ref-pid->fdb;
//    if(fabs(pid->err[1])<=1.0)
//        pid->output=0;
//    else if (pid->err[1]>0)
//        pid->output=pid->outputMax;
//    else if (pid->err[1]<0)
//        pid->output=-pid->outputMax;
    
}

void PropPIDInnnerCalc(PID_Regulator *pid){
    pid->err[1]=pid->ref-pid->fdb;
    pid->componentKp=pid->kp*pid->componentKp;
    pid->componentKi=0.0;
    pid->componentKd=pid->err[1]-pid->err[0];
    pid->output=pid->componentKp+(pid->kd*pid->componentKd);
    pid->output=flimit(pid->output,-pid->outputMax,pid->outputMax);
    pid->err[0]=pid->err[1];
}
void PropPIDOutterCalc(PID_Regulator *pid){
    pid->err[1]=pid->ref-pid->fdb;
    pid->componentKp=pid->kp*pid->componentKp;
    pid->componentKi=0.0;
    pid->componentKd=(pid->err[1]-pid->err[0]);
    pid->output=pid->componentKp+(pid->kd*pid->componentKd);
    pid->output=flimit(pid->output,-pid->outputMax,pid->outputMax);
    pid->err[0]=pid->err[1];
}


void Prop_PID_Calc( PID_Regulator *pid)//copy from stanley top layer code
{   
    pid->this_time=time_tick_1ms;
    pid->err[1]=pid->ref-pid->fdb;//the latest error ------err[1]
    
    
    pid->componentKi=pid->componentKi+pid->err[1]/20;
    
      
    if(pid->componentKi>pid->componentKiMax)
        pid->componentKi=pid->componentKiMax;
    else if(pid->componentKi<-pid->componentKiMax)
        pid->componentKi=-pid->componentKiMax;
    pid->componentKd=(pid->err[1]-pid->err[0])*20;
    pid->output=pid->kp*pid->err[1]+(pid->ki)*(pid->componentKi)+(pid->kd*pid->componentKd);
//    
//    if(pid->err[1]>0&&(fabs(pid->output)>pid->outputMax))
//        pid->output=pid->outputMax;
//    else if(pid->err[1]<0&&(fabs(pid->output)>pid->outputMax))
//        pid->output=-pid->outputMax;
    pid->err[0]=pid->err[1];
    pid->last_time=pid->this_time;
    
}
float outer_last_data=0;
int outer_zero_cnt=0;
void PID_OUTER_Clear()
{
    PROP_OUTER_PID.fdb=0;
    outer_last_data=0;
    outer_zero_cnt=0;
    
}
void PID_OUTER_LOOP()
{
    
    if(Z_EulerAngle-outer_last_data<-pi)
    {
        outer_zero_cnt++;
        PROP_OUTER_PID.fdb=2*pi*(float)outer_zero_cnt+Z_EulerAngle;
    }
    else if(Z_EulerAngle-outer_last_data>pi)
    {
        outer_zero_cnt--;
        PROP_OUTER_PID.fdb=2*pi*(float)outer_zero_cnt+Z_EulerAngle;
    }
    else 
    {
        if(PROP_OUTER_PID.fdb>pi)
            PROP_OUTER_PID.fdb=PROP_OUTER_PID.fdb+(Z_EulerAngle-outer_last_data);
        else if(PROP_OUTER_PID.fdb<-pi)
            PROP_OUTER_PID.fdb=PROP_OUTER_PID.fdb+(Z_EulerAngle-outer_last_data);
        else if((PROP_OUTER_PID.fdb==pi)||(PROP_OUTER_PID.fdb==-pi))
            return;
        else  
            //return;
            PROP_OUTER_PID.fdb=Z_EulerAngle;
    }
    outer_last_data=Z_EulerAngle;
    //PROP_OUTER_PID.ref=X_Euler_reference+IMU_Shift;
    PROP_OUTER_PID.ref=Z_Euler_reference-IMU_Shift;
    //PROP_OUTER_PID.fdb=Z_EulerAngle;
    PROP_OUTER_PID.Calc(&PROP_OUTER_PID);
}
void PID_INNER_LOOP()
{
    
    PROP_INNER_PID.ref=PROP_OUTER_PID.output;
    PROP_INNER_PID.fdb=Z_Gyro;
    PROP_INNER_PID.Calc(&PROP_INNER_PID);
}
void Safety_Check_Loop( float left_power, u16 left_temp, float right_power,u16 right_temp)
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
