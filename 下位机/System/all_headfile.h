#ifndef ALL_HEADERFILE_H
#define ALL_HEADERFILE_H


//##### define ######//
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "led.h"
#include "delay.h"
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* exact-width unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;

//##### CMSIS ######//
#include "stm32f10x.h"
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
//##### main ######//


//##### headware #####//
#include "mpu6050.h"
#include "mpuiic.h"
#include "dht11.h"
#include "led.h"
#include "gps.h"
#include "usart3.h"
#include "timer7.h"
#include "lcd.h"
#include "key.h"
#include "usart.h "
#include "sccb.h"	
#include "ov7670.h"
#include "ov7670cfg.h"
#include "usmart.h"
#include "adc.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "usart3.h"
#include "timer7.h"
#include "Pic_Process.h"
#include "dma.h"

//##### APP #####//
#include "start_task.h"



#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#endif


