#include "LPC11xx.h"                    // Device header
#include "oled.h"
#include "ZIKU.h"
#define OLED_DC_LOW()  LPC_GPIO2->DATA&=~(1<<3)
#define OLED_DC_HIGH() LPC_GPIO2->DATA|=(1<<3)
#define OLED_SDA_HIGH()  LPC_GPIO2->DATA|=(1<<2)
#define OLED_SDA_LOW()   LPC_GPIO2->DATA&=~(1<<2)
#define OLDE_SCL_HIGH()  LPC_GPIO2->DATA|=(1<<1)
#define OLED_SCL_LOW()   LPC_GPIO2->DATA&=~(1<<1)

#define X_WIDTH 128
#define Y_WIDTH 64






const uint8_t F14x16_Idx[] =
{
	"温度显示"
};
const uint8_t F14x16[] = 
{
	
0x00,0x20,0x04,0xF8,0x10,0xFC,0x94,0x94,0x94,0x94,0xFC,0x00,0x00,0x00,
0x00,0x00,0x7E,0x41,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x40,0x00,/*"?",0*/
/* (14 X 16 , ?? )*/

0x00,0x00,0xF8,0x48,0x48,0xE8,0x58,0x46,0x48,0xF8,0x48,0x28,0x00,0x00,
0x40,0x38,0x87,0x80,0x80,0x44,0x4D,0x35,0x35,0x4D,0x42,0x40,0x40,0x00,/*"?",1*/
/* (14 X 16 , ?? )*/

0x00,0x00,0x00,0xFC,0x94,0x94,0x94,0x94,0x94,0x94,0xFC,0x00,0x00,0x00,
0x00,0x00,0x24,0x38,0x20,0x3F,0x20,0x20,0x3F,0x30,0x2D,0x22,0x00,0x00,/*"?",2*/
/* (14 X 16 , ?? )*/

0x00,0x40,0x40,0x44,0x44,0x44,0xC4,0x44,0x44,0x44,0x44,0x44,0x40,0x00,
0x00,0x10,0x08,0x06,0x03,0x40,0x7F,0x00,0x00,0x02,0x04,0x18,0x10,0x00,/*"?",3*/
/* (14 X 16 , ?? )*/


};















void Dly_ms(uint16_t ms)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);				
	LPC_TMR16B0->IR  = 0x01;										
	LPC_TMR16B0->PR  = SystemCoreClock/1000-1;	
	LPC_TMR16B0->MR0 = ms;												
	LPC_TMR16B0->MCR = 0x03;										
	LPC_TMR16B0->TCR = 0x01;											
	NVIC_EnableIRQ (TIMER_16_0_IRQn );
	NVIC_SetPriority (TIMER_16_0_IRQn ,2);
}
void OLED_WrCmd(uint8_t  cmd)
{
	uint8_t i=8;
	
	
	OLED_DC_LOW();//LPC_GPIO2->DATA&=~(1<<3);	//OLED_DC=0    OLED_DC=0;;
    OLED_SCL_LOW();//LPC_GPIO2->DATA&=~(1<<1);	//OLED_SCL=0   OLED_SCL=0;;
	
	
	while(i--)
	  {
		if(cmd&0x80)
		{
			OLED_SDA_HIGH();//LPC_GPIO2->DATA|=(1<<2);	//OLED_SDA=1		//OLED_SDA=1;
		}
		else
		{
			OLED_SDA_LOW();//LPC_GPIO2->DATA&=~(1<<2);	//OLED_SDA=0    //OLED_SDA=0;
		}
		OLDE_SCL_HIGH();//LPC_GPIO2->DATA|=(1<<1);	//OLED_SCL=1	   //OLED_SCL=1;;

		OLED_SCL_LOW() ;//LPC_GPIO2->DATA&=~(1<<1);	//OLED_SCL=0   //OLED_SCL=0;;
		cmd<<=1;;
	  }

}
void OLED_WrDat(uint8_t data)
{
  uint8_t i=8;
//LCD_CS=0;;
OLED_DC_HIGH();// LPC_GPIO2->DATA|=(1<<3);//OLED_DC=1		//  OLED_DC=1;
OLED_SCL_LOW();//  LPC_GPIO2->DATA&=~(1<<1);//OLED_SCL=0     // OLED_SCL=0;
  //asm("nop");
  while(i--)
  {
    if(data&0x80)
	{
	OLED_SDA_HIGH();//LPC_GPIO2->DATA|=(1<<2);//OLED_SDA=1		//OLED_SDA=1;
	
	}
    else
	{
	 OLED_SDA_LOW();//LPC_GPIO2->DATA&=~(1<<2);//OLED_SDA=0    //OLED_SDA=0;
	}
    OLDE_SCL_HIGH();//LPC_GPIO2->DATA|=(1<<1);//OLED_SCL=1	   //OLED_SCL=1;
    //_asm("nop");;
		//asm("nop");
    OLED_SCL_LOW();// LPC_GPIO2->DATA&=~(1<<1);//OLED_SCL=0   //OLED_SCL=0;;
    data<<=1;
  }
	//LCD_CS=1;
}


void OLED_Set_Pos(uint8_t x, uint8_t y)
{
  OLED_WrCmd(0xb0+y);
  OLED_WrCmd(((x&0xf0)>>4)|0x10);
  OLED_WrCmd((x&0x0f)|0x01);
}

void OLED_Fill(uint8_t bmp_data)
{
	uint8_t y,x;

	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			OLED_WrDat(bmp_data);
	}
}

void OLED_CLS(void)
{
	uint8_t y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			OLED_WrDat(0);
	}
}

void OLED_DLY_ms(uint16_t ms)
{
//  u16 a;
//  while(ms)
//  {
//    a=16000;
//    while(a--);
//    ms--;
//  }
//  return;
	int times=1+ms/1000;
	
	while(times--)
	{
	LPC_TMR16B0->TCR = 0x02;		
	LPC_TMR16B0->PR  = SystemCoreClock/1000-1;		
	LPC_TMR16B0->MR0 = ms;	
	LPC_TMR16B0->TCR = 0x01;			  
	while (LPC_TMR16B0->TCR & 0x01);
	}
}


//
void OLED_Init(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<6);//使能时钟
	


	LPC_GPIO2->DIR|=(1<<1);
	LPC_GPIO2->DIR|=(1<<2);	
	LPC_GPIO2->DIR|=(1<<3);	
	
	OLDE_SCL_HIGH();//LPC_GPIO2->DATA|=(1<<1);//OLED_SCL=1  //
	OLED_DC_HIGH();//LPC_GPIO2->DATA|=(1<<3);//OLED_DC=1
	OLED_SDA_HIGH();//LPC_GPIO2->DATA|=(1<<2);//OLED_SDA=1
    

	OLDE_SCL_HIGH();;//LPC_GPIO2->DATA|=(1<<1);//OLED_SCL=1	   //OLED_SCL=1;


  OLED_WrCmd(0xae);//--turn off oled panel
  OLED_WrCmd(0x00);//---set low column address
  OLED_WrCmd(0x10);//---set high column address
  OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WrCmd(0x81);//--set contrast control register
  OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
  OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  OLED_WrCmd(0xa6);//--set normal display
  OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  OLED_WrCmd(0x3f);//--1/64 duty
  OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WrCmd(0x00);//-not offset
  OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WrCmd(0xd9);//--set pre-charge period
  OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WrCmd(0xda);//--set com pins hardware configuration
  OLED_WrCmd(0x12);
  OLED_WrCmd(0xdb);//--set vcomh
  OLED_WrCmd(0x40);//Set VCOM Deselect Level
  OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WrCmd(0x02);//
  OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
  OLED_WrCmd(0x14);//--set(0x10) disable
  OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
  OLED_WrCmd(0xaf);//--turn on oled panel
  OLED_Fill(0x00);  //初始清屏
  OLED_Set_Pos(0,0);

}


//OLED打印矩形

void OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif)
{
	uint8_t n;

	OLED_Set_Pos(x1,y1>>3);
	for(n=x1;n<=x2;n++)
	{
		OLED_WrDat(0x01<<(y1%8));
		if(gif == 1) 	OLED_DLY_ms(50);
	}
	OLED_Set_Pos(x1,y2>>3);
  for(n=x1;n<=x2;n++)
	{
		OLED_WrDat(0x01<<(y2%8));
		if(gif == 1) 	OLED_DLY_ms(5);
	}
}


//OLED输出6x8字符
//显示的位置（x,y），x为点范围0~127，y为页范围0～7
void OLED_P6x8Str(uint8_t x,uint8_t y,uint8_t ch[])
{
  uint8_t c=0,i=0,j=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>X_WIDTH){x=0;y++;}
    OLED_Set_Pos(x,y);
  	for(i=0;i<6;i++)
  	  OLED_WrDat(F6x8[c][i]);
  	x+=6;
  	j++;
  }
}


//OLED输出8x16字符
//显示的位置（x,y），x为点范围0~127，y为页范围0～7
void OLED_P8x16Str(uint8_t x,uint8_t y,uint8_t ch[])
{
  uint8_t c=0,i=0,j=0;

  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>X_WIDTH){x=0;y++;}
    OLED_Set_Pos(x,y);
  	for(i=0;i<8;i++)
  	  OLED_WrDat(F8X16[c*16+i]);
  	OLED_Set_Pos(x,y+1);
  	for(i=0;i<8;i++)
  	  OLED_WrDat(F8X16[c*16+i+8]);
  	x+=8;
  	j++;
  }
}

//OLED输出汉字字符串
//显示的位置（x,y），x为点范围0~127，y为页范围0～7
void OLED_P14x16Str(uint8_t x,uint8_t y,uint8_t ch[])
{
	uint8_t wm=0,ii = 0;
	uint16_t adder=1;

	while(ch[ii] != '\0')
	{
  	wm = 0;
  	adder = 1;
  	while(F14x16_Idx[wm] > 127)
  	{
  		if(F14x16_Idx[wm] == ch[ii])
  		{
  			if(F14x16_Idx[wm + 1] == ch[ii + 1])
  			{
  				adder = wm * 14;
  				break;
  			}
  		}
  		wm += 2;
  	}
  	if(x>118){x=0;y++;}
  	OLED_Set_Pos(x , y);
  	if(adder != 1)// 显示汉字
  	{
  		OLED_Set_Pos(x , y);
  		for(wm = 0;wm < 14;wm++)
  		{
  			OLED_WrDat(F14x16[adder]);
  			adder += 1;
  		}
  		OLED_Set_Pos(x,y + 1);
  		for(wm = 0;wm < 14;wm++)
  		{
  			OLED_WrDat(F14x16[adder]);
  			adder += 1;
  		}
  	}
  	else			  //显示空白字符
  	{
  		ii += 1;
      OLED_Set_Pos(x,y);
  		for(wm = 0;wm < 16;wm++)
  		{
  				OLED_WrDat(0);
  		}
  		OLED_Set_Pos(x,y + 1);
  		for(wm = 0;wm < 16;wm++)
  		{
  				OLED_WrDat(0);
  		}
  	}
  	x += 14;
  	ii += 2;
	}
}

////////////////////////////////////////////////////////
//OLED输出混合文字
//显示的位置（x,y），x为点范围0~127，y为页范围0～7
////////////////////////////////////////////////////////
void OLED_Print(uint8_t x, uint8_t y, uint8_t ch[])
{
	uint8_t ch2[3];
	uint8_t ii=0;
	while(ch[ii] != '\0')
	{
		if(ch[ii] > 127)
		{
			ch2[0] = ch[ii];
	 		ch2[1] = ch[ii + 1];
			ch2[2] = '\0';			//汉字为两个字节
			OLED_P14x16Str(x , y, ch2);	//显示汉字
			x += 14;
			ii += 2;
		}
		else
		{
			ch2[0] = ch[ii];
			ch2[1] = '\0';			//字母占一个字节
			OLED_P6x8Str(x , y , ch2);	//显示字母
			x += 8;
			ii+= 1;
		}
	}
}

//////////////////////////////////////////////////////////
////OLED打印图片
////显示的位置（x,y），x为点范围0~127，y为页范围0～7
//////////////////////////////////////////////////////////
void Draw_BMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[])
{
  uint16_t ii=0;
  uint8_t x,y;

  if(y1%8==0) y=y1/8;
  else y=y1/8+1;
	for(y=y0;y<=y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
		{
			OLED_WrDat(bmp[ii++]);
		}
	}
}


////////////////////////////////////////////////////////
//OLED打印一个数
//显示的位置（x,y），x为点范围0~127，y为页范围0～7
////////////////////////////////////////////////////////
void OLED_PrintNum(uint8_t x,uint8_t y,uint8_t n)
{
	uint8_t m[20];
	sprintf(m,"%d",n);
	OLED_Print(x,y,m);
}


////////////////////////////////////////////////////////
//OLED打印指定图片
//显示的位置（x,y），x为点范围0~127，y为页范围0～7
////////////////////////////////////////////////////////
void OLED_Img(uint8_t x,uint8_t y,uint8_t n)
{
	
	
	if(n==1)
	{
		Draw_BMP(x,y,x+32,y+3,templogo);
	}
	if(n==2)
	{
		Draw_BMP(x,y,x+32,y+3,wetlogo);
	}
	if(n==3)
	{
		Draw_BMP(x,y,x+32,y+3,setlogo);
	}
	if(n==4)
	{
		Draw_BMP(x,y,x+32,y+3,aboutlogo);
	}
	if(n==5)
	{
		Draw_BMP(x,y,x+32,y+3,locklogo);
	}
	if(n==6)
	{
		Draw_BMP(x,y,x+32,y+3,sleepsmalllogo);
	}
	if(n==7)
	{
		Draw_BMP(39,2,87,7,sleeplargelogo);
	}
	if(n==8)
	{
		Draw_BMP(x,y,x+32,y+3,setpdlogo);
	}
	if(n==9)
	{
		Draw_BMP(x,y,x+32,y+3,settemplogo);
	}	
	if(n==10)
	{
		Draw_BMP(x,y,x+32,y+3,setwetlogo);
	}
	if(n==11)
	{
		if(y==1)Draw_BMP(119,2,127,7,cho1);
		if(y==2)Draw_BMP(119,2,127,7,cho2);
		if(y==3)Draw_BMP(119,2,127,7,cho3);
		if(y==4)Draw_BMP(119,2,127,7,cho4);
		if(y==5)Draw_BMP(119,2,127,7,cho5);
		if(y==6)Draw_BMP(119,2,127,7,cho6);		
	}
	if(n==12)
	{
		Draw_BMP(x,y,x+32,y+3,wet2logo);
	}
	if(n==13)
	{
		Draw_BMP(x,y,x+32,y+3,wet3logo);
	}
	if(n==14)
	{
		Draw_BMP(x,y,x+32,y+3,blocklogo);
	}
}
int main(void)
{
	OLED_Init();
	while(1)
	{
   OLED_P14x16Str(10,5,"温度");
		OLED_P14x16Str(10,2,"显示");
		
	}

}