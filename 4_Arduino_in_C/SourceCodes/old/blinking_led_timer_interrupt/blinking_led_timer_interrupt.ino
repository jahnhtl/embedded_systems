/*
 * File:     blinking_led_timer_interrupt.c
 * Created:  02.10.2017
 * Author:   Philipp Jahn
 *
 * Blinking LED with INTERRUPT of Timer Overflow, utilizing the processor
 * more properly
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>  // includes the interrupt functions/macros
#include "bit_macros.h"

#define LED_DDR		DDRB	// define data direction register of LED
#define LED_PORT	PORTB	// define port register of LED
#define LED_PIN		PB5		// define pin of LED

// Interrupt Service Routine for Timer0 Overflow Interrupt
ISR(TIMER0_OVF_vect)
{
  static uint8_t counter;   // local static variable couting ISR calls

  counter++;

  if (counter >= 30)        // counter == 30 -> about 500ms
  {
    TOGGLE_BIT(LED_PORT,LED_PIN);  // LED on/off
    counter = 0;
  }

  return;
}

int main(void)
{
  SET_BIT(LED_DDR,LED_PIN); // set PB5 to output

  SET_BIT(TCCR0B,CS00);     // set prescaler to 1024
  SET_BIT(TCCR0B,CS02);

  SET_BIT(TIMSK0,TOIE0);    // activate Overflow Interrupt of Timer0

  sei();                    // activate global interrupts

  while (1)
  {
    // processor only toggles LED after interrupt occurence
    // in the ISR (interrupt service routine)
  }

  return 0;
}
