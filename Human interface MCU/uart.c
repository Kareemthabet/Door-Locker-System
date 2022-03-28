 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/

#include"uart.h"
#include"common_macros.h"
#include<avr/io.h>
/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

/*
 * Description: Function to intialize the UART driver
 */
void UART_init( const UART_configType *ptr)
{
	uint16 ubrr_value = 0;

	/* U2X = 1 for double transmission speed */
	UCSRA |= (1<<U2X);

	/************************** UCSRB Description **************************
		 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
		 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
		 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
		 * RXEN  = 1 Receiver Enable
		 * RXEN  = 1 Transmitter Enable
		 * UCSZ2 is configured through UART configuration structure
		 * RXB8 & TXB8 not used for 8-bit data mode
		 ***********************************************************************/
	UCSRB |= (1<<RXEN) | (1<<TXEN);
	UCSRB = (UCSRB & 0xFB ) | ( ( (ptr->bitSize) >>2 ) << 2 );

	/************************** UCSRC Description **************************
		 * URSEL   = 1 The URSEL must be one when writing the UCSRC
		 * UMSEL   = 0 Asynchronous Operation
		 * UPM1:0  is configured through UART configuration structure
		 * USBS    is configured through UART configuration structure
		 * UCSZ1:0 is configured through UART configuration structure
		 * UCPOL   = 0 Used with the Synchronous operation only
		 ***********************************************************************/
	UCSRC |= (1<<URSEL);
	UCSRC = ( UCSRC & 0xCF ) | ( ( ptr->parityMode ) << 4 );
	UCSRC = ( UCSRC & 0xF7 ) | ( ( ptr->stopBits ) << 3 );
	UCSRC = ( UCSRC & 0xF9 ) | ( ( ptr->bitSize ) << 1 );

	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / ((ptr->baud_rate) * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}

/*
 * Description: Function to send a byte to another UART
 */
void UART_sendByte( uint8 data)
{
	/*
		 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
		 * transmitting a new byte so wait until this flag is set to one
		 */
	while( BIT_IS_CLEAR(UCSRA,UDRE) );

	/*
		 * Put the required data in the UDR register and it also clear the UDRE flag as
		 * the UDR register is not empty now
		 */
	UDR = data;
}

/*
 * Description: Function to recieve a byte from another UART
 */

uint8 UART_recieveByte( void )
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while( BIT_IS_CLEAR(UCSRA,RXC) );

		/*
		 * Read the received data from the Rx buffer (UDR)
		 * The RXC flag will be cleared after read the data
		 */
	    return UDR;
}

/*
 * Description: Function to send a string to another UART
 */
void UART_sendString( const uint8 *str )
{
	uint8 i = 0;

	/* Send the whole string */
	for( i = 0 ; str[i] != '\0'; i++)
	{
		UART_sendByte(str[i]);
	}
}

/*
 * Description: Function to recieve a string from another UART
 */

void UART_recieveString( uint8 *str )
{
	    uint8 i = 0;

		/* Receive the first byte */
		str[i] = UART_recieveByte();

		/* Receive the whole string until the '#' */
		while(str[i] != '#')
		{
			i++;
			str[i] = UART_recieveByte();
		}

		/* After receiving the whole string plus the '#', replace the '#' with '\0' */
		str[i] = '\0';
}

