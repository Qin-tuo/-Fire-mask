#ifndef _SENSOR_TASK_H
#define _SENSOR_TASK_H

#include "all_headfile.h"



extern u8 temperature;  	    
extern u8 humidity; 
extern u8 adcx;
extern u8 adcv;
extern u8 temp;
extern short MPUtemp;	

extern void SensorTask(void *pvParameters);
extern void Gps_Msg_Show(void);
extern int alert;

#endif





