/******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the BUZZER driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#include"gpio.h"
#include"buzzer.h"

/******************************************************************************
 *                        Function Declarations                               *
 *******************************************************************************/
/*
 * Description : To intialize BUZZER pin as output pin
 */
 void BUZZER_init( void )
 {
	 GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	 GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
 }

 /*
  * Description : To write 1 to buzzer pin
  */
 void BUZZER_on( void )
 {
	 GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
 }

 /*
  * Description : To write 0 to buzzer pin
  */
 void BUZZER_off( void )
 {
	 GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
 }
