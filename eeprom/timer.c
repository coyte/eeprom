/*
 * timer.c
 *
 *  Created on: 13 Mar 2017
 *      Author: niels@teekens.info
 */


#include "timer.h"

int xtrTime = 0;


//init timer2
void Timer0_Init(void)
	{
		TCNT0=0x00;
	}



//start timer2
void Timer0_Start(void)
	{
		TCCR0A = (1 << WGM01); //set CTC bit
		TCCR0B = (1<<CS01) | (1<<CS00);		//prescaler 64
		OCR0A = 125;	// 1ms
		TIMSK0 |= 1<<OCIE0A; //interrupt is generated on

	}




//timer ISR check for rotarry encoder status
ISR( TIMER0_COMPA_vect )
	{
		//reading rotary and button
		RotaryCheckStatus();

		//interrupt fires every ms, add extra timer for 1 sec LED blink
		xtrTime++;
		if (xtrTime > 1000)
		{
			xtrTime = 0;
			PORTB ^= (1 << PORTB0);
//			USART_putstring("M32Rotary\n");

		}

	}

