#include "camera_task.h"
#include "led.h"
#define user_is_error() toe_is_error(errorListLength)

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t CameraTaskStack;
#endif

#define SEND_BUF_SIZE 19202

void camera_refresh(void);

u8 cmd[4]={0, 255, 1, 0 };

/*---ͼ������Ҫ���ⲿ����---*/
extern u8 Pic_Buff[V][H];
extern u8 Black_Line[60];
extern u16 middle_dot;
extern u8 Search_Cnt;    //

 u16 color_r;
u16 color_g;
u16 color_b;
u16 v=0,h=0;
u16 row,col;
int buffer_full = 0;
u8 SendBuff[SEND_BUF_SIZE] = {0xaa,0x03};	
typedef struct{
	u16 Cnt;
}Point;

Point WhitePoint;
Point BlackPoint;



u8 a;
u8 t=0,report=1;			//Ĭ�Ͽ����ϱ�
u8 flag=0;
float pitch,roll,yaw; 		//ŷ����
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����
short MPUtemp;					//�¶�	
	u16 i,rxlen;
	u16 lenx;

	u8 upload=0;	 
	
u8 USART1_TX_BUF[USART3_MAX_RECV_LEN]; 					//����1,���ͻ�����
nmea_msg gpsx; 											//GPS��Ϣ
__align(4) u8 dtbuf[50];   								//��ӡ������
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};	//fix mode�ַ��� 

 float longitude,latitude,altitude;



//��ʾGPS��λ��Ϣ 
void Gps_Msg_Show(void)
{
 	float tp;		   
	POINT_COLOR=BLUELCD;  	 
	tp=gpsx.longitude;
	longitude=tp/100000;
	sprintf((char *)dtbuf,"Longitude:%.5f %1c   ",tp/=100000,gpsx.ewhemi);	//�õ������ַ���
 	LCD_ShowString(30,420,200,12,12,dtbuf);	 	   
	tp=gpsx.latitude;
	latitude=tp/100000;
	sprintf((char *)dtbuf,"Latitude:%.5f %1c   ",tp/=100000,gpsx.nshemi);	//�õ�γ���ַ���
 	LCD_ShowString(30,430,200,12,12,dtbuf);	 	 
	tp=gpsx.altitude;	
	altitude=tp/10;	
 	sprintf((char *)dtbuf,"Altitude:%.1fm     ",tp/=10);	    			//�õ��߶��ַ���
 	LCD_ShowString(30,440,200,12,12,dtbuf);	 			   
//	tp=gpsx.speed;	   
// 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    		//�õ��ٶ��ַ���	 
// 	LCD_ShowString(30,450,200,16,16,dtbuf);	 				    
//	if(gpsx.fixmode<=3)														//��λ״̬
//	{  
//		sprintf((char *)dtbuf,"Fix Mode:%s",fixmode_tbl[gpsx.fixmode]);	
//	  LCD_ShowString(30,200,200,16,16,dtbuf);			   
//	}	 	   
//	sprintf((char *)dtbuf,"GPS+BD Valid satellite:%02d",gpsx.posslnum);	 		//���ڶ�λ��GPS������
// 	LCD_ShowString(30,450,200,12,12,dtbuf);	    
//	sprintf((char *)dtbuf,"GPS Visible satellite:%02d",gpsx.svnum%100);	 		//�ɼ�GPS������
 	LCD_ShowString(30,460,200,12,12,dtbuf);
	
	sprintf((char *)dtbuf,"BD Visible satellite:%02d",gpsx.beidou_svnum%100);	 		//�ɼ�����������
 	LCD_ShowString(30,450,200,12,12,dtbuf);
	
//	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//��ʾUTC����
//	LCD_ShowString(30,280,200,16,16,dtbuf);		    
//	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//��ʾUTCʱ��
//  LCD_ShowString(30,300,200,16,16,dtbuf);		  	  
}


void camera_refresh(void);

void CameraTask(void *pvParameters)//����ͷ������
{
	u8 lightmode=0,saturation=2,brightness=2,contrast=2;
	u8 effect=0;	 
  LCD_ShowString(30,230,200,16,16,"OV7670 Init...");	  
	while(OV7670_Init())//��ʼ��OV7670
	{
		LCD_ShowString(30,230,200,16,16,"OV7670 Error!!");
		delay_ms(200);
	  LCD_Fill(30,230,239,246,WHITE);
		delay_ms(200);
	}
 	LCD_ShowString(30,230,200,16,16,"OV7670 Init OK");
	delay_ms(1500);	 	   
	OV7670_Light_Mode(lightmode);
	OV7670_Color_Saturation(saturation);
	OV7670_Brightness(brightness);
	OV7670_Contrast(contrast);
 	OV7670_Special_Effects(effect);	 
	TIM6_Int_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�
  MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,����SEND_BUF_SIZE.
	
	EXTI8_Init();						//ʹ�ܶ�ʱ������
	OV7670_Window_Set(12,176,240,320);	//���ô���	  
  OV7670_CS=0;					
	LCD_Clear(BLACK);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ���1��DMA���� 
	MYDMA_Enable(DMA1_Channel4);//��ʼһ��DMA���䣡 	
	POINT_COLOR=REDLCD;	  
	
 	while(1)
	{	
	
		camera_refresh();//������ʾ	
		
		
		if(USART3_RX_STA&0X8000)		//���յ�һ��������
		{
			rxlen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
			for(i=0;i<rxlen;i++)USART1_TX_BUF[i]=USART3_RX_BUF[i];	   
 			USART3_RX_STA=0;		   	//������һ�ν���
			USART1_TX_BUF[i]=0;			//�Զ���ӽ�����
			GPS_Analysis(&gpsx,(u8*)USART1_TX_BUF);//�����ַ���
			Gps_Msg_Show();				//��ʾ��Ϣ	
			//if(upload)printf("\r\n%s\r\n",USART1_TX_BUF);//���ͽ��յ������ݵ�����1
 		}

		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			MPUtemp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ����������� 
			if(t%5==0)
			{
				if(MPUtemp<0)
				{
					LCD_ShowChar(30+36,400,'-',12,0);		//��ʾ����
					MPUtemp=-MPUtemp;		//תΪ����
				}else LCD_ShowChar(30+36,400,' ',12,0);		//ȥ������ 
				LCD_ShowNum(30+36+8,400,MPUtemp/100,3,12);		//��ʾ��������	    
				LCD_ShowNum(30+36+30,400,MPUtemp%10,1,12);		//��ʾС������ 
				MPUtemp=pitch*10;
				if(MPUtemp<0)
				{
					LCD_ShowChar(150+36,400,'-',12,0);		//��ʾ����
					MPUtemp=-MPUtemp;		//תΪ����
				}else LCD_ShowChar(150+36,400,' ',12,0);		//ȥ������ 
				LCD_ShowNum(150+36+8,400,MPUtemp/10,3,12);		//��ʾ��������	    
				LCD_ShowNum(150+36+30,400,MPUtemp%10,1,12);		//��ʾС������ 
				MPUtemp=roll*10;
				if(MPUtemp<0)
				{
					LCD_ShowChar(30+36,410,'-',12,0);		//��ʾ����
					MPUtemp=-MPUtemp;		//תΪ����
				}else LCD_ShowChar(30+36,410,' ',12,0);		//ȥ������ 
				LCD_ShowNum(30+36+8,410,MPUtemp/10,3,12);		//��ʾ��������	    
				LCD_ShowNum(30+36+30,410,MPUtemp%10,1,12);		//��ʾС������ 
				MPUtemp=yaw*10;
				if(MPUtemp<0)
				{
					LCD_ShowChar(150+36,410,'-',12,0);		//��ʾ����
					MPUtemp=-MPUtemp;		//תΪ����
				}else LCD_ShowChar(150+36,410,' ',12,0);		//ȥ������ 
				LCD_ShowNum(150+36+8,410,MPUtemp/10,3,12);		//��ʾ��������	    
				LCD_ShowNum(150+36+30,410,MPUtemp%10,1,12);		//��ʾС������  
				t=0;
			}
			t++;
		}
		vTaskDelay(1);

#if INCLUDE_uxTaskGetStackHighWaterMark
        CameraTaskStack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}


const u8*LMODE_TBL[5]={"Auto","Sunny","Cloudy","Office","Home"};							//5�ֹ���ģʽ	    
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7����Ч 
extern u8 ov_sta;	//��exit.c�� �涨��
extern u8 ov_frame;	//��timer.c���涨��		 

//void camera_refresh(void)
//{
//	u32 j;
// 	u16 color;	 
//	if(ov_sta)//��֡�жϸ��£�
//	{
//		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������  
//		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//����ʾ�������õ���Ļ����
//		else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//����ʾ�������õ���Ļ����
//		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
//		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
//		OV7670_RCK_L;
//		OV7670_RCK_H;
//		OV7670_RCK_L;
//		OV7670_RRST=1;				//��λ��ָ����� 
//		OV7670_RCK_H;
//		for(j=0;j<76800;j++)
//		{
//			OV7670_RCK_L;
//			color=GPIOC->IDR&0XFF;	//������
//			OV7670_RCK_H; 
//			color<<=8;  
//			OV7670_RCK_L;
//			color|=GPIOC->IDR&0XFF;	//������
//			OV7670_RCK_H; 
//			LCD->LCD_RAM=color;    
//		}   							  
// 		ov_sta=0;					//����֡�жϱ��
//		ov_frame++; 
//		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
//		
//	} 
//}	 


void camera_refresh(void)
{
	//u32 j;
 	u16 color;
	u8 YUV_U;
//	u8 YUV_Y;
//	u16 BlackPos;
	
	if(ov_sta)//��֡�жϸ��£�
	{  
		uint32_t cnt = 2;
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������  
//		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//����ʾ�������õ���Ļ����
//		else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//����ʾ�������õ���Ļ����
//		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				
		OV7670_RCK_H;

		for(col=0;col<240;col++)
		{			
			if((col%2)==0)h++;
			for(row=0;row<320;row++)
			{ 	
					OV7670_RCK_L;	
					color=OV7670_DATA;	//������
					OV7670_RCK_H;

					OV7670_RCK_L;
					YUV_U=OV7670_DATA;	//������
					OV7670_RCK_H;

				if(((col%2)==0)&&((row%2)==0)) 
				{						
					    v++;
						//	Pic_Buff[v-1][h-1]=(u8)color;
						SendBuff[cnt++]  = (u8)color;		
					  
				}
			
				color<<=8;
				color_r = color&0xf800;
				color_b = color>>11;
				color >>=5;
				color_g =color&0x07e0; 		
				LCD->LCD_RAM= color_r + color_g + color_b;
		}
			v=0;
	}	
	
		h=0;
 		ov_sta=0;					//����֡�жϱ��
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
	} 
}	   

