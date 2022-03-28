 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the ATmega16 LCD driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include"std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define LCD_PORT_ID   	    PORTC_ID
#define LCD_RW_PORT_ID      PORTD_ID
#define LCD_ENABLE_PORT_ID  PORTD_ID
#define LCD_RS_PORT_ID      PORTD_ID

#define LCD_RW_PIN_ID       PIN4_ID
#define LCD_ENABLE_PIN_ID   PIN5_ID
#define LCD_RS_PIN_ID       PIN3_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the LCD driver.
 */
void LCD_init( void );

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand( uint8 command );

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_displayCharacter( uint8 value );

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(  const uint8 *str );

/*
 * Description :
 *Clear LCD screen
 */
void LCD_clearScreen( void );

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_integerToString( int data );

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor( uint8 row, uint8 col );

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn( uint8 row, uint8 col, const uint8 *str );



#endif /* LCD_H_ */
