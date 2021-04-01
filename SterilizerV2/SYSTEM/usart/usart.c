#include "sys.h"
#include "usart.h"	  
#include "H_bridge.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

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
	while((USART1->SR&0X40)==0);
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
u16 DMA1_MEM_LEN;
u8 ReceiveBuff[14]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                      0x00,0x00,0x00,0x00,0x00,0x00,0x00};
u8 ValveStatus[8]={0,0,0,0,0,0,0,0};
int ValveLastStatus[8]={-1,-1,-1,-1,-1,-1,-1,-1};
void uart_init(u32 bound){
    DMA_InitTypeDef DMA_InitStructure; 
   GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
                        //ʹ�ܴ���1 
    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
    USART_Cmd(USART1, ENABLE);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    DMA_DeInit(DMA1_Channel5);
    DMA1_MEM_LEN=14;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  
    DMA_InitStructure.DMA_MemoryBaseAddr =(u32)ReceiveBuff;  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
    DMA_InitStructure.DMA_BufferSize = 14; 
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
    DMA_Init(DMA1_Channel5, &DMA_InitStructure); 
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
    DMA_Cmd(DMA1_Channel5,ENABLE);
}

void LoraDataDecode(u8 *pData)
{
    LED_G=!LED_G;
    if(pData==NULL)
    {
        return;
    }
    if(pData[0]==0x3a&&pData[3]==0x0d&&pData[4]==0x0a)
    {
        u8 i;
        for(i=0;i<8;i++)
        {
            u8 getValue=pData[2]&0x01;
            pData[2]=pData[2]>>1;
            switch(getValue)
            {
                case 0:
                    ValveStatus[i]=0;
                    break;
                case 1:
                    ValveStatus[i]=1;
                    break;
            }
        }
    }
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
    
u8 clear_flag=0;
    LED_R=!LED_R;
    if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET)
    {
         
        DMA_Cmd(DMA1_Channel5,DISABLE);
        LoraDataDecode(ReceiveBuff);
        clear_flag=USART1->SR;
        clear_flag=USART1->DR;
        DMA_ClearFlag(DMA1_FLAG_GL5|DMA1_FLAG_TC5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);
        DMA_SetCurrDataCounter(DMA1_Channel5,14);
        DMA_Cmd(DMA1_Channel5,ENABLE);
    }
} 
#endif	

