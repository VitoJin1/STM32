#include "encoder_read.h"
volatile int round_left=0;
volatile int round_right=0;
u16 encoder_left=0;
u16 encoder_right=0;
double Left_length_fdb=0;
double Right_length_fdb=0;
int Left_Speed_fdb=0;
int Right_Speed_fdb=0;
u16 last_left=0;
u16 last_right=0;
u8 pass_origin_left_flag=0;
u8 pass_origin_right_flag=0;
static  union
{
    char bytes[8];
    float decodedata[2];
}send_message;

void encoder_read(void)
{   
    u8 t=0;
    last_left=encoder_left;
    last_right=encoder_right;
    encoder_left=TIM4->CNT;
    encoder_right=TIM5->CNT;
   
    
    Left_Speed_fdb =(int)encoder_left- (int)last_left;
    Right_Speed_fdb=(int)encoder_right-(int)last_right;
    if( ABS(Left_Speed_fdb)>=0.5*Encoder_left_count)// over max speed -> pass the origin
    {
        if(Left_Speed_fdb>0)
        {
            Left_Speed_fdb=Left_Speed_fdb-Encoder_left_count;
            
        }
        else
        {
            Left_Speed_fdb=Left_Speed_fdb+Encoder_left_count;
            
        }
    }
    if(ABS(Right_Speed_fdb)>=0.5*Encoder_right_count)
    {
        if(Right_Speed_fdb>0)
        { 
            Right_Speed_fdb=Right_Speed_fdb-Encoder_right_count;
           
        }
        else 
        {    
            Right_Speed_fdb=Right_Speed_fdb+Encoder_right_count;
           
        }
    }
    Left_length_fdb=Left_length_fdb+(double)Left_Speed_fdb*Pully_Primeter/Encoder_left_count;
    Right_length_fdb=Right_length_fdb-(double)Right_Speed_fdb*Pully_Primeter/Encoder_right_count;
    if(MODE==CALI_MODE)
    {
    send_message.decodedata[0]=Left_length_fdb;
    send_message.decodedata[1]=Right_length_fdb;
    USART_SendData(USART1, 0x3a);
    while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
    for(t=0;t<8;t++)
    {
        USART_SendData(USART1, send_message.bytes[t]);
        while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
    }
    USART_SendData(USART1, 0x0d);
    while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
    USART_SendData(USART1, 0x0a);
    while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
    }
    
    
    
    
    
    
    /*
    if(round_left==0)
        Left_length_fdb=Left_length_fdb+(double)Left_Speed_fdb*Pully_Primeter/Encoder_left_count;
    else if(round_left>0)
        Left_length_fdb=round_left*Pully_Primeter+(float)encoder_left/Encoder_left_count*Pully_Primeter;
    else if(round_right<0)
        Left_length_fdb=(round_left-1)*Pully_Primeter+(float)encoder_left/Encoder_left_count*Pully_Primeter;
    
    if(round_right==0)
        Right_length_fdb=Right_length_fdb+Right_Speed_fdb;
    else if(round_right>0)
        Right_length_fdb=round_right*Pully_Primeter+(float)encoder_right/Encoder_right_count*Pully_Primeter;
    else if(round_right<0)
        Right_length_fdb=round_right*Pully_Primeter-(1-(float)encoder_right/Encoder_right_count)*Pully_Primeter;
    */
    
    
    /*if(round_left>0||(round_left==0&&Left_Speed_fdb>0))
        Left_length_fdb=round_left*Pully_Primeter+(float)encoder_left/Encoder_left_count*Pully_Primeter;//mm
    else if (round_left<0||(round_left==0&&Left_Speed_fdb<0))
        Left_length_fdb=round_left*Pully_Primeter-(1-(float)encoder_left/Encoder_left_count)*Pully_Primeter;//mm
    
    
    if(round_right>0||(round_right==0&&Right_Speed_fdb>0))
        Right_length_fdb=round_right*Pully_Primeter+(float)encoder_right/Encoder_right_count*Pully_Primeter;//mm
    else if (round_right<0||(round_right==0&&Right_Speed_fdb<0))
        Left_length_fdb=round_right*Pully_Primeter-(1-(float)encoder_right/Encoder_right_count)*Pully_Primeter;//mm
    */
    
    
    //Right_length_fdb=round_right*Pully_Primeter+(float)encoder_right/Encoder_right_count*Pully_Primeter;//mm
    
    //Left_length_fdb=round_left;
    //Right_length_fdb=round_right;
}


