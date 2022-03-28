 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: LCD.c
 *
 * Description: Source file for the ATmega16 LCD driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#include"LCD.h"
#include"gpio.h"
#include"common_macros.h"
#include<util/delay.h>

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD:
 * Setup the LCD pins directions by use the GPIO driver.
 */
void LCD_init( void )
{
	/* Configure the direction data port as output port */
	GPIO_setupPortDirection(LCD_PORT_ID,PORT_OUTPUT);
	/* Configure the direction for RS, RW and E pins as output pins */
	GPIO_setupPinDirection(LCD_RW_PORT_ID,LCD_RW_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_RS_PORT_ID,LCD_RS_PIN_ID,PIN_OUTPUT);

	LCD_sendCommand(0x38); /* use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(0x0c); /* cursor off */
	LCD_sendCommand(0x01); /* clear LCD at the beginning */
}

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand( uint8 command )
{
		GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN_ID,LOGIC_LOW); /* Instruction Mode RS=0 */
		GPIO_writePin(LCD_RW_PORT_ID,LCD_RW_PIN_ID,LOGIC_LOW); /* write data to LCD so RW=0 */
		_delay_ms(1); /* delay for processing Tas = 50ns */
		GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_HIGH);/* Enable LCD E=1 */
		_delay_ms(1);/* delay for processing Tpw - Tdws = 190ns */
		GPIO_writePort(LCD_PORT_ID,command); /* out the required command to the data bus D0 --> D7 */
		_delay_ms(1); /* delay for processing Tdsw = 100ns */
		GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_LOW);/* Disable LCD E=0 */
		_delay_ms(1);/* delay for processing Th = 13ns */
}

/*
 * Description :
 * Send the required data to the screen
 */
void LCD_displayCharacter( uint8 data)
{
	 	    GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN_ID,LOGIC_HIGH); /* Instruction Mode RS=1 */
	 	 	GPIO_writePin(LCD_RW_PORT_ID,LCD_RW_PIN_ID,LOGIC_LOW);  /* write data to LCD so RW=0 */
			_delay_ms(1); /* delay for processing Tas = 50ns */
			GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_HIGH);/* Enable LCD E=1 */
			_delay_ms(1);/* delay for processing Tpw - Tdws = 190ns */
			GPIO_writePort(LCD_PORT_ID,data);/* out the required data to the data bus D0 --> D7 */
			_delay_ms(1);/* delay for processing Tdsw = 100ns */
			GPIO_writePin(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,LOGIC_LOW);/* Disable LCD E=0 */
			_delay_ms(1);/* delay for processing Th = 13ns */
}

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString( const uint8 *str )
{
	uint8 count;
	for(count = 0; str[count]!='\0'; count++)
	{
		LCD_displayCharacter( str[count] );
	}
}

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row,uint8 col)
{
	uint8 lcd_memory_address;
	switch(row)
	{
		case 0:
			lcd_memory_address=col;
				break;
		case 1:
			lcd_memory_address=col+0x40;
				break;
		case 2:
			lcd_memory_address=col+0x10;
				break;
		case 3:
			lcd_memory_address=col+0x50;
				break;
	}
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(lcd_memory_address | 0x80 );
}

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row,uint8 col, const uint8 *str)
{
	LCD_moveCursor(row,col);
	LCD_displayString(str);
}

/*
 * Description :
 * Clear LCD screen
 */
void LCD_clearScreen( void )
{
	LCD_sendCommand(0x01);
}

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data)
{
   uint8 buff[16]; /* String to hold the ascii result */
   itoa(data,buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
   LCD_displayString(buff); /* Display the string */
}



