#ifndef _TIMER_H_
#define _TIMER_H_
#include "main.h"

extern u8 SYSTEM_ERROR;

extern u8 ROBOT_UNLOCK;
void TIM3_Int_Init(u16 arr,u16 psc);
void Timer7_Init(void);
void Target_Update(void);
void TIM2_PWM_Init(u32 arr,u32 psc);
void Unlock_Robot_Detect(void);
void TIM8_PWM_Init(u32 arr,u32 psc);
void TIM12_Init(void);
void TIM4_PWM_Init(u16 arr,u16 psc);
void Pump_Init(void);
extern int Unlock_signal_cnt;
#endif
