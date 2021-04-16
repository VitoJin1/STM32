#ifndef _ROLLER_CONTROL_H_
#define _ROLLER_CONTROL_H_
#include "main.h"
#define Roller_init_delay 30 //means roller init signal needs to sustain 15*50ms 
extern int roller_change_cnt;
void Roller_Motor_Control(void);
void Roller_init(void);
void Roller_ESC_Init_Detect(void);
#endif
