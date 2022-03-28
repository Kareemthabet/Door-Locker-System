/******************************************************************************
 *
 * Module: I2C
 *
 * File Name: i2c.c
 *
 * Description: Header file for the I2C AVR driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#include"i2c.h"
#include"common_macros.h"
#include<avr/io.h>

/*
 * Description: Intialize I2C driver
 */
void TWI_init( const I2C_ConfigType *ptr )
{
	/* Configure I2C BitRate Register TWBR according to chosen SCL frequency */
	TWBR = ( ( F_CPU/ 2*( ptr->sclFreq) ) - 8 );

	/* Set TWPS01 & TWPS00 in TWSR Register */
	TWSR = 0x00;

	/* Configure address in I2C Address Register TWAR */
	TWAR = ptr->slave_address;

	/* Enable I2C Module */
	TWCR  = (1<<TWEN);
}

/*
 * Description: To send the start bit
 */
void TWI_start( void )
{
	/*
	 * Clear TWINT flag before sending start bit
	 * Set start bit TWSTA = 1
	 * Enable TWI Module TWEN = 1
	 */
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	/* Wait untill TWINT flag is set ( start bit is sent successfully) */
	while( BIT_IS_CLEAR(TWCR,TWINT) );
}

/*
 * Description: To send stop bit
 */
void TWI_stop( void )
{
	/*
		 * Clear TWINT flag before sending stop bit
		 * Set stop bit TWSTO = 1
		 * Enable TWI Module TWEN = 1
		 */
		TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

/*
 * Description: To write a data in a slave
 */
void TWI_writeByte( uint8 data )
{
	/*Put data in TWI data register */
	TWDR =data;
	/*
	 * Clear TWINT flag before sending stop bit
	 * Enable TWI Module TWEN = 1
	 */
	TWCR = (1<<TWINT) | (1<<TWEN);

	/* Wait untill TWINT flag is set ( data is sent successfully) */
	while( BIT_IS_CLEAR(TWCR,TWINT) );
}

/*
 * Description: To recieve a byte from slave with ACK
 */
uint8 TWI_readByteWithACK( void )
{
	/*
	 * Clear TWINT flag before recieving data
	 * Set Enable Acknowledgment TWEA = 1
     * Enable TWI Module TWEN = 1
	 */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);

	/* Wait untill TWINT flag is set ( data is read successfully) */
	while( BIT_IS_CLEAR(TWCR,TWINT) );

	return TWDR;
}


/*
 * Description: To recieve a byte from slave with NACK
 */
uint8 TWI_readByteWitNACK( void )
{
	/*
	* Clear TWINT flag before recieving data
    * Enable TWI Module TWEN = 1
    */
	TWCR = (1<<TWINT) | (1<<TWEN);

    /* Wait untill TWINT flag is set ( data is read successfully) */
	while( BIT_IS_CLEAR(TWCR,TWINT) );

	return TWDR;
}


/*
 * Description: To get status of I2C logic
 */
uint8 TWI_getStatus( void )
{
	uint8 status;
	/* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	status = TWSR & 0xF8;
	return status;
}


