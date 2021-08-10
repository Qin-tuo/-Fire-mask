#include "Pic_Process.h"

u8 Black_Line[60]={0};

u8 Pic_Buff[V][H];

u16 middle_dot;

u8 Search_Cnt=1;


/*-----	ͼ���ֵ������-----*/
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
            Pic_Buff[j][i]=0xfe;   //���͵���λ����ʱ����0xfe

         }
       }
}

/*-----	ͼ����ֵ�˲�������1*3��-----*/
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

/*-----	Ѱ�Һ���λ�ú���-----*/
void Get_Black_Line(void)			//Ѱ�Һ���λ��
{
  u8 i,j;

  for(i=0;i<60;i++)
	{
		for(j=79;j>2;j--) //
    {
			if((Pic_Buff[j][i]==0x00)&&(Pic_Buff[j-1][i]==0x00))
			{                       //��һ��ʼ�������2�㶼Ϊ�ڵ㣬����Ϊ��⵽����,����Ϊ75
					Black_Line[i]=75;
					Search_Cnt++;			  //��⵽�ڵ㣬��������ƽ��ʱ���ã�
					break;
			}
			else if((Pic_Buff[j][i]==0xfe)&&(Pic_Buff[j-1][i]==0xfe)&&(Pic_Buff[j-2][i]==0x00)&&(Pic_Buff[j-3][i]==0x00))//�տ�ʼ����ɰ�ɫ����ɫ�ı�������
      {
          Black_Line[i]=j-2;
					Search_Cnt++;				//��⵽�ڵ㣬��������ƽ��ʱ���ã�
          break;
				
      }
			else Black_Line[i]=0;   //δɨ�赽�����ڵ㣬��Ĭ�ϱ߽�Ϊ0
		}
		
	}
	
}

/*-----	ȡ����ֵ����-----*/
u8 abs_xy(int i)
  {      /* compute absolute value of int argument */
    return (i < 0 ? -i : i);
  }

/*-----����λ�ô���������ȡ����ֵ---*/
void Black_Line_handle(void)
{
  u8 i;
  for(i=0;i<60;i++)
  {
    //�жϺ���λ���Ƿ�����
//    if(abs_xy(Black_Line[i]-Black_Line[i+3])>5)
//    {
//      Black_Line[i]=Black_Line[i+3];
//    }
		middle_dot+=Black_Line[i];
	}	
		middle_dot=middle_dot/Search_Cnt;// ȡƽ��
	 
}




