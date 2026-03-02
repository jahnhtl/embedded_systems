#include <Arduino.h>
#include <avr/interrupt.h>

// INT0 = PD2 (Arduino Pin 2) -> Taster (mit Pull-up, low-aktiv)
// LED  = PB5 (Arduino Pin 13, onboard LED)

volatile uint8_t taster_flag = 0;

ISR(INT0_vect)
{
  taster_flag = 1;
}

int main(void)
{
  // PB5 als Ausgang (LED)
  DDRB |= (1 << PB5);

  // PD2 als Eingang mit Pull-up (Taster)
  DDRD  &= ~(1 << PD2);
  PORTD |=  (1 << PD2);

  // INT0: fallende Flanke (Taster gedrueckt = HIGH -> LOW)
  // EICRA: ISC01=1, ISC00=0
  EICRA |= (1 << ISC01);
  EICRA &= ~(1 << ISC00);

  // INT0 aktivieren
  EIMSK |= (1 << INT0);

  // Globale Interrupts aktivieren
  sei();

  while(true)
  {
    if (taster_flag)
    {
      taster_flag = 0;
      PORTB ^= (1 << PB5);  // LED toggeln
    }
  }
}
