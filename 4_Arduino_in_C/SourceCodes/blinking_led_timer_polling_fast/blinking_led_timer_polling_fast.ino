/*
 * File:     blinking_led_timer_polling_fast.c
 * Created:  02.10.2017
 * Author:   Philipp Jahn
 *
 * Blinking LED with POLLING of Timer Overflow, utilizing the processor
 * more properly - but blinking is too fast for our eyes
 */ 

#include <avr/io.h>
#include "bit_macros.h"     // include our bit macros

#define LED_DDR		DDRB	// define data direction register of LED
#define LED_PORT	PORTB	// define port register of LED
#define LED_PIN		PB5		// define pin of LED

int main(void)
{
  SET_BIT(LED_DDR,LED_PIN); // set LED pin to output
	
  SET_BIT(TCCR0B,CS00);     // set prescaler to 1024
  SET_BIT(TCCR0B,CS02);

  TCNT0 = 0;                // not necessary (with reset set to 0 anyway)

  while (1)
  {
    if (GET_BIT(TIFR0,TOV0) == 1)   // check if timer overflow happened
    {
      TOGGLE_BIT(LED_PORT,LED_PIN); // LED on/off
      SET_BIT(TIFR0,TOV0);          // reset overflow flag (set bit!!!)
    }

    // processor is still able to do more operations while LED blinking
  }

  return 0;
}
