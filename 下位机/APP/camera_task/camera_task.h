#ifndef _CAMERA_TASK_H
#define _CAMERA_TASK_H

#include "sccb.h"	
#include "ov7670.h"
#include "ov7670cfg.h"
#include "exti.h"
#include "timer6.h"
#include "lcd.h"
#include "usmart_str.h"
#include "usmart.h"		
#include "delay.h"
#include "sys.h"
#include "all_headfile.h"
#include "sensor_task.h"
#include "all_headfile.h"

extern void CameraTask(void *pvParameters);
void camera_refresh(void);

extern float pitch,roll,yaw; 		//Å·À­½Ç
extern float longitude,latitude,altitude;
#endif
