#include "Pic_Process.h"

u8 Black_Line[60]={0};

u8 Pic_Buff[V][H];

u16 middle_dot;

u8 Search_Cnt=1;


/*-----	图像二值化函数-----*/
void Image_Binaryzation(void)
{   
    int i,j;


    for(i=0;i<H;i++)
      {
        for(j=0;j<V;j++)
        {
          if(Pic_Buff[j][i]<THRESHOLD)
            Pic_Buff[j][i]=0x00;
          else
            Pic_Buff[j][i]=0xfe;   //发送到上位机上时候用0xfe

         }
       }
}

/*-----	图像中值滤波函数【1*3】-----*/
void Image_Filter(void)
{
  int i,j;

   for(i=0;i<H;i++)
	{
	 for(j=0;j<V-2;j++)
	  {
	   if(Pic_Buff[j][i]==0xfe&&Pic_Buff[j+1][i]==0&&Pic_Buff[j+2][i]==0xfe)         
		   Pic_Buff[j][i]=0xfe;
	   else if(Pic_Buff[j][i]==0&&Pic_Buff[j+1][i]==0xfe&&Pic_Buff[j+2][i]==0)     
		   Pic_Buff[j][i]=0x00;

     }
   }
	
}

/*-----	寻找黑线位置函数-----*/
void Get_Black_Line(void)			//寻找黑线位置
{
  u8 i,j;

  for(i=0;i<60;i++)
	{
		for(j=79;j>2;j--) //
    {
			if((Pic_Buff[j][i]==0x00)&&(Pic_Buff[j-1][i]==0x00))
			{                       //若一开始检测连续2点都为黑点，则认为检测到黑线,设置为75
					Black_Line[i]=75;
					Search_Cnt++;			  //检测到黑点，计数（求平均时候用）
					break;
			}
			else if((Pic_Buff[j][i]==0xfe)&&(Pic_Buff[j-1][i]==0xfe)&&(Pic_Buff[j-2][i]==0x00)&&(Pic_Buff[j-3][i]==0x00))//刚开始检测由白色到黑色的边沿跳变
      {
          Black_Line[i]=j-2;
					Search_Cnt++;				//检测到黑点，计数（求平均时候用）
          break;
				
      }
			else Black_Line[i]=0;   //未扫描到连续黑点，则默认边界为0
		}
		
	}
	
}

/*-----	取绝对值函数-----*/
u8 abs_xy(int i)
  {      /* compute absolute value of int argument */
    return (i < 0 ? -i : i);
  }

/*-----黑线位置处理函数，获取中心值---*/
void Black_Line_handle(void)
{
  u8 i;
  for(i=0;i<60;i++)
  {
    //判断黑线位置是否连续
//    if(abs_xy(Black_Line[i]-Black_Line[i+3])>5)
//    {
//      Black_Line[i]=Black_Line[i+3];
//    }
		middle_dot+=Black_Line[i];
	}	
		middle_dot=middle_dot/Search_Cnt;// 取平均
	 
}




