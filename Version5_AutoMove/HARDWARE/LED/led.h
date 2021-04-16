#ifndef __LED_H
#define __LED_H
#include "sys.h"
#include "main.h"
//LED端口定义
#define LED_R PCout(0)	// DS0
#define LED_G PCout(1)	// DS1	 
#define LED_B PCout(2) 
#define LED_Y PCout(3)
#define ROLLER_Switch PCout(4)
#define ProPLeft_Switch PCout(5)
#define ProPRight_Switch PCout(6)

#define WsDat_Dark   0x000000 
#define WsDat_Green   0x0000ff
#define WsDat_Red  0xff0000  
#define WsDat_Blue    0x00ff00
#define WsDat_Yellow (WsDat_Red+WsDat_Green)
#define WsDat_White  0xffffff
#define WsDat_Pink (WsDat_Red+WsDat_Blue)
#define WsDat_Org 0xff0080
#define WsDat_Brown 0x99004c

extern unsigned long last_LEDBanddata_left;
extern unsigned long last_LEDBanddata_right;

void LED_Init(void);//初始化
void LargeCurrentSwitch(void);
void LEDBand_Init(void);
void wsSetAllChip(unsigned long dat1, unsigned long dat2,unsigned long dat3, unsigned long dat4);
unsigned long  wsSetColor(unsigned long Rval,unsigned long Gval,unsigned long Bval);
void wsSetOneChip(unsigned long dat);
void TX1(void) ;
void TX0(void) ;
int float2RGB(float input_min,float input_max,float input_value);
void LEDBand_ShowSignal(float left_power,float left_tempeature,float right_power,float right_tempeature);
void SetLEDBand(unsigned long data1,unsigned long data2);
#endif
