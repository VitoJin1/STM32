#include "remote_decode.h"
void Remote_to_Message()//remoter data transfer to message data 加入零点死区
{
    if((SYSTEM_ERROR&0x01)==0x01)//lose sbus signal 
    {
        LED_R=0;
        Command.Right_X_Speed=0.0;
        Command.Right_Y_Speed=0.0;
        Command.Left_X_Speed=0.0;
        Command.Left_Y_Speed=0.0;
        
        Command.Solenoid_Switch=0;
        Command.Mode=START_MODE;
        Command.Arm=0;
        Command.Roller_Switch=0;
        Command.Balance_Switch=0;
        
        Command.Left_Prop_Speed=0.0;
        Command.Right_Prop_Speed=0.0;
        Command.Roller_Speed=0.0;
    }
    else
    {     
        LED_R=1;        
        Command.Right_X_Speed    = fRamp_for_LORA(remote_data.ch0);
        Command.Right_Y_Speed    = fRamp_for_LORA(remote_data.ch1);
        Command.Left_X_Speed     = fRamp_for_LORA(remote_data.ch2);
        Command.Left_Y_Speed     = fRamp_for_LORA(remote_data.ch3);

        Command.Left_Prop_Speed  = iPwn_for_LORA(remote_data.prop_speed);
        Command.Right_Prop_Speed = iPwn_for_LORA(remote_data.prop_speed);
        Command.Roller_Speed     = iPwn_for_LORA(remote_data.roller_speed);
        
        Command.Solenoid_Switch  = sRamp_for_LORA(remote_data.valve_switch);
        Command.Mode             = sRamp_for_LORA(remote_data.mode_switch);
        Command.Arm              = sRamp_for_LORA(remote_data.screw_switch);
        Command.Roller_Switch    = sRamp_for_LORA(remote_data.roller_switch);
        Command.Balance_Switch   = sRamp_for_LORA(remote_data.balance_switch);
        Command.Pose             = sRamp_for_LORA(remote_data.pose);
        MODE=Command.Mode;
       
        
    }
    
}

float fRamp_for_LORA(int16_t temp)//joystick analog data interprate, 0-26000 is normal, middle area is 13000 +/- 1000, range set to 1000-25000 
{
    float result;
    if(temp>(13000-1000)&&temp<(13000+1000))
        result=0.0;
    else if(temp<=13000-1000)
        result=((float)temp-12000.0f)/11000.0f;
    else if(temp>13000+1000)
        result=((float)temp-14000.0f)/11000.0f;
    if(result>=1.0f)
        result=1.0f;
    else if(result<=-1.0f)
        result=-1.0f;
    return result;
}

short sRamp_for_LORA(int16_t temp)//sbus 开关量翻译
{
  
    if(temp==1)
        return 1;
    else if(temp==2)
        return 2;
    else 
        return 0;// receive 0 and receive error will generate 0 command.
}

int iPwn_for_LORA(int16_t temp)// joystick value ignore 0-1000 , 25000 up ,   1000- 25000 map to 0-750
{
    int result;
    if(temp<1000)
        result =0;
    else if(temp>=25000)
        result =750;
    else 
        result =(int)((temp-1000)/32);
    return result;
}
