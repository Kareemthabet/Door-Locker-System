 /******************************************************************************
 *
 * Module: MOTOR
 *
 * File Name: motor.c
 *
 * Description: Source file for the ATmega16 MOTOR driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#include"gpio.h"
#include"motor.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void DcMotor_init( void )
{
	GPIO_setupPinDirection( MOTOR_PORT_ID,MOTOR_PIN_ID_1,PIN_OUTPUT); /* Configure Motor pins */
	GPIO_setupPinDirection( MOTOR_PORT_ID,MOTOR_PIN_ID_2,PIN_OUTPUT); /* Configure Motor pins */
	GPIO_setupPinDirection( MOTOR_PORT_ID,MOTOR_PIN_ENABLE_ID,PIN_OUTPUT); /* Configure Motor pins */
	GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ENABLE_ID,LOGIC_LOW); /* Stop the motor at the begining */
	GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ID_1,LOGIC_LOW); /* Stop the motor at the begining */
	GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ID_2,LOGIC_LOW); /* Stop the motor at the begining */
}

void DcMotor_Rotate( DcMotor_State state)
{
	/* Check motor status */
	if( state == STOP)      /* Stop the motor */
	{
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ENABLE_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ID_1,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ID_2,LOGIC_LOW);
	}
	else if( state == CW ) /* Rotate the motor clockwise direction */
	{
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ENABLE_ID,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ID_1,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ID_2,LOGIC_HIGH);
	}
	else if( state == ACW )  /* Rotate the motor anticlockwise direction */
	{
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ENABLE_ID,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ID_1,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN_ID_2,LOGIC_LOW);
	}

}


