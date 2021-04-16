#ifndef _MODBUS_H_
#define _MODBUS_H_
#include "main.h"
#define EN_USART2_RX 1
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define READ_HOLD_REG  03
#define SET_SIHGLE_REG 06
#define SET_MULT_REG   10
#define CIRCUS_CHECK   08
#define HIGH(n)  ((n)>>8)
#define LOW(n)    ((n)&0xff)
#define RS485_FrameDistance 2 // unit:ms
#define Cable_Motor_Left_Address 0x01
#define Cable_Motor_Right_Address 0x02
#define Cable_Motor_Speed_Address 0x301

#define Max_Power_Show_Address 0x201b
#define IGBT_Temperature_Address 0x2025
#define Torque_Address 0x2002
#define Cable_Wind_Speed (1500)
#define Cable_Manual_Wind_Speed (3000)
#define MODBUS_RECEIVE_BUF 40
#define Modbus_Max_Power    65
#define Modbus_Max_Temp     80

void RS485_Init(void);
u16 CRC_Compute(u8 *puchMsg, u16 usDataLen);

void RS485_RX_Check(void);
void RS485_SendData(u8 *buff,u8 len);
void Master_03_Slove( u8 board_adr,u16 start_address,u16 lenth );
void Master_06_Slove(u8 board_adr,u16 start_address, int16_t value);
void MODBUS_Data_Process(uint8_t *pData);
extern u8 RS485_RX_Flag;
extern u8 RS485_TX_Flag;
void TIM14_Init(void);
void TIM13_Init(void);
extern u8 ERROR_CODE;
extern u8 SlaveAddress;
extern u8 Function;
extern u16 StartAddress;
extern int16_t ValueOrLength;
extern  int Max_Power_Left;
extern int Max_Power_Right;
extern int Max_Power_Left_Count1;
extern int Max_Power_Left_Count2;
extern u8 MODBUS_DATA_Cache[40];
extern float left_realtime_power;
extern float right_realtime_power;
extern u16 left_realtime_temp;
extern u16 right_realtime_temp;
typedef struct Error_Type
{
    float count;
    float rate;
}Error_Type;
float fPower_decode(u8 *pData);
u16 iTempeature_decode(u8 *pData);
#endif
