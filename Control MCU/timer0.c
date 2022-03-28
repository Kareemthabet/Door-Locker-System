/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Header file for the TIMER0 AVR driver
 *
 * Author: Kareem Ahmed Thabet
 *
 *******************************************************************************/
#include"timer0.h"
#include<avr/io.h>
#include<avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
/* Interrupt Service Routine for Compare Mode */
ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/* Interrupt Service Routine for Normal Mode */
ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}

/*******************************************************************************
 *                       Functions Deifinitions                                *
 *******************************************************************************/
/*
 * Description : Function to initialize the Timer0 driver
 * 	1. Set the required Mode.
 * 	2. Set the required Output Mode Generation for CTC Mode.
 * 	3. Set the required Clock.
 * 	3. Enable the 	Normal or Compare interrupt.
 * 	4. Initialize Timer0 Registers with the required value.
 */
void TIMER0_init( const TIMER0_configType *ptr)
{
	/* FOC0 = 1 NON-PWM Mode */
	TCCR0 |=(1<<FOC0);

	/*
	 * Choose type of timer through bits WGM00 & WGM01
	 * WGM00 & WGM01 = 0 Normal Mode
	 * WGM00 & WGM01 = 1 CTC Mode
	 */
	TCCR0 = ( TCCR0 & 0xBF ) | ( (ptr->timer_type) << 6);
	TCCR0 = ( TCCR0 & 0xF7 ) | ( ( (ptr->timer_type) >> 1) <<3 );

	/*
	 * Choose Waveform Generation Mode through bits COM01 & COM00
	 */
	TCCR0 = ( TCCR0 & 0xCF ) | ( ( ptr->output_mode ) << 4 );


	/*
	 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
	 * of TCCR1B Register
	 */
	TCCR0 = ( TCCR0 & 0xF8 ) | ( ptr->clock_select );

	/*
     * insert the required  value in the timer value registers
	 */
	TCNT0 = ptr->tcnt0_value;
	OCR0 = ptr->ocr0_value;

	/* Configure which interrupt will be enabled */
	TIMSK = ( TIMSK & 0xFC) | ( ptr->interrupt_enable );
}

/*
 * Description: Function to set the Call Back function address.
 */
void TIMER0_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}

/*
 * Description: Function to disable the Timer0 driver
 */
void TIMER0_DeInit(void)
{
	TCCR0 = 0;
	TCNT0 = 0;
	OCR0 = 0;
	TIMSK = 0;
}

