#ifndef _ENCODER_READ_H_
#define _ENCODER_READ_H_
#include "main.h"
//#define Pully_Primeter 50*pi
#define Pully_Primeter 43*pi//42+1
extern double Left_length_fdb;
extern double Right_length_fdb;
extern volatile int round_left;
extern volatile int round_right;
extern int Left_Speed_fdb;
extern int Right_Speed_fdb;

extern u8 pass_origin_left_flag;
extern u8 pass_origin_right_flag;
void encoder_read(void);
#endif
