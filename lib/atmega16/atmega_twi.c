#ifndef _ATMEGA_TWI_
#define _ATMEGA_TWI_

#include "atmega_config.h"
#include "atmega_twi.h"
#include "ov7670.h"
#include <math.h>	
#include "atmega_uart.h"

/* Define bit rate */
#define SCL_CLK 100000L  //define SCL clock speed
//#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))

void twi_Init()			/* I2C initialize function */
{
    TWSR = 0;
	TWBR = 47;

	//TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
	//TWBR = BITRATE(TWSR=0x00);	/* Get bit rate register value by formula */
}

uint8_t twi_Start(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); //send start
	while(!(TWCR & (1 << TWINT)));
	if((TWSR & 0xF8) != TW_START)
		return 0;
	return 1;
}

void twi_Stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); //send stop
	//while ((TWCR & (1 << TWSTO))); /* Wait until stop condition execution */
	_delay_ms(1);
}

uint8_t twi_write_addres(uint8_t addres, uint8_t typeTWI)
{
	TWDR = addres;
	TWCR = (1 << TWINT) | (1 << TWEN); /* clear interrupt to start transmission */
	while (!(TWCR & (1 << TWINT))); /* wait for transmission */
	if ((TWSR & 0xF8) != typeTWI)
		return 0;
	return 1;
}

uint8_t twi_write_byte(uint8_t data, uint8_t type)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN); /* clear interrupt to start transmission */
	// uart_put_string("TWCR");
	while (!(TWCR & (1 << TWINT)));  /*wait for transmission */
	if ((TWSR & 0xF8) != type)
		return 0;
	return 1;
	}

uint8_t write_reg(uint8_t reg, uint8_t dat)
{
	twi_Start();
	twi_write_addres(0x42, TW_MT_SLA_ACK);
  	twi_write_byte(reg, TW_MT_DATA_ACK);
  	twi_write_byte(dat, TW_MT_DATA_ACK);
	// char array[100];
	// 	memset(array, 0, 100);
	// 	sprintf(array, "write addr: %x", addr);
	// 	uart_put_string(array);

	// 	memset(array, 0, 100);
	// 	sprintf(array, " write reg: %x", reg);
	// 	uart_put_string(array);

	// 	memset(array, 0, 100);
	// 	sprintf(array, " write data: %x\n", dat);
	// 	uart_put_string(array);
	twi_Stop();

	return 0;
}

static uint8_t twiRd(uint8_t nack){
	if (nack){
		TWCR = _BV(TWINT) | _BV(TWEN);
		while ((TWCR & _BV(TWINT)) == 0); /* wait for transmission */
		if ((TWSR & 0xF8) != TW_MR_DATA_NACK)
			return 0;
		return TWDR;
	}
	else{
		TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
		while ((TWCR & _BV(TWINT)) == 0); /* wait for transmission */
		if ((TWSR & 0xF8) != TW_MR_DATA_ACK)
			return 0;
		return TWDR;
	}
}

uint8_t rdReg(uint8_t reg){
	uint8_t dat;
	uart_put_string("rdReg ");
	twi_Start();
	twi_write_addres(0x42, TW_MT_SLA_ACK);
	twi_write_byte(reg, TW_MT_DATA_ACK);
	twi_Stop();
	twi_Start();
	twi_write_addres(0x43, TW_MR_SLA_ACK);
	dat = twiRd(1);
	twi_Stop();
	return dat;
}
#endif