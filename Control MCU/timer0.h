 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the TIMER0 AVR driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#ifndef TIMER0_H_
#define TIMER0_H_
/*******************************************************************************
 *                         Includes                                            *
 *******************************************************************************/
#include"std_types.h"

/*******************************************************************************
 *                         Enums and Structures                                *
 *******************************************************************************/
typedef enum{
	normal, CTC = 3
}TIMER0_waveFormGeneration;

typedef enum{
	NORMAL_MODE, TOGGLE, SET, CLEAR
}TIMER0_compareOutputMode;

typedef enum{
	NO_CLK, F_CPU_CLK, F_CPU_CLK_8, F_CPU_CLK_64, F_CPU_CLK_256, F_CPU_CLK_1024,\
	F_CPU_CLK_EXTERNAL_FALLING, F_CPU_CLK_EXTERNAL_RISING
}TIMER0_clockSelect;

typedef enum{
		 DISABLED, NORMAL_ENABLE = 1, CTC_ENABLE = 2
}TIMER0_interruptEnable;

typedef struct{
	TIMER0_waveFormGeneration timer_type;
	TIMER0_compareOutputMode  output_mode;
	TIMER0_clockSelect        clock_select;
	TIMER0_interruptEnable    interrupt_enable;
	uint8 ocr0_value;
	uint8 tcnt0_value;
}TIMER0_configType;

/*******************************************************************************
 *                          Functions prototypes                               *
 *******************************************************************************/

/*
 * Description: Function to intialize TIMER0 driver
 */
void TIMER0_init( const TIMER0_configType *ptr);

/*
 * Description: Function to set the Call Back function address.
 */
void TIMER0_setCallBack(void(*a_ptr)(void));

/*
 * Description: Function to disable the Timer0 driver
 */
void TIMER0_DeInit(void);


#endif /* TIMER0_H_ */
