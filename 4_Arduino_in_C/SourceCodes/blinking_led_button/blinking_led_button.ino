/*
 * File:     blinking_led_button.c
 * Created:  02.01.2019
 * Author:   Philipp Jahn
 */
/* Blinking LED dependent on a button. If the button is pressed the LED
 * shall blink, otherwise the LED shall be off. In this example the bouncing
 * of the button does not need to be covered.
 */ 

#ifndef F_CPU				// avoid re-definition
#define F_CPU 16000000UL
#endif				

#include <avr/io.h>
#include <util/delay.h>
#include "bit_macros.h"     // bit macros moved to external header file
                            // " " -> indicates to search for the included
                            // file in same folder as the current file
							// (instead of < > where first the libraries are
							// checked mentioned in the CLASS_PATH variable
							
// use #defines to make changes of bit mapping much easier
#define LED_DDR		DDRB	// define data direction register of LED
#define LED_PORT	PORTB	// define port register of LED
#define LED_PIN		PB5		// define pin of LED, pin 13 on arduino

#define BUTTON_DDR  DDRB	// define data direction register of Button
#define BUTTON_PORT PORTB   // define port register of Button
#define BUTTON_PINREG PINB  // define pin register of Button
#define BUTTON_PIN  PB0     // define pin of Button, pin 8 on arduino

#define BLINK_PERIOD  200	// change of blinking frequency

int main(void)
{
	SET_BIT(LED_DDR, LED_PIN);			// set LED pin to output
	CLEAR_BIT(BUTTON_DDR, BUTTON_PIN);	// set Button pin to input (not really needed since default value is 0)
	SET_BIT(BUTTON_PORT, BUTTON_PIN);	// activate internal pull-up resistor for Button pin to avoid a undefined state 

	while (1)
	{
		// the LED shall blink as long as the button is pressed = 0
		while(GET_BIT(BUTTON_PINREG, BUTTON_PIN) == 0)	// while(bit_is_clear(BUTTON_PINREG, BUTTON_PIN) would do the same
		{
			TOGGLE_BIT(LED_PORT, LED_PIN);	// toggle LED on/off
			_delay_ms(BLINK_PERIOD/2);		// use defined period devided by 2
		}
		
		CLEAR_BIT(LED_PORT, LED_PIN);		// LED off
	}

	return 0;
}
