#include "iic.h"
void IIC_Init(void){			
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    IIC_SCL=1;
    IIC_SDA=1;
}

//IIC start signal 
void IIC_Start(void){
	SDA_OUT();     
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;
	delay_us(4);
	IIC_SCL=0;
}

//IIC stop signal 
void IIC_Stop(void){
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;
	delay_us(4);							   	
}

//wait for ack signal 
u8 IIC_Wait_Ack(void){
	u8 ucErrTime=0;
	SDA_IN();      
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA){
		ucErrTime++;
		if(ucErrTime>250){
           
			IIC_Stop();
			return 1;
		} 
    }
    
	IIC_SCL=0;// ±÷” ‰≥ˆ0 	   
	return 0;  
}
// generate ack signal 
void IIC_Ack(void){
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

//generate nack signal
void IIC_NAck(void){
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

void IIC_Send_Byte(u8 txd){                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;
    for(t=0;t<8;t++){              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }
}

u8 IIC_Read_Byte(unsigned char ack){
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ ){
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)        // send nack signal
        IIC_NAck();
    else
        IIC_Ack();   // send ack signal    
    return receive;
}
