#ifndef __M3508_H
#define __M3508_H
#include "sys.h"
#define STANDBY_MODE 0
#define HOLD_MODE   1
#define PROTECT_MODE 2

#define PI 3.1415926
/*
slope: design stage: 3s up to max speed(9120) and current(16384);
when 1khz 1ms control interval, slope of speed loop=27,  slope of current=48

*/
typedef struct PID_Regulator
{
    int16_t ref;
    int16_t fdb;
    int16_t err[2];
    int16_t target;
    float kp;
    float ki;
    float kd;
    float componentP;
    float componentI;
    float componentD;
    float componentPMax;
    float componentIMax;
    float componentDMax;
    float output;
    float outputMax;
    float outputlast;
    float slope;//max difference between each control loop 
    void (*Calc)(struct PID_Regulator * pid);
}PID_Regulator;

void CURRENT_PID_Calc(PID_Regulator *pid);
void SPEED_PID_Calc(PID_Regulator * pid);
void dectect_refference(void);
/*PID parameter Intergrade 
{
    ref,fdb,{last_err,current_err},target,\
    kp,ki,kd\
    componentP,componentI,componentD,\
    componentPmax,componentImax,componentDmax,\
    ouput,outputmax,outputmaxlast,\
    slope\
    calculate function poninter\
}
*/

#define M3508_CURRENT_PID_Tight \
{\
    0,0,{0,0},0,       \
    0.8,0.08,0,        \
    0,0,0,             \
    6000,4000,400,     \
    0,6000,0,          \
    6000,              \
    &CURRENT_PID_Calc, \
}\
#define M3508_SPEED_PID_Tight \
{\
    0,0,{0,0},1000,   \
    10,0,5,           \
    0,0,0,            \
    15000,3000,10000, \
    0,6000,0,         \
    8000,             \
    &SPEED_PID_Calc,  \
}\



#define M3508_CURRENT_PID_Normal \
{\
    0,0,{0,0},0,       \
    0.8,0.08,0,        \
    0,0,0,             \
    6000,4000,400,     \
    0,3000,0,          \
    6000,              \
    &CURRENT_PID_Calc, \
}\
#define M3508_SPEED_PID_Normal \
{\
    0,0,{0,0},1000,   \
    10,0,5,           \
    0,0,0,            \
    15000,3000,10000, \
    0,3000,0,         \
    8000,             \
    &SPEED_PID_Calc,  \
}\




#define M3508_CURRENT_PID_Loose \
{\
    0,0,{0,0},0,       \
    0.8,0.08,0,        \
    0,0,0,             \
    6000,4000,400,     \
    0,1000,0,          \
    6000,              \
    &CURRENT_PID_Calc, \
}\
#define M3508_SPEED_PID_Loose \
{\
    0,0,{0,0},1000,   \
    10,0,5,           \
    0,0,0,            \
    15000,3000,10000, \
    0,1000,0,         \
    8000,             \
    &SPEED_PID_Calc,  \
}\




#define M3508_CURRENT_PID_Init \
{\
   0,0,{0,0},0,\
    0.7,0.04,0,\
    0,0,0,\
    10000,8000,0,\
    0,0,0,\
    200,\
    &CURRENT_PID_Calc,\
}\

#define M3508_CURRENT_PID_WIND \
{\
    0,0,{0,0},0,\
    0.7,0.04,0,\
    0,0,0,\
    10000,8000,0,\
    0,12000,0,\
    200,\
    &CURRENT_PID_Calc,\
}\

#define M3508_SPEED_PID_Init \
{\
    0,0,{0,0},0,\
    7,0.02,0,\
    0,0,0,\
    8000,3000,0,\
    0,0,0,\
    200,\
    &SPEED_PID_Calc,\
}\

#define M3508_SPEED_PID_WIND \
{\
    0,0,{0,0},0,\
    2.5,0.12,1,\
    0,0,0,\
    2000,900,1000,\
    0,1500,0,\
    4,\
    &SPEED_PID_Calc,\
}\



extern PID_Regulator M3508_Current_PID;
extern PID_Regulator M3508_Speed_PID;
extern u8 Break_Protect;
void m3508_current_control(void);
void m3508_speed_control(void);
void M3508_Torque_Change(void);
typedef struct{
    float Vi;
    float Vo_last;
    float Vo;
    float Fcutoff;
    float Fs;
}LPF_1orderRC_F;

void LPF_1orderRC_F_init( LPF_1orderRC_F *v);
extern LPF_1orderRC_F current_filter;













#endif
