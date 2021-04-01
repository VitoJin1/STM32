#include "H_bridge.h"
void HBridgeIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);	
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
    GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}
void HbridgeSetting(int num,int status)
{
    if(num==0){    
        switch(status){
            case 1:
                V1Positve= 1;
                V1Negative=0;
                break;
            case 0:
                V1Positve =0;
                V1Negative=0;
                break;
            case -1:
                V1Positve= 0;
                V1Negative=1;
                break;
        }
    }
    
    if(num==1){
        switch(status){
        case 1:
            V2Positve= 1;
            V2Negative=0;
            break;
        case 0:
            V2Positve =0;
            V2Negative=0;
            break;
        case -1:
            V2Positve= 0;
            V2Negative=1;
            break;
        }
    }
    
    if(num==6){
        switch(status){
        case 1:
            V3Positve= 1;
            V3Negative=0;
            break;
        case 0:
            V3Positve =0;
            V3Negative=0;
            break;
        case -1:
            V3Positve= 0;
            V3Negative=1;
            break;
        }
    }
    
    if(num==7){
        switch(status){
        case 1:
            V4Positve= 1;
            V4Negative=0;
            break;
        case 0:
            V4Positve =0;
            V4Negative=0;
            break;
        case -1:
            V4Positve= 0;
            V4Negative=1;
            break;
        }
    }
    
    if(num==5){
        switch(status){
        case 1:
            V5Positve= 1;
            V5Negative=0;
            break;
        case 0:
            V5Positve =0;
            V5Negative=0;
            break;
        case -1:
            V5Positve= 0;
            V5Negative=1;
            break;
        }
    }
    
     if(num==4){
        switch(status){
        case 1:
            V6Positve= 1;
            V6Negative=0;
            break;
        case 0:
            V6Positve =0;
            V6Negative=0;
            break;
        case -1:
            V6Positve= 0;
            V6Negative=1;
            break;
        }
    }
     
       if(num==3){
        switch(status){
        case 1:
            V7Positve= 1;
            V7Negative=0;
            break;
        case 0:
            V7Positve =0;
            V7Negative=0;
            break;
        case -1:
            V7Positve= 0;
            V7Negative=1;
            break;
        }
    }
       
     if(num==2){
        switch(status){
        case 1:
            V8Positve= 1;
            V8Negative=0;
            break;
        case 0:
            V8Positve =0;
            V8Negative=0;
            break;
        case -1:
            V8Positve= 0;
            V8Negative=1;
            break;
        }
    }
}