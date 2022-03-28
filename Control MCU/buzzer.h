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

#ifndef BUZZER_H_
#define BUZZER_H_
/*******************************************************************************
 *                         Definitions                                          *
 *******************************************************************************/
#define BUZZER_PORT_ID PORTD_ID
#define BUZZER_PIN_ID  PIN3_ID
/*******************************************************************************
 *                         Functions Prototypes                                 *
 *******************************************************************************/

/*
 * Description : To intialize BUZZER pin as output pin
 */
 void BUZZER_init( void );

 /*
  * Description : To write 1 to buzzer pin
  */
 void BUZZER_on( void );

 /*
  * Description : To write 0 to buzzer pin
  */
 void BUZZER_off( void );

#endif /* BUZZER_H_ */
