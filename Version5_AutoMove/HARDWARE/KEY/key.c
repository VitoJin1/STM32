#include "key.h"
#include "sys.h" 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6;//PE2~4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
}
//����������
//���ذ���ֵ

//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!

//1---֧������
//2---��֧������
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;
    if(mode)key_up=1;
	if(key_up&&(Switch1==0||Switch2==0))
	{
		delay_ms(10);//ȥ���
        key_up=0;
		if(Switch1==0)return KEY_S1;
		else if(Switch2==0)return KEY_S2;
	}
    else if(Switch1==1&&Switch2==1)
    {
        key_up=1;
    }
 	return 0;// �ް�������
}
