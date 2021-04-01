#ifndef __IOINPUT_H_
#define __IOINPUT_H_
#include "sys.h"
void IOInit(void);
extern u8 IOStatus[14];
#define A0Status GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define A1Status GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define A2Status GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define A3Status GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define A4Status GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define A5Status GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define A6Status GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define A7Status GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#endif