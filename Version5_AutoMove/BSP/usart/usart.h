#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "main.h"
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART4_RX 			1		//使能（1）/禁止（0）串口1接收
#define XY_OFFSET 10
#define LR_OFFSET 1000
#define RoLLER_OFFSET 1000
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern u8 COMMUNICATION_FLAG;
extern u8 SBUS_DATA_Cache[44];
extern u8 IMU_DATA_Cache[91];
//如果想串口中断接收，请不要注释以下宏定义
void USART1_Init(u32 bound);
void uart3_init(u32 bound);
void UART4_Init(u32 bound);
void uart4_init(u32 bound);
void RemoteDataProcess(uint8_t *pData);
extern float num1;
extern float num2;  
extern float left_prop;
extern float right_prop;

extern float prop_stop;

typedef struct
{
    int16_t ch0;
    int16_t ch1;
    int16_t ch2;
    int16_t ch3;
    int16_t valve_switch;
    int16_t balance_switch;
    int16_t mode_switch;//0--top 1--middle 2--down start with top
    int16_t roller_switch;
    int16_t screw_switch;// 0---middle 1--extend 2--retract start with middle
    int16_t roller_speed;
    int16_t prop_speed;
    int16_t pose;// 0---top 1---middle  2---down start with top 
    int16_t pump_speed;
}Remote;
extern Remote remote_data;
typedef struct 
{
    /*joystick control value (4)*/
    float Left_X_Speed;
    float Left_Y_Speed;
    float Right_X_Speed;
    float Right_Y_Speed;
    
    /*pinometer control value, 4 total, 2 listed */
    int Roller_Speed;
    int Left_Prop_Speed;
    int Right_Prop_Speed;
    int Pump_Speed;
   /*switch control value(14) */
    
    short Roller_Switch;
    short Solenoid_Switch;
    short Balance_Switch;
    short Mode;
    short Arm;
    short Pose;
    
}Message;


typedef struct 
{
    u8 header;
    u8 IMU_ID[2];
    u8 Command[2];
    u8 Data_Length[2];
    u8 Time_Stamp[4];
    union{
        u8 byte[76];
        float IMU_Data[19];
    }Used_Data;
    u8 CheckSum[2];
    u8 PackEnd[2];
    
}IMU_RawData;
extern IMU_RawData imu_rawdata;

extern int lora_receive_cnt;
extern Message Command;
extern int64_t time_tick_Command;
extern int64_t time_tick_LORA;

#endif


