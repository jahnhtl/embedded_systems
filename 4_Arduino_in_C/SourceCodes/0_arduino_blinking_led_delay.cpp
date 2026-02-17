/*
 * File:     blinking_led_delay.c
 * Created:  25.09.2017
 * Author:   Philipp Jahn
 *
 * Blinking LED with Delay
 * Problem with delay -> processor is always busy with waiting!!
 */ 

// needed for delay functionality (dependent on clock frequency)
#define F_CPU 16000000UL    // UL indicates that this constant
                            // is an unsinged long variable

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  DDRB |= (1 << PB5);       // set PB5 to output
  PORTB |= (1 << PB5);      // LED on

  while (1)
  {
    PORTB ^= (1 << PB5);    // toggle LED on/off
    _delay_ms(500);         // CPU blocked for 500ms doing nothing
  }

  return 0;                 //no real need for this line, we will 
                            //never reach this line of code, 
                            //artefact from standard C programming
}
