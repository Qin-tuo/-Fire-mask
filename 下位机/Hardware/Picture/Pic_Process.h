#ifndef __PIC_PROCESS_H
#define __PIC_PROCESS_H
#include "sys.h"

#define V 160
#define H 180
#define THRESHOLD 0x6a

void Image_Binaryzation(void);
void Image_Filter(void);
void Get_Black_Line(void);
u8 abs_xy(int i);
void Black_Line_handle(void);

#endif

