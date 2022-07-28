#ifndef _ATMEGA_UART_H_
#define _ATMEGA_UART_H_

#include "atmega_config.h"
#include <avr/io.h>

void uart_init();
void uart_put(uint8_t data);
void uart_put_string(char * data);

#endif