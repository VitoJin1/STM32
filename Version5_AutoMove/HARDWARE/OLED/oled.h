#ifndef __OLED_H
#define __OLED_H		
#include "main.h"

/*
// connection 
//     GND------0V
//     VCC------5V/3.3V
//     D0(PD10)------SCL
//     D1(PD11)-------SDA
//     RES(PD12)------Reset select pin
//     DC(PD13)-------Data/Command select pin
//     CS-------GND(only have one oled, connect to gnd will save one IO)
*/





#define OLED_RST_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_12)
#define OLED_RST_Set() GPIO_SetBits(GPIOD,GPIO_Pin_12)

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_10)
#define OLED_SCLK_Set() GPIO_SetBits(GPIOD,GPIO_Pin_10)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_11)
#define OLED_SDIN_Set() GPIO_SetBits(GPIOD,GPIO_Pin_11)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_13)
#define OLED_DC_Set() GPIO_SetBits(GPIOD,GPIO_Pin_13)

#define OLED_CMD  0 //write command
#define OLED_DATA 1//write data


void OLED_ColorTurn(u8 i);
void OLED_ClearPoint(u8 x,u8 y);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1);
void OLED_ShowString(u8 x,u8 y, char *chr,u8 size1);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1);
void OLED_WR_BP(u8 x,u8 y);
void OLED_Init(void);

#endif  
	 
	 



