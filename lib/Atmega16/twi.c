#include "twi.h"

/* Define bit rate */
#define SCL_CLK 100000L  //define SCL clock speed
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))

void twi_init()			/* I2C initialize function */
{
    TWBR = BITRATE(TWSR=0x00);	/* Get bit rate register value by formula */
}

uint8_t twiStart(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); //send start
	while(!(TWCR & (1 << TWINT)));
	if((TWSR & 0xF8) != TW_START)
		return 0;
	return 1;
}

void twiStop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); //send stop
	while ((TWCR & (1 << TWSTO)));
	_delay_ms(1);
}

uint8_t twi_write_addres(uint8_t addres, uint8_t ack)
{
	TWDR = addres;
	TWCR = (1 << TWINT) | (1 << TWEN); /* clear interrupt to start transmission */

	while (!(TWCR & (1 << TWINT))); /* wait for transmission */

	if ((ack == ACK) && (TWSR & 0xF8) != TW_MT_SLA_ACK)
		return 0;

	return 1;
}

uint8_t twi_write_byte(uint8_t data, uint8_t ack)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN); /* clear interrupt to start transmission */

	while (!(TWCR & (1 << TWINT))); /* wait for transmission */

	if ((ack == ACK) && (TWSR & 0xF8) != TW_MT_DATA_ACK)
		return 0;

	return 1;
}

uint8_t twi_read_byte(uint8_t data, uint8_t ack)
{
	TWCR = ((ack == ACK)
	? ((1 << TWINT) | (1 << TWEN) | (1 << TWEA)) 
	: ((1 << TWINT) | (1 << TWEN))) ;

	while(!(TWCR & (1 << TWINT)));

	if(ack == ACK)
	{
		if((TWSR & 0xF8) != TW_MR_DATA_NACK)
		{
			return 0;
		}
	}
	else
	{
		if((TWSR & 0xF8) != TW_MR_DATA_ACK)
		{
			return 0;
		}
	}

	data = TWDR;

	return 1;
}

uint8_t write_reg(uint16_t addr, uint8_t reg, uint8_t dat)
{
	twiStart();

	if(twi_write_addres(addr, ACK))
		if(twi_write_byte(reg, ACK))
			if(twi_write_byte(dat, ACK))
			{
				twiStop();
				return 1;
			}

	twiStop();

	return 0;
}
