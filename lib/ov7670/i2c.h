#ifndef _I2C_
#define _I2C_

#include "atmega_twi.h"
#include <avr/io.h>

inline static void i2c_init(void) __attribute__((always_inline));
inline static uint8_t i2c_write_reg(uint8_t reg, uint8_t data) __attribute__((always_inline));

inline static void i2c_init(void)
{
	twi_Init();
}

inline static uint8_t i2c_write_reg(uint8_t reg, uint8_t data)
{
	return write_reg(reg, data);
}

#endif
