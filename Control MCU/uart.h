 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#ifndef UART_H_
#define UART_H_
/*******************************************************************************
 *                         Includes                                            *
 *******************************************************************************/
#include"std_types.h"


/*******************************************************************************
 *                         Enums and Structures                                *
 *******************************************************************************/
typedef enum{
	Disabled,EVEN_PARITY = 2, ODD_PARITY = 3
}UART_parityMode;

typedef enum{
	N_BITS_5, N_BITS_6, N_BITS_7, N_BITS_8, N_BITS_9 = 7
}UART_bitSize;

typedef enum{
	ONE, TWO
}UART_stopBits;

typedef struct{
	UART_parityMode parityMode;
	UART_bitSize    bitSize;
	UART_stopBits   stopBits;
	uint32 baud_rate;

}UART_configType;

/*******************************************************************************
 *                          Functions prototypes                               *
 *******************************************************************************/

/*
 * Description: Function to intialize the UART driver
 */
void UART_init( const UART_configType *ptr);

/*
 * Description: Function to send a byte to another UART
 */
void UART_sendByte( uint8 data);

/*
 * Description: Function to recieve a byte from another UART
 */

uint8 UART_recieveByte( void );

/*
 * Description: Function to send a string to another UART
 */
void UART_sendString( const uint8 *str );

/*
 * Description: Function to recieve a string from another UART
 */

void UART_recieveString( uint8 *str );

#endif /* UART_H_ */
