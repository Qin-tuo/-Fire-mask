#include "print_task.h"

#define user_is_error() toe_is_error(errorListLength)

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t PrintTaskStack;
#endif


void PrintTask(void *pvParameters)//心跳程序主任务
{

	
    while (1)
    {
			  printf("%d,%d,%d,%d,%.2f,%.2f,%.2f,%.4f,%.4f,%.2f\r\n",temperature,humidity,adcx,adcv,pitch,roll,yaw,longitude,latitude,altitude);
        vTaskDelay(20);
			
#if INCLUDE_uxTaskGetStackHighWaterMark
        PrintTaskStack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
