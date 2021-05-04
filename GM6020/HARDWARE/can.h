#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
#include "stm32f10x.h"
#include "stm32f10x_can.h"



static void CAN_GPIO_Config(void);
static void CAN_NVIC_Config(void);
static void CAN_Mode_Config(void);
static void CAN_Filter_Config(void);
void CAN_Config(void);
void CAN_SetMsg(void);
void CAN_DJI_C620_DataSend( int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
void Can_Decode(CanRxMsg * data);

extern int16_t Motor_Angle;
extern int16_t Motor_Speed;
extern int16_t Motor_Torque;
extern int16_t Motor_Temp;

#endif

















