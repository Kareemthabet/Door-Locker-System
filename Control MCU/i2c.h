/******************************************************************************
 *
 * Module: I2C
 *
 * File Name: i2c.h
 *
 * Description: Header file for the I2C AVR driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#ifndef I2C_H_
#define I2C_H_
/*******************************************************************************
 *                         Includes                                            *
 *******************************************************************************/
#include"std_types.h"

/*******************************************************************************
 *                         Definitions                                         *
 *******************************************************************************/
/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                         Enums and Structures                                *
 *******************************************************************************/
typedef enum{
	NORMAL = 100000,FAST_MODE = 400000, FAST_MODE_PLUS = 1000000, HIGH_SPEED_MODE = 3400000
}I2C_sclFreq;
typedef struct{
	 I2C_sclFreq  sclFreq ;
	 uint8 slave_address;
}I2C_ConfigType;

/*******************************************************************************
 *                          Functions prototypes                               *
 *******************************************************************************/

/*
 * Description: Intialize I2C driver
 */
void TWI_init(  );

/*
 * Description: To send the start bit
 */
void TWI_start( void );

/*
 * Description: To send stop bit
 */
void TWI_stop( void );

/*
 * Description: To write a data in a slave
 */
void TWI_writeByte( uint8 data );

/*
 * Description: To recieve a byte from slave with ACK
 */
uint8 TWI_readByteWithACK( void );


/*
 * Description: To recieve a byte from slave with NACK
 */
uint8 TWI_readByteWitNACK( void );


/*
 * Description: To get status of I2C logic
 */
uint8 TWI_getStatus( void );

#endif /* I2C_H_ */
