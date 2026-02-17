/*
 * File:     blinking_led_button_interrupt.c
 * Created:  02.01.2019
 * Author:   Philipp Jahn
 *
 * Blinking LED dependent on a button. If the button is pressed the LED
 * shall blink, otherwise the LED shall be off. In this example the bouncing
 * of the button does not need to be covered.
 */ 

#ifndef F_CPU        // this conditional directive ensures that F_CPU has not been defined somewhere else already (without it you might get a warning)
#define F_CPU 16000000UL
#endif               // end of conditional directive
        

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bit_macros.h"     // bit macros moved to external header file
                            // "" -> indicates to search for the include
                            // file in same folder as the current file

// use #defines to make changes of bit mapping much easier
#define LED_DDR   DDRB     	// define data direction register of LED
#define LED_PORT  PORTB 	// define port register of LED
#define LED_PIN   PB5   	// define pin of LED, pin 13 on arduino

#define BUTTON_DDR  DDRD  	// define data direction register of Button
#define BUTTON_PORT PORTD   // define port register of Button
#define BUTTON_PINREG PIND  // define pin register of Button
#define BUTTON_PIN  PD2     // define pin of Button, pin 2 on arduino

#define BLINK_PERIOD  200 	// change of blinking frequency

uint8_t volatile blinking = 0;  // the keyword 'volatile' avoids unwanted optimizations by the compiler

ISR(INT0_vect)
{
  blinking = blinking ^ 1;  
}


int main(void)
{
  SET_BIT(LED_DDR, LED_PIN);      // set LED pin to output
  CLEAR_BIT(BUTTON_DDR, BUTTON_PIN);  // set Button pin to input (not really needed since default value is 0)
  SET_BIT(BUTTON_PORT, BUTTON_PIN); // activate internal pull-up resistor for Button pin to avoid a undefined state 

  EICRA |= (1 << ISC01); // a falling edge triggers interrupt for INT0
  EIMSK |= (1 << INT0);  // external interrupt 0 activated

  sei();          // set global enable interrupt flag

  while (1)
  {
    while(blinking == 1)
    {
      TOGGLE_BIT(LED_PORT, LED_PIN);  // toggle LED on/off
      _delay_ms(BLINK_PERIOD/2);    // use defined period devided by 2
    }
    
    CLEAR_BIT(LED_PORT, LED_PIN);   // LED off
  }

  return 0;
}
