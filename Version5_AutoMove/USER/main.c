#include "main.h"
#define putter_switch_test 1
#define BLUETOOTH_DEBUG 0
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
#if BLUETOOTH_DEBUG
    float data_upload[8];
#endif
    
    //Master_06_Slove(Cable_Motor_Left_Address,Cable_Motor_Speed_Address,0);
    
    
    while(1) 
	{         
        /********************for PID diagnose***************************************/
        //PROP_OUTER_PID
#if BLUETOOTH_DEBUG
        data_upload[0]=PROP_INNER_PID.ref*100;
        data_upload[1]=PROP_INNER_PID.fdb*100;
        data_upload[2]=PROP_INNER_PID.output;
        data_upload[3]=PROP_OUTER_PID.ref*100;
        data_upload[4]=PROP_OUTER_PID.fdb*100;
        data_upload[5]=Command.Right_Y_Speed;
        data_upload[6]=Z_EulerAngle*100;
        data_upload[7]=Z_Gyro*100;
        vcan_sendware( (uint8_t *)data_upload,sizeof(data_upload));
        delay_ms(10);
#endif         

#if (!BLUETOOTH_DEBUG)
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
         if(ROBOT_UNLOCK==1)
        {
            fun_test(MODE,Command.Arm);
        }
        //printf("l= %f r =%f\r\n",left_realtime_power,right_realtime_power);
#endif

    } 
}
