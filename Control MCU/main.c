 /******************************************************************************
 *
 * File Name: MC2
 *
 * Descriptions: Source file for Locker Door System Application
 *
 * Created on: Nov 4, 2021
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#include"uart.h"
#include"external_eeprom.h"
#include"motor.h"
#include"timer0.h"
#include"buzzer.h"
#include<util/delay.h>
#include<avr/interrupt.h>
#include<avr/io.h>


#define MC1_READY   0x01
#define MC2_READY   0x10
#define OPEN_DOOR_  0x02
#define CHANGE_PASS_ 0x03
#define CORRECT     0x05
#define INCORRECT   0x06
#define SUCCESS       1
#define UNSUCCESS     0
#define ADDRESS   0x111
#define ERROR_PASS     0x88
#define CHANGE_PASSWORD  0x66

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
uint8 g_pass1[5] = {0};
uint8 g_pass2[5] = {0};
volatile uint16 g_ticks = 0;
uint8 g_error = 0;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description: To check if the 2 passwords are identical or not
 */
uint8 CHECK_EQUALITY( void );

/*
 * Description: To recieve a password from MC1
 */
void RECIEVE_PASS( uint8 *ptr );
/*
 * Description: To recieve a password from MC1 through synchronized messages
 */
void SEND_RECIEVE( uint8 *ptr );

/*
 * Description: To send to MC1  if 2 passwords are identical or not
 */
void EQUALITY_RESPONSE( void );

/*
 * Description: To save password inside the eeprom
 */
void SAVE_INSIDE_EEPROM( uint8 *ptr );

/*
 * Description: To choose form 2 options to open the door or recieve a new password
 */
void MAIN_MENU( void );
/*
 * Description: To check if the password sent is equal to the password saved inside eeprom
 */
uint8 CHECK_WITH_EEPROM( uint8 *ptr );
/*
 * Description: To intialize timer0 driver
 */
void TIMER_CONFIG( void );

/*
 * Description: To Rotate MOTOR CW
 */
void MOTOR_ON_CW();

/*
 * Description: To Rotate MOTOR ACW
 */
void MOTOR_ON_ACW();

/*
 * Description: To Rotate MOTOR OFF
 */
void MOTOR_OFF();

/*
 * Description: To intialize buzzer if the password is entered wrong
 */
void BUZZER_ERROR( void );

/*
 * Description: To set the buzzer on
 */
void BUZZER_ON (void );


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void BUZZER_ON( void )
{
	/* Set buzzer on */
	BUZZER_on();
	g_ticks++;
}
void BUZZER_ERROR( void )
{
	/* Intialize Timer0 driver through structure configuration */
	TIMER0_configType config_1 ={normal, NORMAL_MODE,F_CPU_CLK_1024, NORMAL_ENABLE, 0 ,0};
	TIMER0_init( &config_1 );

	/* Send error message to MC1 */
	UART_sendByte(ERROR_PASS);

	/* Call function BUZEER on inside the ISR for 1 minute */
	while(g_ticks >= 0 && g_ticks <= 1831 )
	{
		TIMER0_setCallBack(BUZZER_ON);
	}
	/*Reset the buzzer */
	BUZZER_off();
	g_ticks = 0;

	/* Deinitialize timer0 driver */
	TIMER0_DeInit();
}

uint8 CHECK_EQUALITY( void )
{
	/* Check if the 2 password are equal */
	for(uint8 i = 0; i < 5; i++)
	{
	  if(g_pass1[i] != g_pass2[i])
	  {
		 return UNSUCCESS;
	  }
	}
    return SUCCESS;
}

void RECIEVE_PASS( uint8 *ptr )
{
	/*Recieve password from MC1 */
	  for( uint8 i = 0; i < 5; i++)
	  {
		  ptr[i] = UART_recieveByte();
	  }
}

void SEND_RECIEVE( uint8 *ptr )
{
	/* To receive a password from MC1 through synchronized messages */
	while(UART_recieveByte() != MC1_READY);
	 UART_sendByte(MC2_READY);
	 RECIEVE_PASS(ptr);
	 UART_sendByte(MC2_READY);
}

void EQUALITY_RESPONSE( void )
{
	/* If the 2 passwords are equal send to MC1 Correct message */
	 if( CHECK_EQUALITY() == SUCCESS )
		 {
			 while(UART_recieveByte() != MC1_READY);
			 UART_sendByte(CORRECT);
		 }
	 /* If the 2 passwords are unequal send to MC1 incorrect message:
	  * send to MC1 incorrect message
	  * tell the user two renter 2 new passwords
	  */
	 else if( CHECK_EQUALITY() == UNSUCCESS )
		 {
			 while(UART_recieveByte() != MC1_READY);
			 UART_sendByte(INCORRECT);
			 SEND_RECIEVE( g_pass1 );
			 SEND_RECIEVE( g_pass2 );
			 EQUALITY_RESPONSE();
		 }
}

void SAVE_INSIDE_EEPROM( uint8 *ptr)
{
	/* Save the password inside the eeprom */
	  for( uint8 i = 0; i < 5; i++)
		  {
			  EEPROM_writeByte( ADDRESS+i, ptr[i] );
		  }
}

uint8 CHECK_WITH_EEPROM(uint8 *ptr)
{
	/* check the sent password with that saved in the eeprom */
	while(UART_recieveByte() != MC1_READY);
	for( uint8 i = 0; i < 5; i++)
	{
		EEPROM_readByte( ADDRESS+i, g_pass2 );
	}
	for( uint8 i = 0; i < 5; i++)
	{
		 if( g_pass2[i] != g_pass1[i] )
		 {
			 return INCORRECT;
		 }
    }
	return CORRECT;
}

void MOTOR_ON_CW()
{
	/* Rotate MOTOR CW */

	g_ticks++;
	DcMotor_Rotate( CW );
}

void MOTOR_ON_ACW()
{
	/* Rotate MOTOR ACW */
	g_ticks++;
	DcMotor_Rotate( ACW );
}

void MOTOR_OFF()
{
	/* Reset MOTOR */
	g_ticks++;
	DcMotor_Rotate( STOP );
}

void TIMER_CONFIG()
{
	/* Initialize Timer driver through structure configuration */
	TIMER0_configType config_1 ={normal, NORMAL_MODE,F_CPU_CLK_1024, NORMAL_ENABLE, 0 ,0};
	TIMER0_init( &config_1 );
	/* Rotate the motor for 15 seconds */
	while(g_ticks >= 0 && g_ticks <= 459 )
	{
		TIMER0_setCallBack(MOTOR_ON_CW);

	}
	g_ticks = 0;
	/* Reset the motor  CW for 15 seconds */
	while(g_ticks >= 0 && g_ticks <= 92 )
	{
		TIMER0_setCallBack(MOTOR_OFF);
	}
	g_ticks = 0;
	/* Rotate the motor ACW for 15 seconds */
	while(g_ticks >= 0 && g_ticks <= 459 )
	{
		TIMER0_setCallBack(MOTOR_ON_ACW);
	}
	/* Deinitialize the timer0 driver */
	TIMER0_DeInit();
	g_ticks = 0;
	/* Reset the Motor */
	MOTOR_OFF();
}
void MAIN_MENU()
{
	/* Wait untill recieve a message from MC2 */
	if(UART_recieveByte() == OPEN_DOOR_)
	{
		/*
		 * If the recieved message is open door:
		 * 1. recieve password from MC1
		 * 2. Check it with eeprom
		 * 3. if the password sent 3 times wrong , the buzzer will be activate
		 */
		UART_sendByte(MC2_READY);
		SEND_RECIEVE( g_pass1 );
		if( CHECK_WITH_EEPROM( g_pass1 ) == CORRECT )
		{
			UART_sendByte(CORRECT);
			TIMER_CONFIG();
		}
		else
		{
			g_error++;
			UART_sendByte(INCORRECT);
			if( g_error == 3)
			{
				while(UART_recieveByte() != MC1_READY);
				BUZZER_ERROR();
				MAIN_MENU();
			}
				MAIN_MENU();
		}
	}

	else
	{
		/*
	     * If the recieved message is change pass:
		 * 1. recieve password from MC1
		 * 2. Check it with eeprom
		 * 3. if the password sent 3 times wrong , the buzzer will be activate
		 * 4. if it is correct tell the MC1 to enter new password
		 */
		UART_sendByte(MC2_READY);
		SEND_RECIEVE( g_pass1 );
		if( CHECK_WITH_EEPROM( g_pass1 ) == CORRECT )
		{
			UART_sendByte(CORRECT);
			SEND_RECIEVE( g_pass1 );
			SEND_RECIEVE( g_pass2 );
			EQUALITY_RESPONSE();
			SAVE_INSIDE_EEPROM( g_pass1 );
		}
		else
		{
		g_error++;
		UART_sendByte(INCORRECT);
		if( g_error == 3)
		{
		 while(UART_recieveByte() != MC1_READY);
		 BUZZER_ERROR();
		 MAIN_MENU();
		}
		MAIN_MENU();
		}
	}

}
int main()
{
  SREG |=(1<<7);  /* Enable global interrupt enable */
  DcMotor_init();   /* initialize DC motor driver */
  BUZZER_init();  /* initialize buzzer driver */
  /* initialize UART driver through structure configuration */
  UART_configType config_2={Disabled, N_BITS_8, ONE,9600};
  UART_init( &config_2 );

  /* Recieve first password from MC1 */
  SEND_RECIEVE( g_pass1 );

  /* Recieve second password from MC1 */
  SEND_RECIEVE( g_pass2 );

  /* check if two passwords are equal */
  EQUALITY_RESPONSE();

  /* Save the password inside the eeprom */
  SAVE_INSIDE_EEPROM( g_pass1 );
  while(1)
	{
	  MAIN_MENU();
	}
}

