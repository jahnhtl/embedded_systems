/*
 * File:     blinking_led_delay_macros_header.c
 * Created:  25.09.2017
 * Author:   Philipp Jahn
 */
/* Blinking LED with Delay and easy to read macros in header file.
 * Problem with delay -> processor is always busy with waiting!!
 */ 

#ifndef F_CPU				// this conditional directive ensures that F_CPU has not been defined somewhere else already (without it you might get a warning)
#define F_CPU 16000000UL
#endif	

#include <avr/io.h>
#include "bit_macros.h"
#include <util/delay.h>

int main(void)
{
	SET_BIT(DDRB,PB5);			// set PB5 to output

	while (1)
	{
		SET_BIT(PORTB,PB5);		// LED on
		_delay_ms(900);			// CPU blocked for 900ms doing nothing

		CLEAR_BIT(PORTB,PB5);	// LED off
		_delay_ms(100);
	}

	return 0;
}
