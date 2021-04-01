#ifndef __H_BRIDGE_H_
#define __H_BRIDGE_H_
#include "sys.h"
#define V1Negative  PAout(0)
#define V1Positve   PAout(1)
#define V2Negative  PAout(2)
#define V2Positve   PAout(3)
#define V3Negative  PAout(4)
#define V3Positve   PAout(5)
#define V4Negative  PAout(6)
#define V4Positve   PAout(7)
#define V5Negative  PBout(0)
#define V5Positve   PBout(1)
#define V6Negative  PBout(2)
#define V6Positve   PBout(3)
#define V7Negative  PBout(4)
#define V7Positve   PBout(5)
#define V8Negative  PBout(6)
#define V8Positve   PBout(7)
#define LED_R PBout(15)
#define LED_G PBout(14)
#define LED_B PBout(13)
void HBridgeIOInit(void);
void HbridgeSetting(int num,int status);

#endif 
