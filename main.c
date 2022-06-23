

#define F_CPU 11059200

#include "ov7670.h"
#include "uart.h"

// To turn off test pattern comment the test_pattern definition in ov7670_config.h

int main(void)
{
	uart_init(B9600);

	OV7670_init();

	while(1)
	{
		#ifdef QVGA
			capture_image(320, 240);
		#elif defined QQVGA
			capture_image(160, 120);
		#elif defined VGA
			capture_image(640, 480);
		#endif
	}

	return 0;
}