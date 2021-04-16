#ifndef _MAIN_H_
#define _MAIN_H_
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "bsp.h"
#include "timer.h"
#include "modbus.h"
#include "CM_Control.h"
#include "Base_Calculation.h"
#include "math.h"
#include "encoder.h"
#include "encoder_read.h"
#include "usart.h"
#include "visual_scope.h"
#include "Propeller_Control.h"
#include "Step_Motor_Control.h"
#include "exti.h"
#include "remote_decode.h"
#include "Roller_Control.h"
#include "Imu_Decode.h"
#include "key.h"
#include "oled.h"
#include "capture.h"
#include "Putter_Control.h"
#include "Ultrasonic.h"
#include "pulse.h"

#define MANUAL_MODE 0
#define AUTO_MODE 1
#define CALI_MODE 2
#define START_MODE 3
#define ENABLE_BLUETOOTH 0
#define ABS(x) ((x>0)? (x):(-x))
#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))
extern u8 MODE;
extern u16 modbus_dectect_flag;
extern float Z_EulerAngle;
extern float Z_Gyro;
extern u8  right_loss_flag;

#endif
