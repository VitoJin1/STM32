#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "IOInput.h"
#include "timer.h"
#include "iic.h"
#include "led.h"

u8 high_byte=0x00;
u8 low_byte=0x00;

u8 IICCurrentDataCnt1=0;
u8 IICCurrentDataCnt2=0;
int IICCurrentData[8]={12500,12500,12500,12500,12500,12500,12500,12500};

//collect iic data

//notice: in order to  provide acurate analog data, the data you read is based on the your last setting of registor

void IICDataCollect()
{
    
    IIC_Start(); 
    IIC_Send_Byte(0x92);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x01);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x42);
    IIC_Wait_Ack();
    IIC_Send_Byte(0xe3);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x92);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x00);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x93);
    IIC_Wait_Ack();
    high_byte=IIC_Read_Byte(1);
    low_byte=IIC_Read_Byte(1);
    IIC_Stop();
    IICCurrentData[IICCurrentDataCnt1]=high_byte*256+low_byte;
    IICCurrentDataCnt1=0;
    delay_ms(2);
    
    IIC_Start(); 
    IIC_Send_Byte(0x92);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x01);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x52);
    IIC_Wait_Ack();
    IIC_Send_Byte(0xe3);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x92);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x00);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x93);
    IIC_Wait_Ack();
    high_byte=IIC_Read_Byte(1);
    low_byte=IIC_Read_Byte(1);
    IIC_Stop();
    IICCurrentData[IICCurrentDataCnt1]=high_byte*256+low_byte;
    IICCurrentDataCnt1=1;
    delay_ms(2);
    
    IIC_Start(); 
    IIC_Send_Byte(0x92);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x01);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x62);
    IIC_Wait_Ack();
    IIC_Send_Byte(0xe3);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x92);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x00);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x93);
    IIC_Wait_Ack();
    high_byte=IIC_Read_Byte(1);
    low_byte=IIC_Read_Byte(1);
    IIC_Stop();
    IICCurrentData[IICCurrentDataCnt1]=high_byte*256+low_byte;
    IICCurrentDataCnt1=2;
    delay_ms(2);
    
    IIC_Start(); 
    IIC_Send_Byte(0x92);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x01);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x72);
    IIC_Wait_Ack();
    IIC_Send_Byte(0xe3);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x92);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x00);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x93);
    IIC_Wait_Ack();
    high_byte=IIC_Read_Byte(1);
    low_byte=IIC_Read_Byte(1);
    IIC_Stop();
    IICCurrentData[IICCurrentDataCnt1]=high_byte*256+low_byte;
    IICCurrentDataCnt1=3;
    delay_ms(2);
    
    IIC_Start(); 
    IIC_Send_Byte(0x90);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x01);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x42);
    IIC_Wait_Ack();
    IIC_Send_Byte(0xe3);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x90);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x00);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x91);
    IIC_Wait_Ack();
    high_byte=IIC_Read_Byte(1);
    low_byte=IIC_Read_Byte(1);
    IIC_Stop();
    IICCurrentData[IICCurrentDataCnt2+4]=high_byte*256+low_byte;
    IICCurrentDataCnt2=0;
    delay_ms(2);
    
    IIC_Start(); 
    IIC_Send_Byte(0x90);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x01);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x52);
    IIC_Wait_Ack();
    IIC_Send_Byte(0xe3);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x90);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x00);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x91);
    IIC_Wait_Ack();
    high_byte=IIC_Read_Byte(1);
    low_byte=IIC_Read_Byte(1);
    IIC_Stop();
    IICCurrentData[IICCurrentDataCnt2+4]=high_byte*256+low_byte;
    IICCurrentDataCnt2=1;
    delay_ms(2);
    
    IIC_Start(); 
    IIC_Send_Byte(0x90);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x01);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x62);
    IIC_Wait_Ack();
    IIC_Send_Byte(0xe3);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x90);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x00);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x91);
    IIC_Wait_Ack();
    high_byte=IIC_Read_Byte(1);
    low_byte=IIC_Read_Byte(1);
    IIC_Stop();
    IICCurrentData[IICCurrentDataCnt2+4]=high_byte*256+low_byte;
    IICCurrentDataCnt2=2;
    delay_ms(2);
    
    IIC_Start(); 
    IIC_Send_Byte(0x90);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x01);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x72);
    IIC_Wait_Ack();
    IIC_Send_Byte(0xe3);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x90);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x00);
    IIC_Wait_Ack();
    IIC_Stop();
    IIC_Start();
    IIC_Send_Byte(0x91);
    IIC_Wait_Ack();
    high_byte=IIC_Read_Byte(1);
    low_byte=IIC_Read_Byte(1);
    IIC_Stop();
    IICCurrentData[IICCurrentDataCnt2+4]=high_byte*256+low_byte;
    IICCurrentDataCnt2=3;
    delay_ms(2);
    
}



void UartSendData2Lora(int * pAnalogData, u8 * pDigitalData)
{
    
    u8 Data2Send;
    u8 i;
   
    USART_SendData(USART1,0x0a);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    
    USART_SendData(USART1,0x0d);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    
    for(i=0;i<8;i++){
        Data2Send=pAnalogData[i]>>8;
        USART_SendData(USART1,Data2Send);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        
        Data2Send=pAnalogData[i]&0xff;
        USART_SendData(USART1,Data2Send);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        
    }
    
    Data2Send=pDigitalData[0]*128+pDigitalData[1]*64+pDigitalData[2]*32+pDigitalData[3]*16+pDigitalData[4]*8+pDigitalData[5]*4+pDigitalData[6]*2+pDigitalData[7]*1;
    USART_SendData(USART1,Data2Send);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    
    Data2Send=pDigitalData[8]*128+pDigitalData[9]*64+pDigitalData[10]*32+pDigitalData[11]*16+pDigitalData[12]*8+pDigitalData[13]*4+1*2+1*1;
    USART_SendData(USART1,Data2Send);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    USART_SendData(USART1,0x0d);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    
    USART_SendData(USART1,0x0a);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    
}
int main(void)
{		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init( );	  
    IOInit();  	
    LED_Init();
	uart_init(115200);
    IIC_Init();
    TIM3_Int_Init(999,71);    
//    delay_ms(2000);
  while(1){
    IICDataCollect();
    /*
    for(i=0;i<8;i++){
        printf("%d=%d  ",i,IICCurrentData[i]);
    }。。。。。、、
    for(i=0;i<14;i++){
        printf("IO =%d ",IOStatus[i]);
    }
    printf("\r\n");
    */
    UartSendData2Lora(IICCurrentData,IOStatus);
    delay_ms(20);
    
	}	 
} 

