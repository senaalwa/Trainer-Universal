#ifndef _ATMEGA_UART_
#define _ATMEGA_UART_

#include "atmega_config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define USART_BAUDRATE 57600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


void uart_init()
{
	UCSRB |= (1 << TXEN);/* Turn on transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */
	UBRRL = BAUD_PRESCALE;		/* Load lower 8-bits of the baud rate value */
	UBRRH = (BAUD_PRESCALE >> 8);	/* Load upper 8-bits*/
}

void uart_put(uint8_t data)
{
	while (!(UCSRA & (1 << UDRE)));
	UDR = data;
}

void uart_put_string(char * data)
{
	unsigned int i = 0;
	while(data[i] != 0)
	{
		uart_put(data[i]);
		i++;
	}
}

#endif