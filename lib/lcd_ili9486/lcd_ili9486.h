#ifndef _LCD_ILI9486_H_
#define _LCD_ILI9486_H_

#include "atmega_config.h"
#include <stdbool.h>

//define some colour values
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void Push_Command(uint16_t cmd, uint8_t *block, int8_t N);
void Set_Addr_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void init_table8(const void *table, int16_t size);
void Set_Rotation(uint8_t r);
void Lcd_Init(void);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
void Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
void Draw_Pixe(int16_t x, int16_t y, uint16_t color);
void Fill_Rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void Fill_Screen(uint16_t color);
void Set_Text_Mode(bool mode);
void Set_Text_Size(uint8_t s);
void Set_Text_Cousur(int16_t x, int16_t y);
void Set_Text_colour(uint16_t color);
void Set_Text_Back_colour(uint16_t color);
void Fill_ScreenRGB(uint8_t r, uint8_t g, uint8_t b);
void Draw_Char(int16_t x, int16_t y, uint8_t c, uint16_t color,uint16_t bg, uint8_t size, bool mode);
void Print_String(const char *st, int16_t x, int16_t y);

#endif