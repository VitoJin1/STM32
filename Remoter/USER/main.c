#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "IOInput.h"
#include "timer.h"
#include "iic.h"
u8 high_byte=0x00;
u8 low_byte=0x00;

u8 IICCurrentDataCnt1=0;
u8 IICCurrentDataCnt2=0;
int IICCurrentData[8]={0,0,0,0,0,0,0,0};
void IICDataCollect()
{
    u8 HighByte=0x00;
    u8 LowByte =0x00;
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

int main(void)
{		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init( );	  
    IOInit();  	
	uart_init(115200);
    IIC_Init();
    

	
//    TIM3_Int_Init(999,71);    
//    delay_ms(2000);
  while(1)
	{
        
        int i;
        IICDataCollect();
        for(i=0;i<8;i++){
            printf("%d=%d  ",i,IICCurrentData[i]);
        }
        printf("\r\n");
        delay_ms(500);
        
		//UartSendData2Lora(IOStatus);
        //delay_ms(500);
        /*
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
        printf("high=%x low=%x  total1=%d \r\n",high_byte,low_byte,high_byte*256+low_byte);
        IIC_Stop();
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
        printf("high=%x low=%x  total2=%d \r\n",high_byte,low_byte,high_byte*256+low_byte);
        IIC_Stop();
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
        printf("high=%x low=%x  total3=%d \r\n",high_byte,low_byte,high_byte*256+low_byte);
        IIC_Stop();
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
        printf("high=%x low=%x  total3=%d \r\n",high_byte,low_byte,high_byte*256+low_byte);
        IIC_Stop();
        delay_ms(2);
        
        delay_ms(500);
        */
	}	 
} 

