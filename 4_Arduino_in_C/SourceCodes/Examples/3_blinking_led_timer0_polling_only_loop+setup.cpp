#include <Arduino.h>

// behaves strange due to Timer0 is used by millis() and activated interrupts
// can be solved by reseting TCCR0A, TCCR0B, TIMSK0 completely to 0 at the beginning.

void setup()
{
  // Pin PB5 als Ausgang konfigurieren
  DDRB = DDRB | (1 << PB5);

  // Timer0 konfigurieren mit Prescaler 1024
  TCCR0B = (1 << CS02) | (1 << CS00);

}

void loop()
{
  static uint8_t overflows_anzahl = 0;

  // Check ob Overflow von Timer0 aufgetreten ist
  if(TIFR0 & (1 << TOV0))
  {
    overflows_anzahl++;
    TIFR0 = TIFR0 | (1 << TOV0);
  }

  if(overflows_anzahl >= 31)
  {
    PORTB = PORTB ^ (1 << PB5);
    overflows_anzahl = 0;
  }
}
