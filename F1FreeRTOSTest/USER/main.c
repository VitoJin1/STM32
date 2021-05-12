#include "led.h"
#include "delay.h"

#include "sys.h"
#include "usart.h"

#include "FreeRTOS.h"
#include "task.h"


/************************************************
//STM32F103ZE核心板
 PWM输出实验  

************************************************/

#define START_TASK_PRIO 1
#define START_STK_SIZE  128
TaskHandle_t StartTask_Handler;
void start_task (void * pvParameters);

#define LEDBLUE_TASK_PRIO 2
#define LEDBLUE_STK_SIZE 50
TaskHandle_t LEDBLUETASK_Handler;
void LEDBlue_task(void *pvParameters);

#define LEDGREEN_TASK_PRIO 3
#define LEDGREEN_STK_SIZE 50
TaskHandle_t LEDGREENTASK_Handler;
void LEDGreen_task(void *pvParameters);

int main(void)
{		
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    delay_init();
    LED_Init();	
    xTaskCreate((TaskFunction_t)start_task  ,
                (const char*)"start_task"   ,
                (uint16_t)START_STK_SIZE    ,
                (void*)NULL                 ,
                (UBaseType_t)START_TASK_PRIO,
                (TaskHandle_t *)&StartTask_Handler);
    vTaskStartScheduler();
}
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();
    xTaskCreate((TaskFunction_t)LEDBlue_task,
                 (const char*)"ledblue_task",
                 (uint16_t)LEDBLUE_STK_SIZE,
                 (void*)NULL,
                 (UBaseType_t)LEDBLUE_TASK_PRIO,
                 (TaskHandle_t *)&LEDBLUETASK_Handler);
                 
    xTaskCreate((TaskFunction_t)LEDGreen_task,
                 (const char*)"ledgreen_task",
                 (uint16_t)LEDGREEN_STK_SIZE,
                 (void*)NULL,
                 (UBaseType_t)LEDGREEN_TASK_PRIO,
                 (TaskHandle_t *)&LEDGREENTASK_Handler);
    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();
}
void LEDBlue_task(void *pvParameters){
    while(1){
        LED_B=~LED_B;
        vTaskDelay(500);
    }
}

void LEDGreen_task(void *pvParameters){
    while(1){
        LED_G=0;
        vTaskDelay(200);
        LED_G=1;
        vTaskDelay(800);
    }
}
