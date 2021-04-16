#ifndef _PULSE_H_
#define _PULSE_H_
#include "main.h"
#define LEFT_Dir PBout(13)
#define RIGHT_Dir PBout(14)
struct TwoPointAutoMove{
    int64_t PointA[2];
    int64_t PointB[2];
    int current_step;
    int total_step;
    u8 status;
    int vel_left;
    int vel_right;
    void (*clear)(struct TwoPointAutoMove *p);
};

void ClearData(struct TwoPointAutoMove *p);


void TIM10_Freq_Config(u32 Cycle);
void TIM11_Freq_Config(u32 Cycle);
void CableMotor_Dir_Init(void);

extern struct TwoPointAutoMove AutoMove;
#endif 
