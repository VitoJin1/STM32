#include "main.h"
#define putter_switch_test 1
u8 MODE=START_MODE;
float Z_EulerAngle=0.0;
float Z_Gyro=0.0;
/*
0-not dectect anything
1-dectect left power
2-dectect right power
3-dectect left tempature
4-dectect right tempature
*/
u16 modbus_dectect_flag=0;
u8 right_loss_flag=0;
int main(void)
{ 
    BSP_Init();
    ///SetLEDBand(WsDat_Org,WsDat_Org);
    float data_upload[8];
    int channel[16];
    u8 i=0;
    //Master_06_Slove(Cable_Motor_Left_Address,Cable_Motor_Speed_Address,0);
    u8 key_value=0;
    
    while(1) 
	{ 
        
        
        /********************for PID diagnose***************************************/
        
        
//        data_upload[0]=PROP_INNER_PID.ref*100;
//        data_upload[1]=PROP_INNER_PID.fdb*100;
//        data_upload[2]=PROP_INNER_PID.output*10;
//        data_upload[3]=PROP_OUTER_PID.ref*100;
//        data_upload[4]=PROP_OUTER_PID.fdb*100;
//        data_upload[5]=IMU_Shift*100;
//        data_upload[6]=Z_EulerAngle*100;
//        data_upload[7]=Z_Gyro*100;
//        vcan_sendware( (uint8_t *)data_upload,sizeof(data_upload));
//        delay_ms(10);
        
        
//        for(i=0;i<8;i++)
//        {
//            USART_SendData(USART1,MODBUS_DATA_Cache[i]);
//            while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//        }
//         USART_SendData(USART1,0xaa);
//            while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//         
//        
       
       

       key_value=KEY_Scan(1);
       
/****putter control test*********/
#if 1
    switch(key_value)
    {
        case KEY_S1:
            /*
            if(distance<5)
                distance++;
            else
                distance=5;
        */
//            Putter_IN1=1;
//            Putter_IN2=0;
            break;
        case KEY_S2:
            /*
            if(distance>1)
            distance--;
            else 
                distance=1;
        */
//            Putter_IN1=0;
//            Putter_IN2=1;
            break;
//        case 0:
//            
//            Putter_IN1=0;
//            Putter_IN2=0;
//            break;  
    }
#endif
/****putter control test end*****/
       /*
       if(Switch1==0)
       {
           delay_ms(10);
           if(Switch1==0)
           {
               LED_Y=0;
               right_loss_flag=1;
           }
           else
           { 
               LED_Y=1;
               right_loss_flag=0;
           }
       }
       else 
       {
           LED_Y=1;
           right_loss_flag=0;
       }
       */
//        
//         
//        if(key_value)
//        {
//            switch(key_value)
//            {
//                case KEY_S1:
//                    //DMA_Cmd(DMA1_Stream5, ENABLE);
//                    Master_03_Slove(Cable_Motor_Left_Address,Max_Power_Show_Address,1);
//                    LED_B=!LED_B;
//                    break;
//                case KEY_S2:
//                    LED_Y=!LED_Y;
//                    if(LED_Y==0)
//                    Master_06_Slove(Cable_Motor_Left_Address,Cable_Motor_Speed_Address,1000);
//                    else if(LED_Y==1)
//                    Master_06_Slove(Cable_Motor_Left_Address,Cable_Motor_Speed_Address,0);    
//                    break;
//            }
//        }
//      
 
        
        //LEDBand_ShowSignal(left_realtime_power,left_realtime_temp,right_realtime_power,right_realtime_temp);
       
       
       
        
        delay_ms(100);
        Master_03_Slove(Cable_Motor_Left_Address,Max_Power_Show_Address,1);
        modbus_dectect_flag=1;
        delay_ms(100);
        Master_03_Slove(Cable_Motor_Left_Address,IGBT_Temperature_Address,1);
        modbus_dectect_flag=3;
        delay_ms(100);
        Master_03_Slove(Cable_Motor_Right_Address,Max_Power_Show_Address,1);
        modbus_dectect_flag=2;
        delay_ms(100);
        Master_03_Slove(Cable_Motor_Right_Address,IGBT_Temperature_Address,1);
        modbus_dectect_flag=4;
        delay_ms(100);
       
        
        
        /*
         if(ROBOT_UNLOCK==1)
        {
        fun_test(MODE,Command.Arm);
        }*/
        
        
        
  
        
    } 
}
