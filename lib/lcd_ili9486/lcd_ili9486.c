#ifndef _LCD_ILI9486_
#define _LCD_ILI9486_

#include "atmega_config.h"
#include "atmega_io.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "LCDWIKI_font.c"
#include "lcd_ili9486.h"

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998
#define WIDTH 480
#define HEIGHT 320

//
#define CC 0x2C
#define XC 0x2A //COLADDRSET
#define YC 0x2B //PAGEADDRSET
#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04
#define RC 0x2E
#define SC1 0x33
#define SC2 0x37
#define MD 0x36
#define VL 0
#define R24BIT 0

#define TFTLCD_DELAY16  0xFFFF
#define TFTLCD_DELAY8   0x7F
#define MAX_REG_NUM     24

// Set value of TFT register: 16-bit address, 16-bit value
// See notes at top about macro expansion, hence hi & lo temp vars
//#define writeCmdData16(a, d) {uint8_t hi, lo; hi = (a)>>8; lo = (a); CD_COMMAND; write8(hi); write8(lo); hi = (d)>>8; lo = (d); CD_DATA; write8(hi); write8(lo);}
#define writeCmdData16(a, d) {CD_COMMAND; write16(a); CD_DATA; write16(d);}

uint16_t text_color, text_bgcolor, draw_color, width, height, rotation;
uint8_t text_size;
int16_t text_x, text_y;
bool text_mode = 0;

void Push_Command(uint16_t cmd, uint8_t *block, int8_t N)
{
  	CS_ACTIVE;
    writeCmd16(cmd);
    while (N-- > 0) 
	{
    uint8_t u8 = *block++;
    writeData8(u8);
}
CS_IDLE;
}

void Set_Addr_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	CS_ACTIVE;
      uint8_t x_buf[] = {x1>>8,x1&0xFF,x2>>8,x2&0xFF}; 
      uint8_t y_buf[] = {y1>>8,y1&0xFF,y2>>8,y2&0xFF}; 
    
      Push_Command(XC, x_buf, 4); //set x address
      Push_Command(YC, y_buf, 4); //set y address
	CS_IDLE;		
}

void init_table8(const void *table, int16_t size)
{
	uint8_t i;
    uint8_t *p = (uint8_t *) table, dat[MAX_REG_NUM];            //R61526 has GAMMA[22] 
    while (size > 0) 
	{
        uint8_t cmd = pgm_read_byte(p++);
        uint8_t len = pgm_read_byte(p++);
        if (cmd == TFTLCD_DELAY8) 
		{
            _delay_ms(len);
            len = 0;
        } 
		else 
		{
            for (i = 0; i < len; i++)
            {
                dat[i] = pgm_read_byte(p++);
            }
			Push_Command(cmd,dat,len);
        }
        size -= len + 2;
    }
}


void Set_Rotation(uint8_t r)
{
    rotation = r & 3;           // just perform the operation ourselves on the protected variables
    width = (rotation & 1) ? HEIGHT : WIDTH;
    height = (rotation & 1) ? WIDTH : HEIGHT;
	  CS_ACTIVE;
		uint8_t val;
		switch (rotation) 
		{
		   	case 0:
		     	val = ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR; //0 degree 
		     	break;
		   	case 1:
		     	val = ILI9341_MADCTL_MV | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR ; //90 degree 
		     	break;
		 	case 2:
		    	val = ILI9341_MADCTL_MY |ILI9341_MADCTL_BGR; //180 degree 
		    	break;
		   	case 3:
		     	val = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR; //270 degree
		     	break;
		 }
		 writeCmdData8(MD, val); 
	}

void Lcd_Init()
{
  SET_OUTPUT;
  SET_PORT;
  SET_OUTPUT_PINMASK;
  CS_IDLE;
  WR_IDLE;
  CD_DATA;
  
  static const uint8_t ILI9486_regValues[] PROGMEM = 
  {
    0xF1, 6, 0x36, 0x04, 0x00, 0x3C, 0x0F, 0x8F,
    0xF2, 9, 0x18, 0xA3, 0x12, 0x02, 0xB2, 0x12, 0xFF, 0x10, 0x00, 
    0xF8, 2, 0x21, 0x04,
    0xF9, 2, 0x00, 0x08,
    0x36, 1, 0x08, 
    0xB4, 1, 0x00,
    0xC1, 1, 0x41,
    0xC5, 4, 0x00, 0x91, 0x80, 0x00,
    0xE0, 15, 0x0F, 0x1F, 0x1C, 0x0C, 0x0F, 0x08, 0x48, 0x98, 0x37, 0x0A, 0x13, 0x04, 0x11, 0x0D, 0x00,
    0xE1, 15, 0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10 ,0x03, 0x24, 0x20, 0x00,				
    0x3A, 1, 0x55,
    0x11,0,
    0x36, 1, 0x28,
    TFTLCD_DELAY8, 120,
    0x29,0 
  };
  init_table8(ILI9486_regValues, sizeof(ILI9486_regValues));
  Set_Rotation(1);
  width = WIDTH;
  height = HEIGHT;
}

void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
{	
  unsigned int i,j;
  writeCmd8(0x02c); //write_memory_start
  CD_DATA;
  CS_ACTIVE;
  l=l+x;
  Set_Addr_Window(x,y,l,y);
  j=l*2;
  for(i=1;i<=j;i++)
  {
    writeData8(c);
  }
  CS_IDLE;
}

void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
{	
  unsigned int i,j;
  writeCmd8(0x02c); //write_memory_start
  CD_DATA;
  CS_ACTIVE;
  l=l+y;
  Set_Addr_Window(x,y,x,l);
  j=l*2;
  for(i=1;i<=j;i++)
  { 
    writeData8(c);
  }
  CS_IDLE; 
}

void Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}

void Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  unsigned int i;
  for(i=0;i<h;i++)
  {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}

int Color_To_565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

//KBV
//set x,y  coordinate and color to draw a pixel point 
void Draw_Pixe(int16_t x, int16_t y, uint16_t color)
{
	if((x < 0) || (y < 0) || (x > WIDTH) || (y > HEIGHT))
	{
		return;
	}
	Set_Addr_Window(x, y, x, y);
	CS_ACTIVE;
	writeCmdData16(CC, color);
	CS_IDLE;
}

//fill area from x to x+w,y to y+h
void Fill_Rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	int16_t end;
	if (w < 0) 
	{
        w = -w;
        x -= w;
    }                           //+ve w
    end = x + w;
    if (x < 0)
    {
        x = 0;
    }
    if (end > WIDTH)
    {
        end = WIDTH;
    }
    w = end - x;
    if (h < 0) 
	{
        h = -h;
        y -= h;
    }                           //+ve h
    end = y + h;
    if (y < 0)
    {
        y = 0;
    }
    if (end > HEIGHT)
    {
        end = HEIGHT;
    }
    h = end - y;
    Set_Addr_Window(x, y, x + w - 1, y + h - 1);//set area
	CS_ACTIVE;
	writeCmd8(CC);
  if (h > w) 
	{
        end = h;
        h = w;
        w = end;
    }
	while (h-- > 0) 
	{
		end = w;
		do 
		{
   			writeData16(color);//set color data
        } while (--end != 0);
	}	
	CS_IDLE;
}

//GUI
//Fill the full screen with color
void Fill_Screen(uint16_t color)
{
	Fill_Rect(0, 0, WIDTH, HEIGHT, color);
}

void Set_Text_Mode(bool mode)
{
	text_mode = mode;
}

//set text size
void Set_Text_Size(uint8_t s)
{
	text_size = s;
}

void Set_Text_Cousur(int16_t x, int16_t y)
{
	text_x = x;
	text_y = y;
}

//set text colour with 16bit color
void Set_Text_colour(uint16_t color)
{
	text_color = color;
}

void Set_Text_Back_colour(uint16_t color)
{
	text_bgcolor = color;	
}

void Draw_Char(int16_t x, int16_t y, uint8_t c, uint16_t color,uint16_t bg, uint8_t size, bool mode)
{
	if((x >= WIDTH) || (y >= HEIGHT) || ((x + 6 * size - 1) < 0) || ((y + 8 * size - 1) < 0))
	{
    	return;
	}		
  	if(c >= 176)
  	{
		c++; 
  	}
  int8_t i;
	for (i=0; i<6; i++) 
	{
    	uint8_t line;
    	if (i == 5)
    	{
      		line = 0x0;
    	}
    	else
    	{
      		line = pgm_read_byte(lcd_font+(c*5)+i);
    	}
      int8_t j;
    	for (j = 0; j<8; j++) 
		{
      		if (line & 0x1) 
			{ 
        		if (size == 1)
        		{
        			Draw_Pixe(x+i, y+j, color);
        		}
        		else 
				{  
					Fill_Rect(x+(i*size), y+(j*size), size, size, color);
        		}
        	} 
			else if (bg != color) 				
			{
				if(!mode)
				{
	        		if (size == 1) 
	        		{
	        			Draw_Pixe(x+i, y+j, bg);
	        		}
	        		else 
					{  
						Fill_Rect(x+i*size, y+j*size, size, size, bg);
					}
				}
			}
      		line >>= 1;
    	}
    }
}

//write a char
size_t write(uint8_t c)
{
	if (c == '\n') 
	{
    	text_y += text_size*8;
    	text_x = 0;
 	} 
	else if(c == '\r')
	{
	}
	else 
	{
    	Draw_Char(text_x, text_y, c, text_color, text_bgcolor, text_size, text_mode);
    	text_x += text_size*6;		
  }	
  	return 1;	
}

//print string
size_t Print(uint8_t *st, int16_t x, int16_t y)
{
	int16_t pos;
	uint16_t len;
	const char *p = (const char *)st;
	size_t n = 0;
	if (x == CENTER || x == RIGHT) 
	{
		len = strlen((const char *)st) * 6 * text_size;		
		pos = (WIDTH - len); 
		if (x == CENTER)
		{
			x = pos/2;
		}
		else
		{
			x = pos - 1;
		}
	}
    Set_Text_Cousur(x, y);
	while(1)
	{
    unsigned char ch = *(p++);//pgm_read_byte(p++);
		if(ch == 0)
		{
			break;
		}
		if(write(ch))
		{
			n++;
		}
		else
		{
			break;
		}
	}	
	return n;
}

//print string
void Print_String(const char *st, int16_t x, int16_t y)
{
	Print((uint8_t *)st, x, y);
}

#endif