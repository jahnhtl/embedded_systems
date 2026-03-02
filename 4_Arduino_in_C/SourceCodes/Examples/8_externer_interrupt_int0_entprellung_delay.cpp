#include <Arduino.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// INT0 = PD2 (Arduino Pin 2) -> Taster (mit Pull-up, low-aktiv)
// LED  = PB5 (Arduino Pin 13, onboard LED)
//
// Einfache Entprellung:
//   Nach dem Interrupt kurz warten (_delay_ms), dann Pin-Zustand bestaetigen.
//   Anschliessend warten bis Taster losgelassen wird.

#define ENTPRELL_MS 20

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

  // INT0: fallende Flanke
  EICRA |= (1 << ISC01);
  EICRA &= ~(1 << ISC00);
  EIMSK |= (1 << INT0);

  sei();

  while(true)
  {
    if (taster_flag)
    {
      taster_flag = 0;

      _delay_ms(ENTPRELL_MS);            // Prellen abklingen lassen

      if (!(PIND & (1 << PD2)))          // Taster noch gedrueckt?
      {
        PORTB ^= (1 << PB5);             // LED toggeln

        while (!(PIND & (1 << PD2)));    // warten bis losgelassen
        _delay_ms(ENTPRELL_MS);          // Prellen beim Loslassen abwarten
      }
    }
  }
}
