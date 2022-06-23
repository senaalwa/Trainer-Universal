#ifndef _ATMEGA16_IO_H_
#define _ATMEGA16_IO_H_

/*
 * Bismillah 
*/
#include "atmega_config.h"
#include <avr/io.h>

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
	// XCLK 8 MHz
	XCLK_DDR |= (1 << XCLK);
	TCNT2 = 0;
	OCR2 = 1;
	TCCR2 = (1<<WGM21) | (1<<COM20) | (1<<CS20);
}

#endif
