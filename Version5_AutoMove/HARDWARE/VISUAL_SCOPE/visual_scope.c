#include "visual_scope.h"
void UART_send_byte(u8 byte) 
{
       USART_SendData(USART1, byte);        
      while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
}

void uart_putbuff(uint8_t *data,uint32_t size)
{
    uint8_t i=0;
    for(;i<size;i++)
    {
        UART_send_byte(*(data+i));
    }
}

void vcan_sendware(uint8_t *wareaddr, uint32_t waresize)
{
    uint8_t cmdf[2] = {0x03, 0xfc};    
    uint8_t cmdr[2] = {0xfc, 0x03};    

    uart_putbuff(cmdf, sizeof(cmdf));   
    uart_putbuff(wareaddr, waresize);   
    uart_putbuff(cmdr, sizeof(cmdr));    
}
