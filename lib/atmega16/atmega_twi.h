#ifndef _ATMEGA_TWI_H_
#define _ATMEGA_TWI_H_

#include "atmega_config.h"
#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>

/*#define ACK 1
#define NACK 0

void twi_init(void);
uint8_t twi_start(void);
void twi_stop(void);
uint8_t twi_write_addres(uint8_t addres, uint8_t ack);
uint8_t twi_write_byte(uint8_t data, uint8_t ack);
uint8_t twi_read_byte(uint8_t data, uint8_t ack);
uint8_t write_reg(uint16_t addr, uint8_t reg, uint8_t dat);*/

#define ACK 1
#define NACK 0

void twi_Init(void);
uint8_t twi_Start(void);
void twi_Stop(void);
uint8_t twi_write_addres(uint8_t addres, uint8_t ack);
uint8_t twi_write_byte(uint8_t data, uint8_t ack);
uint8_t twi_read_byte(uint8_t data, uint8_t ack);
uint8_t write_reg(uint8_t reg, uint8_t dat);
uint8_t rdReg(uint8_t reg);

#endif