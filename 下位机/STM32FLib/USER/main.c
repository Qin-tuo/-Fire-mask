#include "all_headfile.h"

void BSP_init(void);



int main(void)
{ 
	BSP_init();
  delay_ms(100);
  startTast();
  vTaskStartScheduler();
	while(1)
	{
		
	}
}


	u8 key=0XFF;
void BSP_init(void)
{

	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	LED_Init();
	usart3_init(38400);
	uart_init(115200);
	uart2_init(9600);
  LCD_Init();
	MPU_Init();
	LCD_Init();
 	usmart_dev.init(72);		//初始化USMART		
	KEY_Init();					//初始化按键
	//TPAD_Init(6);				//触摸按键初始化 
	DHT11_Init();
	Adc_Init();
	Adc3_Init();
	MPU_Init();					//初始化MPU6050
	
	
	
	
		while(DHT11_Init())	//DHT11初始化	
	{
		LCD_Fill(30,130,239,130+16,WHITE);
		delay_ms(200);
	}		
	
		while(mpu_dmp_init()!=0)
	{
		LCD_ShowString(30,130,200,16,16,"MPU6050 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
		delay_ms(200);
	}
	
		if(SkyTra_Cfg_Rate(5)!=0)	//设置定位信息更新速度为5Hz,顺便判断GPS模块是否在位. 
	{
   	LCD_ShowString(30,120,200,16,16,"SkyTraF8-BD Setting...");
		do
		{
			usart3_init(9600);			//初始化串口3波特率为9600
	  	SkyTra_Cfg_Prt(3);			//重新设置模块的波特率为38400
			usart3_init(38400);			//初始化串口3波特率为38400
      key=SkyTra_Cfg_Tp(100000);	//脉冲宽度为100ms
		}while(SkyTra_Cfg_Rate(5)!=0&&key!=0);//配置SkyTraF8-BD的更新速率为5Hz
	  LCD_ShowString(30,120,200,16,16,"SkyTraF8-BD Set Done!!");
		delay_ms(500);
		LCD_Fill(30,120,30+200,120+16,WHITE);//清除显示 
	}
}

