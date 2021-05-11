#include "led.h" 
#include "math.h"

#define wsChipNum1   5 
#define wsChipNum2   5 
#define wsChipNum3   5 
#define wsChipNum4   5 
unsigned long last_LEDBanddata_left;
unsigned long last_LEDBanddata_right;
//LED IO初始化

void LED_Init(void)
{    	 
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOF时钟
    //GPIOF9,F10初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
    GPIO_SetBits(GPIOC, GPIO_Pin_0);
    GPIO_SetBits(GPIOC, GPIO_Pin_1);
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
    GPIO_SetBits(GPIOC, GPIO_Pin_3);
   
}
void LargeCurrentSwitch(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOF时钟
    //GPIOF9,F10初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
    GPIO_ResetBits(GPIOC, GPIO_Pin_4);
    GPIO_ResetBits(GPIOC, GPIO_Pin_5);
    GPIO_ResetBits(GPIOC, GPIO_Pin_6);
}
void LEDBand_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;        
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE );         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
}
void TX0(void) 
{ 
	  
			  PBout(12) = 1;
			  
		
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
      __NOP();__NOP(); __NOP();__NOP();__NOP();
      __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
    
    __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
      __NOP();__NOP(); __NOP();__NOP();__NOP();
      __NOP();__NOP();__NOP();__NOP();__NOP(); 
    __NOP();__NOP();__NOP();__NOP();__NOP();
    
    
	 __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  
	  
			 PBout(12)= 0; 
		
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP(); 
    
      __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
      
      __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP(); 
      
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP(); 
    
    __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
      __NOP();__NOP();__NOP();__NOP();
      
     
	   
} 
void TX1(void)         
{ 
	    PBout(12) = 1;
		
	 	
	    __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP(); 
    
      __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
      
      __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP(); 
      
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP(); 
    
    __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
      __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	 PBout(12) = 0;
    __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
      __NOP();__NOP(); __NOP();__NOP();__NOP();
      __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
    
    __NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();
      __NOP();__NOP(); __NOP();__NOP();__NOP();
      __NOP();__NOP();__NOP();__NOP();__NOP(); 
    __NOP();__NOP();__NOP();__NOP();__NOP();
    
    
	__NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();
      
   
	  
}
void wsSetOneChip(unsigned long dat)
{
    unsigned char i;
        
    for(i=0; i < 24; i++)
    {
        if(0x800000 == (dat & 0x800000) )        
            TX1();
        else     
					  TX0();
        dat<<=1;                                                       
     }
}
void SetLEDBand(unsigned long data1,unsigned long data2)
{
    unsigned char i;
    i=0;
    while(i<11)
    {
        wsSetOneChip(data1);
        i++;
    }
    i=0;
    last_LEDBanddata_left=data1;
    while(i<10)
    {
        wsSetOneChip(data2);
        i++;
    }
    i=0;
    last_LEDBanddata_right=data2;
}
void wsSetAllChip(unsigned long dat1, unsigned long dat2,unsigned long dat3, unsigned long dat4)
{
    
    unsigned char j;
    j=0;
    wsSetOneChip(dat1);
    while(j<wsChipNum1)
    
    {
        wsSetOneChip(dat1);  // j / 0
        j++;
    }
       j=0;
    while(j<wsChipNum2)
    
    {
        wsSetOneChip(dat2);  // j / 0
        j++;
    }
    j=0;
    while(j<wsChipNum3)
    
    {
        wsSetOneChip(dat3);  // j / 0
        j++;
    }
       j=0;
    while(j<wsChipNum4)
    
    {
        wsSetOneChip(dat4);  // j / 0
        j++;
    }
}
unsigned long  wsSetColor(unsigned long Rval,unsigned long Gval,unsigned long Bval)
{
    unsigned long setColor=0x000000;
    setColor=(Rval<<16)|(Bval<<8)|(Gval);
    return setColor;
}

/*all value should be positive*/
void LEDBand_ShowSignal(float left_power,float left_tempeature,float right_power,float right_tempeature)
{
    unsigned long left_up_color=0xffffff;
    //unsigned long left_down_color=0xffffff;
    unsigned long right_up_color=0xffffff;
    //unsigned long right_down_color=0xffffff;
    left_power=fabs(left_power);
    right_power=fabs(right_power);
    //left_up_color=wsSetColor(0xff,0xff-float2RGB(0,20,left_power),0xff-float2RGB(0,20,left_power));
    //left_down_color=wsSetColor(0xff,0xff-float2RGB((float)0,(float)80,(float)left_tempeature),0xff-float2RGB((float)0,(float)80,(float)left_tempeature));
    //right_up_color=wsSetColor(0xff,0xff-float2RGB(0,20,right_power),0xff-float2RGB(0,20,right_power));
    //right_down_color=wsSetColor(0xff,0xff-float2RGB(((float)0),((float)80),((float)right_tempeature)),0xff-float2RGB(((float)0),((float)80),((float)right_tempeature)));
    
    if(left_power>=0&&left_power<5)
        left_up_color=0xffc8c8;
    else if(left_power>=5&&left_power<10)
        left_up_color=0xff9696;
    else if(left_power>=10&&left_power<15)
        left_up_color=0xff6464;
    else if(left_power>=16&&left_power<20)
        left_up_color=0xff3232;
    else 
        left_up_color=0xff0000;
    
    if(right_power>=0&&right_power<5)
        right_up_color=0xffc8c8;
    else if(right_power>=5&&right_power<10)
        right_up_color=0xff9696;
    else if(right_power>=10&&right_power<15)
        right_up_color=0xff6464;
    else if(right_power>=16&&right_power<20)
        right_up_color=0xff3232;
    else 
        right_up_color=0xff0000;
    
    
    
    
    
    //printf("%lu,%lu\r\n",left_up_color,right_up_color);
    //wsSetAllChip(left_up_color,left_down_color,right_up_color,right_down_color);
    wsSetAllChip(left_up_color,left_up_color,right_up_color,right_up_color);
     PBout(12) = 0;
    //??? once add this delay, the first input value will not update(keep 0)
    //delay_ms(10);
    
}
/* float to RGB value(0x00-0xff)*/
int float2RGB(float input_min,float input_max,float input_value)
{
    float ratio=input_value/(input_max-input_min);
    int RGB_value=0x00;
    RGB_value=ratio*0xFF;
    RGB_value=ilimit(RGB_value,0x00,0xff);
    return RGB_value;
}


