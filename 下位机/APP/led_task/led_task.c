#include "led_task.h"

#define user_is_error() toe_is_error(errorListLength)

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t LedTaskStack;
#endif


void LedTask(void *pvParameters)//心跳程序主任务
{

	
    while (1)
    {
			  LED0=~LED0;
        vTaskDelay(500);
			
#if INCLUDE_uxTaskGetStackHighWaterMark
        LedTaskStack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
