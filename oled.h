#ifndef _OELD_H
#define _OELD_H
#include "LPC11xx.H"
void OLED_WrCmd(uint8_t  cmd);
void Dly_ms(uint16_t ms);
void OLED_WrDat(uint8_t data);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_Fill(uint8_t bmp_data);
void OLED_CLS(void);
void OLED_DLY_ms(uint16_t ms);
void GpioInit(void);
void OLED_PutPixel(uint8_t x,uint8_t y,uint8_t data1);
void OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif);
void OLED_P8x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
void OLED_P6x8Str(uint8_t x,uint8_t y,uint8_t ch[]);
void OLED_P14x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
void OLED_Print(uint8_t x, uint8_t y, uint8_t ch[]);
void OLED_PrintNum(uint8_t x,uint8_t y,uint8_t n);
#endif