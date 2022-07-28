#include "atmega_config.h"
#include "ov7670.h"
#include "atmega_uart.h"
#include "i2c.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "lcd_ili9486.h"

//define resolution selection 
#define VGA //QQVGA, QVGA, VGA

int main(void)
{	
	// cli();
	uart_init();
	OV7670_init();

	Lcd_Init();
	Fill_Screen(WHITE);
	while(1)
	{
		#ifdef QVGA
			capture_image(320, 240);
		#elif defined QQVGA
		//uart_put_string("1");
			capture_image(160, 120);
		#elif defined VGA
			capture_image(640, 480);
		#endif
	
	}

	return 0;
}