#include "usart.h"
/////////////////////////
///////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F103ZE���İ�
//����1��ʼ��		
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 



//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
u8 COMMUNICATION_FLAG=1;
float num1=0;
float num2=0;  
float left_prop=0.1;
float right_prop=0.2;
float prop_stop=0;

u8 IMU_DATA_Cache[91];
u8 SBUS_DATA_Cache[44];

Remote remote_data;
Message Command;

IMU_RawData imu_rawdata;

void USART1_Init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); //��ʼ������1 
    USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1  
	//USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
    
    
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	

	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    

}
float X_Command_Filter[6]={0.0,0.0,0.0,0.0};
float Y_Command_Filter[6]={0.0,0.0,0.0,0.0};

int filter_index=0;
float Average_Limit_Filter(float *array,int filter_count,float limit_max,float limit_min)
{
//    for(int i=0;i<filter_count;i++)
//    {
//        if(*(array+i)>limit_max)
//            *(array+i)=limit_max;
//        if(*(array+i)<limit_min)
//            *(array+i)=limit_min;
//    }
    return (*array+*(array+1)+*(array+2)+*(array+3))/filter_count;
 
}
int64_t time_tick_Command;
int64_t time_tick_LORA;
//volatile int last_prop_left[4]={0,0,0,0};
//volatile int last_prop_right[4]={0,0,0,0};
volatile int temp_left=0;
volatile int temp_right=0;
volatile float temp_X=0.0;
volatile float temp_Y=0.0;
volatile int temp_roller=0;
int lora_receive_cnt=0;
void USART1_IRQHandler(void)                	//����4�жϷ������
{
	//USART_SendData(USART1,0x01);
    u8 Res;
     u16 temp;
    
    //u8 k;
   
    
    
    
  static  union
{
    char bytes[37];
    float decodedata[9];
}receivemessage;


/*
    USART_RX_STA
    
    0x8000 all receive finished
    0x4000 receive 0x0d tailer
    0x2000 receive 0x3a header
    0x0000 receive error----buffer overflow
    */
if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//Enter receiveing process
{
    
    Res =USART_ReceiveData(USART1);//(USART1->DR);//Read receiving data
    //USART_SendData(UART4,Res);
    if((USART_RX_STA&0x8000)==0)//not received all
    {
        if(Res==0x3a)USART_RX_STA|=0x2000;//header flag
        else
        {
            if(USART_RX_STA&0x2000)
            {
                if(Res==0x0d)USART_RX_STA|=0x4000;
                else
                {
                    if(USART_RX_STA&0x4000)
                    {
                        if(Res==0x0a){
                            
                            USART_RX_STA|=0x8000;}
                    }
                    else
                    {                        
                      
                        temp=USART_RX_STA&0x1FFF;
                       // not knowing why I can't write like this ??
                        //in this way the od oa must follow  3a,otherwise they can't be recongnized
//                        USART_RX_BUF[USART_RX_STA&0X1FFF]=Res ;
//                        USART_RX_STA++;
//                        if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0; 
                        USART_RX_BUF[temp]=Res;
                        USART_RX_STA=USART_RX_STA+1;
                        if (temp>USART_REC_LEN-1)
                            USART_RX_STA=0x0000;
                    }
                }
            }   
        }
    }
}

/*
DATA Agreement
0x3a         %f      %f     %f      %f        %f         %f          %f           %f        %f         %f     %f        0x0d   0xoa
Frame head   X_Vel   Y_Vel  ang_vel wind_Left wind_right unwind_Left unwind_right left_prop right_prop roller prop_Stop Frame tail  
             Data0   Data1  Data2   Data3     Data4      Data5       Data6        Data7     Data8      Data9  Data10     
*/
if(USART_RX_STA&0x8000)// all receiving finished
{	
    u16 len;
    COMMUNICATION_FLAG=1;
    len=USART_RX_STA&0x1fff;//�õ��˴ν��յ������ݳ���
    time_tick_Command=time_tick_1ms;
    #if ENABLE_BLUETOOTH
   // printf("received length%d\r\n",len);
    
    for(i=0;i<len;i++)
    {
        receivemessage.bytes[i]=USART_RX_BUF[i];  
        //printf("%x ",receivemessage.bytes[i]);
    }
    //printf("\r\n");
    #endif 
    
    Command.Left_X_Speed=receivemessage.decodedata[0]-XY_OFFSET;
    Command.Right_Y_Speed=receivemessage.decodedata[1]-XY_OFFSET;
    Command.Roller_Speed=receivemessage.decodedata[2]-RoLLER_OFFSET;
    Command.Left_Prop_Speed=receivemessage.decodedata[3]-LR_OFFSET;
    Command.Right_Prop_Speed=receivemessage.decodedata[4]-LR_OFFSET;
    
    Command.Roller_Switch=receivemessage.bytes[36]&0x01;
    Command.Solenoid_Switch=(receivemessage.bytes[36]&0x02)>>1;
    Command.Balance_Switch=(receivemessage.bytes[36]&0x04)>>2;
    
  
  
    
    if(Command.Left_Prop_Speed>750||Command.Left_Prop_Speed<0)
        Command.Left_Prop_Speed=temp_left;
    if(Command.Right_Prop_Speed>750||Command.Left_Prop_Speed<0)
        Command.Right_Prop_Speed=temp_right;
    if(Command.Roller_Speed>750||Command.Roller_Speed<0)
        Command.Roller_Speed=temp_roller;
    temp_roller=Command.Roller_Speed;
    temp_left=Command.Left_Prop_Speed;
    temp_right=Command.Right_Prop_Speed;
    
    if(Command.Left_X_Speed>1||Command.Left_X_Speed<-1)
        Command.Left_X_Speed=temp_X;
    if(Command.Right_Y_Speed>1||Command.Right_Y_Speed<-1)
        Command.Right_Y_Speed=temp_Y;
    temp_X=Command.Left_X_Speed;
    temp_Y=Command.Right_Y_Speed;
   
    
    /*average filter + limiting filter*/
    if(fabs(Command.Left_X_Speed)<=1.0&&fabs(Command.Right_Y_Speed)<=1.0)
    {
    X_Command_Filter[filter_index]=Command.Left_X_Speed;
    Y_Command_Filter[filter_index]=Command.Right_Y_Speed;
    filter_index=(filter_index+1)%4;
    }
    Command.Left_X_Speed =Average_Limit_Filter(X_Command_Filter,4,1.0,-1.0);
    Command.Right_Y_Speed=Average_Limit_Filter(Y_Command_Filter,4,1.0,-1.0);
    
    
    
        USART_RX_STA=0;
}

} 


void UART4_Init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//ʹ��USART1��GPIOAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//ʹ��USART1��GPIOAʱ��
	//USART4_TX   GPIOC.10
    //USART_DeInit(UART4);
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11; //PC.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//�����������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA.9
 

  //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(UART4, &USART_InitStructure); //��ʼ������1
    //USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ���1 
}


/*SBUS Receive init(uart3+DMA)*/

void uart3_init(u32 bound){
GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART1ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOA9����ΪUSART1
	//GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;	//�շ�ģʽ
    USART_Init(USART3, &USART_InitStructure); //��ʼ������1 
    USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1  
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
    
    
	USART_ClearFlag(USART3, USART_FLAG_TC);
	

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
    DMA_DeInit(DMA1_Stream1);
    while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SBUS_DATA_Cache;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
    DMA_InitStructure.DMA_BufferSize = 220;
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
    DMA_Init(DMA1_Stream1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream1, ENABLE);
}
u8 Res;
u8 u3receive_count=0;
u8 DMA_Residual_length;
void USART3_IRQHandler(void)                	//����1�жϷ������
{ u8 clear_flag=0;
    
//    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//Enter receiveing process
//{
//    Res =USART_ReceiveData(USART3);//(USART1->DR);//Read receiving data
//    USART_SendData(USART1,Res);
////    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//    //u3receive_count++;
//    USART_ClearITPendingBit(USART3,USART_IT_RXNE);
//}
if(USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET){
    
    u8 i=0;
    DMA_Cmd(DMA1_Stream1,DISABLE);
    clear_flag=USART3->SR;
    clear_flag=USART3->DR;
    DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1|DMA_FLAG_FEIF1|DMA_FLAG_DMEIF1|DMA_FLAG_TEIF1|DMA_FLAG_HTIF1);
    time_tick_LORA=time_tick_1ms;
    for(i=0;i<22;i++){
        if(SBUS_DATA_Cache[i]==0x0a&&SBUS_DATA_Cache[i+1]==0x0d&&SBUS_DATA_Cache[i+20]==0x0d&&SBUS_DATA_Cache[i+21]==0x0a)
        {
             RemoteDataProcess(&SBUS_DATA_Cache[i]);
            break;
        }
    }
    DMA_SetCurrDataCounter(DMA1_Stream1,44);
    DMA_Cmd(DMA1_Stream1,ENABLE);
    
    
//    USART_SendData(USART1,0xff);
//    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//    for(i=0;i<22;i++){
//    USART_SendData(USART1,SBUS_DATA_Cache[i]);
//        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//        
//    }
//    USART_SendData(USART1,0xff);
//    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
    
    
}
}

void uart4_init(u32 bound){
GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��USART1ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(UART4, &USART_InitStructure); //��ʼ������1 
    USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���1  
	USART_ITConfig(UART4,USART_IT_IDLE,ENABLE);
    
    
	USART_ClearFlag(UART4, USART_FLAG_TC);
	

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    
    USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
    DMA_DeInit(DMA1_Stream2);
    while (DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&UART4->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)IMU_DATA_Cache;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
    DMA_InitStructure.DMA_BufferSize = 910;
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
    DMA_Init(DMA1_Stream2, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream2, ENABLE);
}
u8 Res;
int u4receive_count=0;
void UART4_IRQHandler(void)                	//����1�жϷ������
{
    u8 clear_flag=0;
//    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//Enter receiveing process
//{
//    Res =USART_ReceiveData(UART4);//(USART1->DR);//Read receiving data
//    //USART_SendData(UART4,Res);
//    //while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//    u4receive_count++;
//    USART_ClearITPendingBit(UART4,USART_IT_RXNE);
//}
if(USART_GetITStatus(UART4,USART_IT_IDLE)!=RESET){
    calc_cnt[0]++;
    DMA_Cmd(DMA1_Stream2,DISABLE);
    
    clear_flag=UART4->SR;
    clear_flag=UART4->DR;
    DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_TCIF2|DMA_FLAG_FEIF2|DMA_FLAG_DMEIF2|DMA_FLAG_TEIF2|DMA_FLAG_HTIF2);
    DMA_SetCurrDataCounter(DMA1_Stream2,91);
    DMA_Cmd(DMA1_Stream2,ENABLE);
   
//    for(i=0;i<6;i++)
//    {
//        USART_SendData(USART1,IMU_DATA_Cache[i]);
//        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//    }
//    USART_SendData(USART1,0x0d);
//    USART_SendData(USART1,0x0a);
    USART_ClearITPendingBit(UART4,USART_IT_IDLE);
}
}
int16_t RawAnalogDataFromLora[8];
int16_t RawAnalogDataFromLoraLast[8]={13000,13000,13000,13000,0,0,0,0};
u8 LoraDataFirstRead=0;
void RemoteDataProcess(uint8_t *pData)
{
    if(pData ==NULL)
    {
        return; 
    }
    u8 i=0;
    uint16_t temp=0;
    /*
    for(i=0;i<16;i=i+2)
    { 
        temp=pData[2+i]*256+pData[2+i+1];
        
    }
    
    
  
    for(i=0;i<6;i++)
    {
        printf("%d = %d ",i+16,(pData[19]>>(8-1-i))&0x01);
    }
    printf("\r\n");
    */
    //prevent read ffff and regard it as 65535 ,regonize error message
    
    
    
    if(pData[0]==0x0a&&pData[1]==0x0d&&pData[20]==0x0d&&pData[21]==0x0a)
    {
        for(i=0;i<16;i=i+2)
        {
            RawAnalogDataFromLora[i/2]=pData[2+i]*256+pData[2+i+1];
            if(ABS(RawAnalogDataFromLoraLast[i/2]-RawAnalogDataFromLora[i/2])>10000)
                RawAnalogDataFromLora[i/2]=RawAnalogDataFromLora[i/2];
        }
        
        remote_data.ch2=(pData[2]*256+pData[3]*1);
        remote_data.ch3=(pData[4]*256+pData[5]*1);
        remote_data.ch1=(pData[6]*256+pData[7]*1);
        remote_data.ch0=(pData[8]*256+pData[9]*1);
        
        remote_data.roller_speed=(pData[10]*256+pData[11]*1);
        remote_data.pump_speed=(pData[12]*256+pData[13]*1);
        //(pData[14]*256+pData[15]*1);
        remote_data.prop_speed=(pData[16]*256+pData[17]*1);
        
        if( (((pData[18]>>7)&0x01)==1)  && (((pData[18]>>6)&0x01)==0)  )
            remote_data.pose=0;
        else if( (((pData[18]>>7)&0x01)==0)  && (((pData[18]>>6)&0x01)==0) )
            remote_data.pose=1;
        else if( (((pData[18]>>7)&0x01)==0)  && (((pData[18]>>6)&0x01)==1)  )
            remote_data.pose=2;
        else 
            remote_data.pose=0;// error situation
        
        
        if( (((pData[18]>>5)&0x01)==0)  && (((pData[18]>>4)&0x01)==0) )
            remote_data.screw_switch=0;
        else if( (((pData[18]>>5)&0x01)==1)  && (((pData[18]>>4)&0x01)==0) )
            remote_data.screw_switch=1;
        else if( (((pData[18]>>5)&0x01)==0)  && (((pData[18]>>4)&0x01)==1) )
            remote_data.screw_switch=2;
        /*else 
            remote_data.screw_switch=0;*/// error situation
        /*
            temp=(pData[18]>>5)&0x01;
            printf("screw =%d",temp);
            temp=(pData[18]>>4)&0x01;
            printf("     %d    ",temp);
        */
        if( (((pData[18]>>3)&0x01)==1)  && (((pData[18]>>2)&0x01)==0) )
            remote_data.mode_switch=0;
        else if (  (((pData[18]>>3)&0x01)==0)  && (((pData[18]>>2)&0x01)==0) )
            remote_data.mode_switch=1;
        else if (  (((pData[18]>>3)&0x01)==0)  && (((pData[18]>>2)&0x01)==1) )
            remote_data.mode_switch=2;
        
        /*
            temp=(pData[18]>>3)&0x01;
            printf("mode =%d",temp);
            temp=(pData[18]>>2)&0x01;
            printf("    =%d\r\n",temp);  
        */
        /*
        else  
            remote_data.mode_switch=0;*///error situation 

        //(pData[18]>>1)&0x01;
        //(pData[18]>>0)&0x01;
        //(pData[19]>>7)&0x01;
        //(pData[19]>>6)&0x01;
        remote_data.roller_switch  =(pData[19]>>5)&0x01;
        // (pData[19]>>4)&0x01;
        remote_data.balance_switch =(pData[19]>>3)&0x01;
        remote_data.valve_switch   =(pData[19]>>2)&0x01;
        lora_receive_cnt++;
    }
    else {  
        return;
    }
}
