#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_
#include "main.h"
#define Trig1 PEout(8)
#define Trig2 PEout(10)
#define Trig3 PEout(12)
#define Trig4 PEout(14)
void Trig_Init(void);
void Trig1_Enable(void);
void TIM1_CH1_Cap_Init(u32 arr,u16 psc);
extern  int distance;
extern int propeller_stop_flag;
extern int ultraHeightAlert;
void Ultrasonic_Detect_Task(int ultrasonic_count);
void Ultrasonic1_Init(void);
void Beep_200ms(void);
#endif
