/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       start_task.c/h
  * @brief      �������񣬽�һ������������������Դ�������������ȼ�,
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. ���
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */

#include "Start_Task.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led_task.h"
#include "lcd_task.h"
#include "sensor_task.h"
#include "camera_task.h"
#include "print_task.h"

#define Camera_TASK_PRIO 25
#define Camera_STK_SIZE 1024
static TaskHandle_t CameraTask_Handler;

#define Print_TASK_PRIO 20
#define Print_STK_SIZE 512
static TaskHandle_t PrintTask_Handler;

#define Sensor_TASK_PRIO 23
#define Sensor_STK_SIZE 256
static TaskHandle_t SensorTask_Handler;


#define Lcd_TASK_PRIO 10
#define Lcd_STK_SIZE 512
static TaskHandle_t LcdTask_Handler;

#define Led_TASK_PRIO 5
#define Led_STK_SIZE 256
static TaskHandle_t LedTask_Handler;

#define START_TASK_PRIO 1
#define START_STK_SIZE 512
static TaskHandle_t StartTask_Handler;



void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();
	
				xTaskCreate((TaskFunction_t)LcdTask,
                (const char *)"LcdTask",
                (uint16_t)Lcd_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)Lcd_TASK_PRIO,
                (TaskHandle_t *)&LcdTask_Handler);
//								
	    xTaskCreate((TaskFunction_t)LedTask,
                (const char *)"LedTask",
                (uint16_t)Led_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)Led_TASK_PRIO,
                (TaskHandle_t *)&LedTask_Handler);								
								
      xTaskCreate((TaskFunction_t)SensorTask,
                (const char *)"SensorTask",
                (uint16_t)Sensor_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)Sensor_TASK_PRIO,
                (TaskHandle_t *)&SensorTask_Handler);
								
			xTaskCreate((TaskFunction_t)CameraTask,
                (const char *)"CameraTask",
                (uint16_t)Camera_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)Camera_TASK_PRIO,
                (TaskHandle_t *)&CameraTask_Handler);
								
			xTaskCreate((TaskFunction_t)PrintTask,
                (const char *)"PrintTask",
                (uint16_t)Print_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)Print_TASK_PRIO,
                (TaskHandle_t *)&PrintTask_Handler);
								
//			xTaskCreate((TaskFunction_t)TestTask,
//                (const char *)"TestTask",
//                (uint16_t)Test_STK_SIZE,
//                (void *)NULL,
//                (UBaseType_t)Test_TASK_PRIO,
//                (TaskHandle_t *)&TestTask_Handler);

								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}



void startTast(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //������
                (const char *)"start_task",          //��������
                (uint16_t)START_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)START_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&StartTask_Handler); //������
}
