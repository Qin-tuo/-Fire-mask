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

	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LED_Init();
	usart3_init(38400);
	uart_init(115200);
	uart2_init(9600);
  LCD_Init();
	MPU_Init();
	LCD_Init();
 	usmart_dev.init(72);		//��ʼ��USMART		
	KEY_Init();					//��ʼ������
	//TPAD_Init(6);				//����������ʼ�� 
	DHT11_Init();
	Adc_Init();
	Adc3_Init();
	MPU_Init();					//��ʼ��MPU6050
	
	
	
	
		while(DHT11_Init())	//DHT11��ʼ��	
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
	
		if(SkyTra_Cfg_Rate(5)!=0)	//���ö�λ��Ϣ�����ٶ�Ϊ5Hz,˳���ж�GPSģ���Ƿ���λ. 
	{
   	LCD_ShowString(30,120,200,16,16,"SkyTraF8-BD Setting...");
		do
		{
			usart3_init(9600);			//��ʼ������3������Ϊ9600
	  	SkyTra_Cfg_Prt(3);			//��������ģ��Ĳ�����Ϊ38400
			usart3_init(38400);			//��ʼ������3������Ϊ38400
      key=SkyTra_Cfg_Tp(100000);	//������Ϊ100ms
		}while(SkyTra_Cfg_Rate(5)!=0&&key!=0);//����SkyTraF8-BD�ĸ�������Ϊ5Hz
	  LCD_ShowString(30,120,200,16,16,"SkyTraF8-BD Set Done!!");
		delay_ms(500);
		LCD_Fill(30,120,30+200,120+16,WHITE);//�����ʾ 
	}
}

