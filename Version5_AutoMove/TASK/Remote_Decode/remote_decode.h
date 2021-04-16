#ifndef __REMOTE_DECODE_H
#define __REMOTE_DECODE_H
#include "main.h"

void Remote_to_Message(void);
     
float fRamp_for_LORA(int16_t temp);
short sRamp_for_LORA(int16_t temp);
int iPwn_for_LORA(int16_t temp);
#endif 

