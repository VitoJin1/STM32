#include "modbus.h"
u32 RS485_Baudrate=115200;//communication baudrate
u8 RS485_Parity=0;// 0: None  1:Odd 2: Even Check


u8 RS485_RX_BUFF[200];//receive buffer
u16 RS485_RX_CNT=0;//receive frame count
u8 RS485_RX_Flag=0;//receive flag

u8 RS485_TX_BUFF[200];//transmit buffer
u16 RS485_TX_CNT=0;// transmit count
u8 RS485_TX_Flag=1;//transmit flag
u8 RS485_TX_EN=1;
u8 errpace=0;// 1:too much wait on serial port, drop this frame
//2: noise dectected, frame error ot check error
u8 TX_RX_Set=0;// 0:TX 1:RX

u8 SlaveAddress=0x01;
u8 Function=0x03;
u16 StartAddress=0x301;
int16_t ValueOrLength=0x01;
u8 ERROR_CODE=0;//RS485 error type 
u8 MODBUS_DATA_Cache[40];
void RS485_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
    
   	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10复用为USART1
	
	//USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = RS485_Baudrate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	//USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    switch(RS485_Parity)
    {
            case 0:USART_InitStructure.USART_Parity=USART_Parity_No;break;//无校验
            case 1:USART_InitStructure.USART_Parity=USART_Parity_Odd;break;//奇校验
            case 2:USART_InitStructure.USART_Parity=USART_Parity_Even;break;//偶校验
    }
    USART_Init(USART2, &USART_InitStructure); //初始化串口1
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
    USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	//Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
    USART_Cmd(USART2, ENABLE);  //使能串口1 
    //TIM14_Init();
    
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
    DMA_DeInit(DMA1_Stream5);
    while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)MODBUS_DATA_Cache;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
    DMA_InitStructure.DMA_BufferSize = MODBUS_RECEIVE_BUF;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream5, ENABLE);
}

u8 RS485_Receive[7];
u8 RS485_index=0;
int Max_Power_Left=0;
int Max_Power_Right=0;
int Max_Power_Left_Count1=0;
int Max_Power_Left_Count2=0;
u8 RS485Receive_STA=0x00;
u8 RS485Receive_BUF[4];
u8 RS485RL_Flag=0;
volatile u16 receive_len=0;
int modbusReceive_cnt=0;
void USART2_IRQHandler(void)//串口2中断服务程序
{
 u8 clear_flag=0;
    
//    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
//    {
//        res=USART_ReceiveData(USART2);
//       
//    }
    if(USART_GetITStatus(USART2,USART_IT_IDLE)!=RESET)
    {
        modbusReceive_cnt=40-DMA_GetCurrDataCounter(DMA1_Stream5);
        //printf("r:%d\r\n",modbusReceive_cnt);
        DMA_Cmd(DMA1_Stream5,DISABLE);
        clear_flag=USART2->SR;
        clear_flag=USART2->DR;
        DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5|DMA_FLAG_FEIF5|DMA_FLAG_DMEIF5|DMA_FLAG_TEIF5|DMA_FLAG_HTIF5);
        if(7==modbusReceive_cnt)//0x03 function code will receive 7 bytes echo, 0x06 will receive 8 bytes
        {
            MODBUS_Data_Process(MODBUS_DATA_Cache);
        }
        DMA_SetCurrDataCounter(DMA1_Stream5,40);
        
        DMA_Cmd(DMA1_Stream5,ENABLE);
        //count=MODBUS_RECEIVE_BUF-DMA_GetCurrDataCounter(DMA1_Stream5);
        USART_ClearITPendingBit(USART2,USART_IT_IDLE);
        
//        printf("count=%d\r\n",receive_len);
//        receive_len=0;
//        for(i=0;i<8;i++)
//        {
//            USART_SendData(USART1,MODBUS_DATA_Cache[i]);
//            while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//        }
//        USART_SendData(USART1,0xaa);
//        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//        USART_SendData(USART1,0x0a);
//        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//        
    }
    /*
	 if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
        {
            res=USART_ReceiveData(USART2);
            //USART_SendData(UART4,res);
            if((RS485Receive_STA&0x80)==0)//not finish receiving
            {
                if(res==0x01|res==0x02)
                {
                    
                    RS485Receive_STA|=0x10;
                    if(res==0x01)
                        RS485RL_Flag=0;
                    else if(res==0x02)
                        RS485RL_Flag=1;
                }
                else
                {
                    if(RS485Receive_STA&0x10)
                    {
                        //USART_SendData(UART4,0x01);
                        if(res==0x03)   
                        {
                            RS485Receive_STA|=0x20;
                            //USART_SendData(UART4,0xAA);
                        }
                        else
                        { 
                            //USART_SendData(UART4,res);
                            if(RS485Receive_STA&0x20)
                            {
                                if(res==0x02)
                                {
                                    //USART_SendData(UART4,0x03);
                                    
                                    RS485Receive_STA|=0x40;
                                }
                                else 
                                {
                                   
                                    if(RS485Receive_STA&0x40)
                                {
                                    if(RS485RL_Flag==0)
                                    {
                                        RS485Receive_BUF[RS485_index]=res;
                                        
                                    }
                                    else if(RS485RL_Flag==1)
                                    {
                                        RS485Receive_BUF[RS485_index+2]=res;
                                    }
                                    RS485_index++;
                                    if(RS485_index==2)
                                    {
                                        RS485_index=0;
                                        RS485Receive_STA|=0x80;
                                    }
                                }
                                }   
                            }
                        }
                    }
                }
            }
//            RS485_Receive[RS485_index]=res;
//            Max_Power_Left_Count1++;
//            if(((RS485_Receive[(RS485_index+1)%7]==0x01)||(RS485_Receive[(RS485_index+1)%7]==0x02))&&RS485_Receive[(RS485_index+2)%7]==0x03&&RS485_Receive[(RS485_index+3)]==0x02)
//            {   Max_Power_Left_Count2++;
//                u8 * poniter=&(RS485_Receive[RS485_index+1]);
//                CRC_Result=CRC_Compute(poniter,5);
//                if(((CRC_Result>>8&0xFF)==RS485_Receive[RS485_index+6])&&((CRC_Result&0xFF)==RS485_Receive[RS485_index]))
//                {
//                    if(RS485_Receive[(RS485_index+1)%7]==0x01)
//                    {
//                        Max_Power_Left=((RS485_Receive[(RS485_index+4)%7]>>4)&0x0F)*16*16*16+(RS485_Receive[(RS485_index+4)%7]&0x0F)*16*16+((RS485_Receive[(RS485_index+5)%7]>>4)&0x0F)*16+((RS485_Receive[(RS485_index+5)%7]>>4)&0x0F)*1;
//                        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//                    }
//                    else if(RS485_Receive[(RS485_index+1)%7]==0x02)
//                    {
//                        Max_Power_Right=((RS485_Receive[(RS485_index+4)%7]>>4)&0x0F)*16*16*16+(RS485_Receive[(RS485_index+4)%7]&0x0F)*16*16+((RS485_Receive[(RS485_index+5)%7]>>4)&0x0F)*16+((RS485_Receive[(RS485_index+5)%7]>>4)&0x0F)*1;
//                        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//                    }
//                }
//            }
//            RS485_index=(RS485_index+1)%7;

//            if(RS485_RX_CNT<199)
//                {
//                    RS485_RX_BUFF[RS485_RX_CNT]=res;
//                    RS485_RX_CNT++;
//                    //Command.x_vel++;
//                   
//                    TIM_ClearITPendingBit(TIM14,TIM_IT_Update);//清除定时器溢出中断
//                    TIM_SetCounter(TIM14,0);//当接收到一个新的字节，将定时器7复位为0，重新计时（相当于喂狗）
//                    TIM_Cmd(TIM14,ENABLE);//开始计时
//                }
        }*/
        
        /*
        if(RS485Receive_STA&0x80)//all received
        {
            //USART_SendData(UART4,0x04);
            Max_Power_Left=((RS485Receive_BUF[1]>>4)&0x0F)*16*16*16+(RS485Receive_BUF[1]&0x0F)*16*16+((RS485Receive_BUF[0]>>4)&0x0F)*16+(RS485Receive_BUF[0]&0x0F)*1;
            Max_Power_Right=((RS485Receive_BUF[3]>>4)&0x0F)*16*16*16+(RS485Receive_BUF[3]&0x0F)*16*16+((RS485Receive_BUF[2]>>4)&0x0F)*16+(RS485Receive_BUF[2]&0x0F)*1;                       
            RS485Receive_STA=0x00;
        }*/

}
u8 i=0;
float left_realtime_power=0.0;
float right_realtime_power=0.0;

u16 left_realtime_temp=0;
u16 right_realtime_temp=0;

float left_realtime_torque=0.0;
float right_realtime_torque=0.0;

void MODBUS_Data_Process(uint8_t *pData)
{
    //printf("received\r\n");
    //printf("left=%f,right=%f\r\n",left_realtime_torque,right_realtime_torque);
    if(pData==NULL)
    {
        return ;
    }
    if(((0x01==pData[0])||(0x02==pData[0]))&&0x03==pData[1]&&0x02==pData[2])//confirm it is echo signal 
    {
        if(1==modbus_dectect_flag)
            left_realtime_power=fPower_decode(&(pData[3]));
        else if(2==modbus_dectect_flag)
            right_realtime_power=fPower_decode(&(pData[3]));
        else if(3==modbus_dectect_flag) 
            left_realtime_temp=iTempeature_decode(&(pData[3]));
        else if(4==modbus_dectect_flag)
            right_realtime_temp=iTempeature_decode(&(pData[3]));
        else if(5==modbus_dectect_flag)
        {
            
            left_realtime_torque=fPower_decode(&(pData[3]));
            printf("left=%f",left_realtime_torque);
        }
        else if(6==modbus_dectect_flag)
        {
            right_realtime_torque=fPower_decode(&(pData[3]));
            printf("right=%f\r\n",right_realtime_torque);
        }
            
    }
    else 
    {
        return ;
    }
}
float fPower_decode(u8 *pData)
{
    float result;
    result=0.1*(pData[0]*256+pData[1]);
    if(result>=200)                 //65535/2/10acctually
        result=result-6553.5;
    return result;
}
u16 iTempeature_decode(u8 *pData)
{
    //printf("temp_decode\r\n");
    u16 result;
    result=pData[0]*256+pData[1];
    //if(result>80)
      //  printf("!!!%x!!!%x\r\n",pData[0],pData[1]);
    return result;
}
void RS485_RX_Check(void)
{
 
   #if ENABLE_BLUETOOTH
    if(RS485_RX_CNT==5)
    {   
        RS485_Error=RS485_RX_BUFF[2];
        
        switch(RS485_Error)
        {
            case 0x01: ERROR_CODE=1;//CRC check Wrong
                        printf("ERROR CODE=%d\r\n",ERROR_CODE);
                        break;
            case 0x02: ERROR_CODE=2;//Wrong Function Code
                        printf("ERROR CODE=%d\r\n",ERROR_CODE);
                        break;
            case 0x04: ERROR_CODE=3;//Wrong Data Length/Value
                        printf("ERROR CODE=%d\r\n",ERROR_CODE);
                        break;
            case 0x08: ERROR_CODE=4;//Wrong Reg address
                        printf("ERROR CODE=%d\r\n",ERROR_CODE);
                        break;
            case 0x10: ERROR_CODE=5;//Wrong Reg Value
                        printf("ERROR CODE=%d\r\n",ERROR_CODE);
                        break;
            case 0x20: ERROR_CODE=6;//Wrong Reg Change
                        printf("ERROR CODE=%d\r\n",ERROR_CODE);
                        break;
            case 0x40: ERROR_CODE=7;//EEPROM busy
                        printf("ERROR CODE=%d\r\n",ERROR_CODE);
                        break;
            
        }       
    }
    
    else if(RS485_RX_CNT==8)
    {
        //ERROR_CODE=9;
        printf("Normal\r\n");
    }
    else
    {
        ERROR_CODE=8;//wrong receive bytes count
//        printf("ERROR CODE=%d\r\n",ERROR_CODE);
//        for (i=0;i<RS485_RX_CNT;i++)
//        printf("RS485_Data[%d]=%x",i,RS485_RX_BUFF[i]);
    }
    #endif
    RS485_RX_CNT=0;
    
}


void RS485_SendData(u8 *buff,u8 len)
{ 
		RS485_TX_EN=1;//切换为发送模式
		while(len--)
		{
						while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);//等待发送区为空
						USART_SendData(USART2,*(buff++));
		}
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//等待发送完成
		
}

void Master_03_Slove( u8 board_adr,u16 start_address,u16 lenth )
{ 
	u16 calCRC;
    RS485_TX_BUFF[0] = board_adr;
    RS485_TX_BUFF[1] = READ_HOLD_REG;    //modbus 指令码03
    RS485_TX_BUFF[2] = HIGH(start_address);  
    RS485_TX_BUFF[3] = LOW(start_address); 
    RS485_TX_BUFF[4] = HIGH(lenth);
    RS485_TX_BUFF[5] = LOW(lenth);
    calCRC=CRC_Compute(RS485_TX_BUFF,6);
    RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF[7]=(calCRC)&0xFF;
	RS485_SendData(RS485_TX_BUFF,8);
}

void Master_06_Slove(u8 board_adr,u16 start_address, int16_t value)
{
	
    u16 calCRC;
	RS485_TX_BUFF[0] = board_adr;
    RS485_TX_BUFF[1] = SET_SIHGLE_REG;   //modbus 指令码06
    RS485_TX_BUFF[2] = HIGH(start_address);  
    RS485_TX_BUFF[3] = LOW(start_address); 
    RS485_TX_BUFF[4] = HIGH(value);
    RS485_TX_BUFF[5] = LOW(value);
    calCRC=CRC_Compute(RS485_TX_BUFF,6);
	RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
    RS485_TX_BUFF[7]=(calCRC)&0xFF;
	RS485_SendData(RS485_TX_BUFF,8);
}

//CRC校验 自己后面添加的

const u8 auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,0x80, 0x41, 0x00, 0xC1, 0x81, 0x40} ; 


const u8 auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,0x43, 0x83, 0x41, 0x81, 0x80, 0x40} ;


u16 CRC_Compute(u8 *puchMsg, u16 usDataLen) 
{ 
	u8 uchCRCHi = 0xFF ; 
	u8 uchCRCLo = 0xFF ; 
	u32 uIndex ; 
	while (usDataLen--) 
	{ 
		uIndex = uchCRCHi ^ *puchMsg++ ; 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	} 
	return ((uchCRCHi<< 8)  | (uchCRCLo)) ; 
}//uint16 crc16(uint8 *puchMsg, uint16 usDataLen)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TIM13_Init(void)//1s timer for ultrasonic reminder
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  

    TIM_TimeBaseInitStructure.TIM_Period = 0.2*2000-1; 	
    TIM_TimeBaseInitStructure.TIM_Prescaler=42000-1; 
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
    TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStructure);    
    TIM_ClearITPendingBit(TIM13,TIM_IT_Update);
    TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE); 

    NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM13,DISABLE); 
}
void TIM14_Init(void)// 5s stop for safety protection
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  
	
    TIM_TimeBaseInitStructure.TIM_Period = 5*2000-1; 	
	TIM_TimeBaseInitStructure.TIM_Prescaler=42000-1; 
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); 
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM14,DISABLE); 
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)//debug later 2020/9/21!!!!!!!!2020/10/5 is it the right interrupt handler?
{                                                                   
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM14,TIM_IT_Update);// clear interrupt flag 
        Safety_Flag=0x0000;
        left_realtime_temp=0;
        right_realtime_temp=0;
		TIM_Cmd(TIM14,DISABLE);                     //disable timer
	}
    
}
void TIM8_UP_TIM13_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM13,TIM_IT_Update)!=RESET)
    {
        TIM_ClearITPendingBit(TIM13,TIM_IT_Update);
        TIM_SetCompare2(TIM8,1000);
        
        TIM_Cmd(TIM13,DISABLE);                     //disable timer
        //printf("count ends at %lld\r\n",time_tick_1ms);
    }
}