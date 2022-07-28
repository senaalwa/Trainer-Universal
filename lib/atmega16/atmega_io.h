#ifndef _ATMEGA_IO_H_
#define _ATMEGA_IO_H_
/*
 * Bismillah 
*/
#include "atmega_config.h"
#include <avr/io.h>

// LCD 
// CS RS WR PA567
#define SET_OUTPUT {DDRB |= 0x1B; DDRD |= 0x71;}
#define SET_OUTPUT_PINMASK {DDRA |= 0xE0;}
#define SET_PORT {PORTB |= 0x1B; PORTD |= 0x71;}

#define WR_ACTIVE  PORTA &= ~(1 << 7)
#define WR_IDLE    PORTA |=  (1 << 7)
#define CD_COMMAND PORTA &= ~(1 << 6)
#define CD_DATA    PORTA |=  (1 << 6)
#define CS_ACTIVE  PORTA &= ~(1 << 5)
#define CS_IDLE    PORTA |=  (1 << 5)

#define BMASK 0x1B
#define DMASK 0x71

#define WR_STROBE {WR_ACTIVE; WR_IDLE;}
// Write 8-bit value to LCD data lines
#define write8(d) { PORTB = (PORTB & ~BMASK) | (((d) & 0x03) | (((d)&(0x03<<2))<<1)); PORTD = (PORTD & ~DMASK) | ((((d)&(0X07<<5))>>1) | (((d)&(0x01<<4))>>4)); WR_STROBE; } 
#define writeData8(x){ CD_DATA; write8(x); }
#define writeCmd8(x){ CD_COMMAND; write8(x); }
#define writeCmdData8(a, d) { CD_COMMAND; write8(a); CD_DATA; write8(d); }
#define write16(d) { uint8_t h = (d)>>8, l = d; write8(h); write8(l); }
#define writeCmd16(x){ CD_COMMAND; write16(x);}
#define writeData16(x){ CD_DATA; write16(x) }

// OV7670
// IO PINS
#define XCLK_DDR DDRD
#define XCLK PD7 // timer OC2

#define VSYNC_DDR DDRD
#define VSYNC_PIN PIND
#define VSYNC PD2 // interrupt 0

#define PCLK_DDR DDRD
#define PCLK_PIN PIND
#define PCLK PD3 // interrupt 1

#define HREF_DDR DDRB
#define HREF_PIN PINB
#define HREF PB7

#define DATA_DDR { DDRA &= ~(0x0F); DDRC &= ~(0xF0); } //set DDRA 0~3 & DDRC 4~7 as input register
#define DATA_PIN (PINA & (0x0F))|(PINC & (0xF0));


inline static void set_data_in(void) __attribute__((always_inline));
inline static uint8_t get_data(void) __attribute__((always_inline));
inline static void set_pclk_in(void) __attribute__((always_inline));
inline static uint8_t get_pclk(void) __attribute__((always_inline));
inline static void set_vsync_in(void) __attribute__((always_inline));
inline static uint8_t get_vsync(void) __attribute__((always_inline));
inline static void set_href_in(void) __attribute__((always_inline));
inline static uint8_t get_href(void) __attribute__((always_inline));
inline static void init_xclk(void) __attribute__((always_inline));

inline static void set_data_in(void)
{
	DATA_DDR;
}

inline static uint8_t get_data(void)
{
	return DATA_PIN;
}

inline static void set_pclk_in(void)
{
	PCLK_DDR &= ~(1 << PCLK);
}

inline static uint8_t get_pclk(void)
{
	return PCLK_PIN & (1 << PCLK);
}

inline static void set_vsync_in(void)
{
	VSYNC_DDR &= ~(1 << VSYNC);
}

inline static uint8_t get_vsync(void)
{
	return VSYNC_PIN & (1 << VSYNC);
}

inline static void set_href_in(void)
{
	HREF_DDR &= ~(1 << HREF);
}

inline static uint8_t get_href(void)
{
	return HREF_PIN & (1 << HREF);
}

inline static void init_xclk(void)
{
	XCLK_DDR |= (1 << XCLK);
	TCNT2 = 0;
	OCR2 = 1;
	TCCR2 = (1<<WGM21) | (1<<COM20) | (1<<CS20);
}

#endif