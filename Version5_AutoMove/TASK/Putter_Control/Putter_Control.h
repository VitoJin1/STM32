#ifndef _PUTTER_CONTROL_H_
#define _PUTTER_CONTROL_H_
#include "main.h"

#define Putter_IN1 PDout(4)
#define Putter_IN2 PDout(5)

#define Putter_IN3 PDout(4)
#define Putter_IN4 PDout(5)
//#define Fan_Control PEout(3)
void Putter_Init(void);
void Putter_Control_Loop(void);
void FanControl(u8 controlvalue);
void FanControlLoop(void);
extern u8 Putter_Value;
void fun_test(u8 mode,short signal );
#endif
