#ifndef _PROPELLER_CONTROL_H_
#define _PROPELLER_CONTROL_H_
#include "main.h"

//outer loop pid init structure 

void Balance_Check(void);
void Propeller_Control(void);
void Propeller_Init(void);
void IMU_Shift_Cali(void);
int ilimit(int input,int min, int max);
float flimit(float input,float min, float max);
extern u8 prop_init_flag;
extern u8 prop_initing_flag;
extern u8 Balance_Stable;
extern u8 prop_safe_start;
extern float Z_Euler_reference;
extern float IMU_Shift;
#endif
