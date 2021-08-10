#include "lcd_task.h"
#include "all_headfile.h"
#define user_is_error() toe_is_error(errorListLength)

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t LcdTaskStack;
#endif


void LcdTask(void *pvParameters)//显示屏主任务
{
	POINT_COLOR=REDLCD;	  
	while(1)
	{
		LCD_ShowString(30,10,200,16,16,"Temp:"); 
		LCD_ShowxNum(70,10,temperature,2,16,0); 
		LCD_ShowString(100,10,200,16,16,"hum:"); 
		LCD_ShowxNum(130,10,humidity,2,16,0); 
		LCD_ShowString(30,40,200,16,16,"MQ:"); 
		LCD_ShowxNum(70,40,adcx,4,16,0);   	
	  LCD_ShowString(120,40,200,16,16,"VO:"); 
		LCD_ShowxNum(150,40,adcv,4,16,0);   
		
		LCD_ShowString(30,400,200,12,12," Temp:    . C");	
		LCD_ShowString(150,400,200,12,12,"Pitch:    . C");	
		LCD_ShowString(30,410,200,12,12," Roll:    . C");	 
		LCD_ShowString(150,410,200,12,12," Yaw :    . C");	
    
		vTaskDelay(1);
			
#if INCLUDE_uxTaskGetStackHighWaterMark
        LcdTaskStack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
