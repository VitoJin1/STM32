#ifndef _STEP_MOTOR_CONTROL_H_
#define _STEP_MOTOR_CONTROL_H_
#include "main.h"
#define STEP_MOTOR_DIR PEout(4)
void Step_Motor_Move(int dir);
void Step_Motor_Dir_Init(void);
void Step_Motor_Control(void);
void Valve_Control(void);

void Valve_Control_Init(void);
extern u8 roller_init_flag;
extern u8 roller_initing_flag;
#endif
