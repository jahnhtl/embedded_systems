#include <Arduino.h>

void setup();
void loop();

int main()
{
  setup();
  
  while (true)
  {
    loop();
  }
}

void setup()
{
  // Pin PB5 als Ausgang konfigurieren
  DDRB = DDRB | (1 << PB5);

  // Timer0 konfigurieren mit Prescaler 1024
  TCCR0B = (1 << CS02) | (1 << CS00);

}

void loop()
{
  static uint8_t overflows_anzahl = 0; // use local static instead of global variable!

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
