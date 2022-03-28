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
#ifndef MOTOR_H_
#define MOTOR_H_

#include"std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define MOTOR_PORT_ID       PORTB_ID
#define MOTOR_PIN_ENABLE_ID PIN2_ID
#define MOTOR_PIN_ID_1      PIN0_ID
#define MOTOR_PIN_ID_2      PIN1_ID

/*******************************************************************************
 *                                 Enums                                  *
 *******************************************************************************/
typedef enum{
	STOP,CW,ACW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the Motor driver.
 */
void DcMotor_init( void );

/*
 * Description :
 * Function responsible for determine the motor speed and direction.
 */
void DcMotor_Rotate( DcMotor_State state);

#endif /* MOTOR_H_ */
