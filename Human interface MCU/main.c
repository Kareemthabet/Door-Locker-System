 /******************************************************************************
 *
 * File Name: MC1
 *
 * Descriptions: Source file for Locker Door System Application
 *
 * Created on: Nov 4, 2021
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#include"uart.h"
#include"lcd.h"
#include"timer0.h"
#include"keypad.h"
#include<util/delay.h>
#include<avr/io.h>
#include<avr/interrupt.h>


#define MC1_READY        0x01
#define MC2_READY        0x10
#define OPEN_DOOR_       0x02
#define CHANGE_PASS_     0x03
#define CORRECT          0x05
#define INCORRECT        0x06
#define ERROR_PASS       0x88
#define CHANGE_PASSWORD  0x66

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
uint8 g_pass1[5] = {0};
uint8 g_pass2[5] = {0};
uint8 g_choice = 0;
uint16 g_ticks = 0;
uint8 g_error = 0;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description: To create a new password and send it to the other MC
 */
void CREATE_PASSWORD( void );

/*
 * Description: To Create a new password and save the password inside an array
 */
void ENTER_PASSWORD( void );

/*
 * Description: send password to the other MC
 */
void SEND_TO_MC2 ( uint8 *ptr );

/*
 * Description: Send the password to the other MC using UART
 */
void SEND_PASS( uint8 *ptr );

/*
 * Description: Recieve from the other MC if the 2 password are the same or not
 */
void CHECK( void );

/*
 * Description: Function to display the Application choices open door or change password
 */
void MAIN_MENU( void );

/*
 * Description: Function to send to the other MC to open the door (rotate the motor)
 */
void OPEN_DOOR ( void );

/*
 * Description: Function to send to the other MC to open the door (rotate the motor)
 */
void CHANGE_PASS( void );

/*
 * Description: Function to display door information on the LCD
 */
void DISPLAY_DOOR_INFO ( void );

/*
 * Description: Function to display ERROR message if the password entered 3 times wrong
 */
void DISPLAY_ERROR_MESSAGE ( void );

/*
 * Description: Function to increment global variable g_ticks after every execution of ISR
 */
void INCREMENT_TICKS( void );

/*
 * Description: Function to receive an error message form other MC if password entered 3 times wrong
 *
 */
void WRONG_PASS(void );


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void WRONG_PASS( void )
{
	/* Intialize Timer driver through structure configuration */
	TIMER0_configType config_1 ={normal, NORMAL_MODE,F_CPU_CLK_1024, NORMAL_ENABLE, 0 ,0};
	TIMER0_init( &config_1 );

	/* Wait untill recieve ERROR message from MC2 */
	while(UART_recieveByte() != ERROR_PASS );
	LCD_clearScreen();
	g_ticks = 0;

	/* Display ERROR message inside ISR OF timer 0 for 1 minute */
	while(g_ticks >= 0 && g_ticks <= 1831 )
	{
		TIMER0_setCallBack(DISPLAY_ERROR_MESSAGE);
	}
	g_ticks = 0;
	LCD_clearScreen();

	/* Deintialize timer driver */
	TIMER0_DeInit();
}


void DISPLAY_ERROR_MESSAGE ( void )
{
	LCD_moveCursor(0,0);
	g_ticks++;

	/* Display ERROR message through LCD */
	LCD_displayString("ERROR");
}
void INCREMENT_TICKS( void )
{
	/* Increment g_ticks */
	g_ticks++;
}

void CREATE_PASSWORD( void )
{
	/* Call ENTERP PASSWORD Function to create 2 passwords */
	ENTER_PASSWORD();

	/* Send two passwords to MC2 */
	SEND_TO_MC2 (g_pass1 );
	SEND_TO_MC2 ( g_pass2 );
}

void ENTER_PASSWORD( void )
{
	uint8 num = 0;
	LCD_displayString("Enter Password");
	LCD_moveCursor(1,0);

	/* Enter first password and save it inside an array */
	for(uint8 i =0; i < 5; i++)
		{
			num = KEYPAD_getPressedKey();
			_delay_ms(200);
			g_pass1[i]=num;
			LCD_displayCharacter('*');
			_delay_ms(200);
		}
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("Renter Password");
	LCD_moveCursor(1,0);

	/* Enter second password and save it inside an array */
	for(uint8 i =0; i < 5; i++)
		{
			num = KEYPAD_getPressedKey();
			_delay_ms(200);
			g_pass2[i]=num;
			LCD_displayCharacter('*');
			_delay_ms(200);
		}
	LCD_clearScreen();

}

void SEND_TO_MC2 ( uint8 *ptr )
{
	/* Send ready message to MC2 */
	UART_sendByte(MC1_READY);
	/* Wait untill recieve ready message from MC2 */
	while(UART_recieveByte() != MC2_READY);
	/* Send password to MC2 */
	SEND_PASS(ptr);
	/* Wait untill recieve ready message from MC2 */
	while(UART_recieveByte() != MC2_READY);
}
void SEND_PASS( uint8 *ptr)
{
	/* Send password to MC2 by sending element element inside the array */
	 for( uint8 i = 0; i < 5; i++)
		  {
			  UART_sendByte(ptr[i]);
		  }
}

void CHECK( void )
{
	/* Send ready message to MC2 */
	UART_sendByte(MC1_READY);
	/* Wait untill recieve correct message from MC2 */
	if( UART_recieveByte() == CORRECT )
	{
		/* if recieve correct message display on LCD correct pass */
		LCD_displayString("CORRECT PASS");

	}
	else
	{
		/* if recieve incorrect message:
		 * 1.display on LCD incorrect pass
		 * 2.tell the user to enter 2 new passwords
		 */
		LCD_displayString("INCORRECT PASS");
		_delay_ms(500);
		LCD_clearScreen();
		CREATE_PASSWORD();
		CHECK();
	}
}

void DISPLAY_DOOR_INFO( void )
{
	/* Initialize Timer driver through structure configuration */
	TIMER0_configType config_1 ={normal, NORMAL_MODE,F_CPU_CLK_1024, NORMAL_ENABLE, 0 ,0};
	TIMER0_init( &config_1 );
	LCD_clearScreen();
	/* Display open door message inside ISR OF timer 0 for 15 seconds */
	while(g_ticks >= 0 && g_ticks <= 459 )
	{
		TIMER0_setCallBack(INCREMENT_TICKS);
		LCD_moveCursor(0,0);
		LCD_displayString("Door open");
	}
	g_ticks = 0;
	LCD_clearScreen();
	/* Display door opened message inside ISR OF timer 0 for 3 seconds */
	while(g_ticks >= 0 && g_ticks <= 92 )
	{
		TIMER0_setCallBack(INCREMENT_TICKS);
		LCD_moveCursor(0,0);
		LCD_displayString("Door opened");
	}
	g_ticks = 0;
	LCD_clearScreen();
	/* Display close door message inside ISR OF timer 0 for 15 seconds */
	while(g_ticks >= 0 && g_ticks <= 459 )
	{
		TIMER0_setCallBack(INCREMENT_TICKS);
		LCD_moveCursor(0,0);
		LCD_displayString("Door close");
	}
	LCD_clearScreen();
	/* Deintialize timer0 driver */
	TIMER0_DeInit();
	g_ticks = 0;

}
void MAIN_MENU()
{
	LCD_clearScreen();
	LCD_displayString("+:OPEN DOOR");
	LCD_moveCursor(1,0);
	LCD_displayString("-:CHANGE PASS");
	do
	 {
		g_choice = KEYPAD_getPressedKey();
		_delay_ms(200);
	 }
	/* If user press '+', the user will go to open door function
	 * If user press '-', the user will go to change password function
	 */
	while( g_choice != '+' && g_choice != '-' );
	if( g_choice == '+')
	{
		_delay_ms(200);
		OPEN_DOOR();
	}
	else
	{
		_delay_ms(200);
		CHANGE_PASS();
	}
}

void OPEN_DOOR()
{
	LCD_clearScreen();
	uint8 num = 0;
	LCD_displayString("Enter Password");
	LCD_moveCursor(1,0);
	_delay_ms(200);
	/* Enter password and save it inside an array */
	for(uint8 i =0; i < 5; i++)
		{
			num = KEYPAD_getPressedKey();
			_delay_ms(200);
			g_pass1[i]=num;
			LCD_displayCharacter('*');
			_delay_ms(200);
		}


	/* Send open message to MC2 */
	UART_sendByte(OPEN_DOOR_);

	/* Wait untill recieve ready message from MC2 */
	while(UART_recieveByte() != MC2_READY);
	LCD_clearScreen();

	/* Send password to MC2 */
	SEND_TO_MC2(g_pass1);

	/* Send ready message to MC2 */
	UART_sendByte(MC1_READY);

	/* Wait untill recieve a message from MC2 */
	if(UART_recieveByte() == CORRECT)
	{
		/* GO to Display DOOR INFO function which display door situation */
        DISPLAY_DOOR_INFO();
	}
	else
	{
		/* display Wrong pass on LCD */
		LCD_displayString("WRONG PASS");
		g_error++;

		/* If user enter the password 3 times wrong it display wrong pass on LCD and then return to main menu function */
		if( g_error == 3)
		{
			UART_sendByte(MC1_READY);
			WRONG_PASS();
			MAIN_MENU();
		}
		_delay_ms(500);
		LCD_clearScreen();
		OPEN_DOOR();
	}
}

void CHANGE_PASS()
{
	LCD_clearScreen();
	uint8 num = 0;
	LCD_displayString("Enter Password");
	LCD_moveCursor(1,0);
	_delay_ms(200);

	/* Enter password and save it inside an array */
	for(uint8 i =0; i < 5; i++)
		{
			num = KEYPAD_getPressedKey();
			_delay_ms(200);
			g_pass1[i]=num;
			LCD_displayCharacter('*');
			_delay_ms(200);
		}

	/* Send change password message to MC2 */
	UART_sendByte(CHANGE_PASSWORD);

	/* Wait untill recieve ready message from MC2 */
	while(UART_recieveByte() != MC2_READY);
	LCD_clearScreen();

	/* Send password to MC2 */
	SEND_TO_MC2(g_pass1);

	/* Send ready message to MC2 */
	UART_sendByte(MC1_READY);

	/* Wait untill recieve a message from MC2 */
	if(UART_recieveByte() == CORRECT)
		{
		/* If the MC1 recieved a correct message:
		 * 1. Display Correct on the LCD
		 * 2. Tell the user to enter a new password
		 */
		   LCD_clearScreen();
	       LCD_displayString("CORRECT");
	       _delay_ms(500);
	       LCD_clearScreen();
	       LCD_displayString("ENTER NEW PASS");
	        _delay_ms(500);
	        LCD_clearScreen();
	        CREATE_PASSWORD();
	        CHECK();

		}
		else
		{
			/* If the MC1 recieved a incorrect message:
			 * 1. Display wrong pass on the LCD
			 * 2. Tell the user to reenter password
			 */
			LCD_displayString("WRONG PASS");
			g_error++;
			/* If the user enter the password 3 times wrong:
			 * 1. display wrong pass on the LCD for 1 min
			 * 2. return to main menu
			 */
			if( g_error == 3)
			{
				UART_sendByte(MC1_READY);
				WRONG_PASS();
				MAIN_MENU();
			}
			_delay_ms(500);
			LCD_clearScreen();
			CHANGE_PASS();
		}
	}

int main()
{
	SREG |=(1<<7); /* Enable GLOBAL INTERRUPT Enable */
	LCD_init();    /* Intialize LCD driver */

	/* Intialize the UART drive through configuration structure */

	UART_configType config={Disabled,N_BITS_8,ONE,9600};
	UART_init(&config);

	/* Create password Function */
	CREATE_PASSWORD();

	/* Check if two password are the same or not */
	CHECK();
	while(1)
	{
		MAIN_MENU();
	}
}


