#ifndef _CM_CONTROL_H_
#define _CM_CONTROL_H_
#include "main.h"
#define CW (-1)
#define CCW 1
//#de _Up 0.252//asin(0.25)----dead zone is 14.43, max tension is 2G 
#define Dead_Zone_Top 1000
#define Dead_Zone_Side 500
#define pi acos(-1)
#define Cable_Max_Speed 119.0// cable motor max speed pi*d (1khz,400 digital gear ratio,25 gear box ratio) 
#define Initial_C 4050
#define Initial_Left 2214
#define Initial_Right 2259
#define Rotate_Matrix 0.0
#define SBUS_Updata_Loop 25//25ms update sbus receiver
#define Ultrasonic_detect_Loop 200//check the height of bottom every 1s 
#define Unlock_Robot_Loop 100
#define Balance_check_Loop 100
#define IMU_Shift_Calibration_Loop 100//100ms manual calibrate IMU 
#define PROP_OUTER_LOOP 100
#define PROP_INNER_LOOP 50
#define PN008 100
#define IMU_Updata_Loop 20 //20ms update IMU message
#define Cable_Control_Loop 100// 10ms control cable position

#define Step_Motor_Control_Loop 100// 100ms control loop
#define Propeller_Control_Loop 50//50ms control loop
#define Valve_Control_Loop 500
#define Roller_Control_Loop 200
#define Encoder_Read_Loop 35
#define Command_Disconnect_Loop 200
//PID parameter fix*************************************
#define CM_LEFT_KP 40.0
#define CM_LEFT_KI 0.0
#define CM_LEFT_KD 0
#define CM_RIGHT_KP 40.0
#define CM_RIGHT_KI 0.0
#define CM_RIGHT_KD 0
#define CM_LEFT_OUTPUTMAX 1500
#define CM_RIGHT_OUTPUTMAX 1500
#define CM_LEFT_RAMP 30
#define CM_RIGHT_RAMP 30


#define CM_LEFT_KP_AUTO 8.0
#define CM_LEFT_KI_AUTO 0.03
#define CM_LEFT_KD_AUTO 0
#define CM_RIGHT_KP_AUTO 8.0
#define CM_RIGHT_KI_AUTO 0.03
#define CM_RIGHT_KD_AUTO 0
#define CM_LEFT_OUTPUTMAX_AUTO 1500
#define CM_RIGHT_OUTPUTMAX_AUTO 1500
/********************************************************/
#define PROP_ANGLE_IMAX 100
#define PROP_GYRO_IMAX 100
#define PROP_ANGLE_OUTPUT_MAX 3
#define PROP_GYRO_OUTPUT_MAX 200



//*******************************************************
typedef struct PID_Regulator
{
    float ref;
    float fdb;
    float err[2];
    float kp;
    float ki;
    float kd;
    float componentKp;
    float componentKi;
    float componentKd;
    float componentKpMax;
    float componentKiMax;
    float componentKdMax;
    float output;
    float outputMax;
    float outputLast;
    void (*Calc)( struct PID_Regulator *pid);//function pointer, C don't allow00
                                           //function as struct member
    float ramp;
    int64_t this_time;
    int64_t last_time;
}PID_Regulator;
void PID_Calc(PID_Regulator *pid);

#define CM_LEFT_PID_Init \
{\
  0,0,{0,0},\
  CM_LEFT_KP,CM_LEFT_KI,CM_LEFT_KD,\
  0,0,0,\
  1500,17500,0,\
  0,CM_LEFT_OUTPUTMAX,0,\
  &PID_Calc,\
  CM_LEFT_RAMP,\
}\

#define CM_RIGHT_PID_Init \
{\
  0,0,{0,0},\
  CM_RIGHT_KP,CM_RIGHT_KI,CM_RIGHT_KD,\
  0,0,0,\
  1500,17500,0,\
  0,CM_RIGHT_OUTPUTMAX,0,\
  &PID_Calc,\
  CM_RIGHT_RAMP,\
}\

#define CM_LEFT_PID_AUTO_Init \
{\
  0,0,{0,0},\
  CM_LEFT_KP_AUTO,CM_LEFT_KI_AUTO,CM_LEFT_KD_AUTO,\
  0,0,0,\
  1500,17500,0,\
  0,CM_LEFT_OUTPUTMAX_AUTO,0,\
  &PID_Calc,\
  CM_LEFT_RAMP,\
}\

#define CM_RIGHT_PID_AUTO_Init \
{\
  0,0,{0,0},\
  CM_RIGHT_KP_AUTO,CM_RIGHT_KI_AUTO,CM_RIGHT_KD_AUTO,\
  0,0,0,\
  1500,17500,0,\
  0,CM_RIGHT_OUTPUTMAX_AUTO,0,\
  &PID_Calc,\
  CM_RIGHT_RAMP,\
}\


#define PROP_ANGLE_LOOP_PID \
{\
  0,0,{0,0},\
  1.5,0,0.0,\
  0,0,0,\
  500000,0,500000,\
  0,PROP_ANGLE_OUTPUT_MAX,0,\
  &PropPIDOutterCalc,\
  CM_LEFT_RAMP,\
  0,0,\
}\

//innner loop pid init structure

//250,0,50 start-----100,0,0

#define PROP_GYRO_LOOP_PID \
{\
  0,0,{0,0},\
  100,0,0,\
  0,0,0,\
  500000,0,500000,\
  0,PROP_GYRO_OUTPUT_MAX,0,\
  &PropPIDInnnerCalc,\
  CM_LEFT_RAMP,\
  0,0,\
}\

extern PID_Regulator CM_LEFT_PID;
extern PID_Regulator CM_RIGHT_PID;



extern int64_t Last_Left_set;
extern int64_t Last_Right_set;
extern int64_t Length_Right_set;
extern int64_t Length_Left_set;

extern int accumulate_hor;
extern int accumulate_vel;
extern int64_t left_side;
extern int64_t right_side;
extern int64_t x_move;
extern int64_t y_move;

extern uint64_t time_tick_1ms;
extern int calc_count;
extern float rand_number_left;
extern float rand_number_right;
extern int output_l;
extern int output_r;
extern int Safety_Flag;

void PID_Loop(void);
void CableMotorControlLoop(void);
void Movement_Calc(float Left_Right,float  Up_Down,long c, long left,long right,double theta);
void Absolute_Calc(float Left_Right,float Up_Down,long c, long left,long right,double theta);
void Safety_Check_Loop(float left_power, u16 left_temp, float right_power,u16 right_temp);
void PID_OUTER_LOOP(void);
void PID_INNER_LOOP(void);
void PropPIDInnnerCalc(PID_Regulator *pid);
void PropPIDOutterCalc(PID_Regulator *pid);
extern  PID_Regulator PROP_OUTER_PID;
extern  PID_Regulator PROP_INNER_PID;
void Prop_PID_Calc(  PID_Regulator *pid);
void PID_OUTER_Clear(void);
#endif

