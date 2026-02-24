#include <Arduino.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// INT0 = PD2 (Arduino Pin 2) -> Taster 1: LED einschalten
// INT1 = PD3 (Arduino Pin 3) -> Taster 2: LED ausschalten
// LED  = PB5 (Arduino Pin 13, onboard LED)

#define ENTPRELL_MS 20

volatile uint8_t taster1_flag = 0;   // INT0: LED ein
volatile uint8_t taster2_flag = 0;   // INT1: LED aus

ISR(INT0_vect)
{
  taster1_flag = 1;
}

ISR(INT1_vect)
{
  taster2_flag = 1;
}

int main(void)
{
  // PB5 als Ausgang (LED)
  DDRB |= (1 << PB5);

  // PD2, PD3 als Eingaenge mit Pull-up (Taster)
  DDRD  &= ~((1 << PD2) | (1 << PD3));
  PORTD |=   (1 << PD2) | (1 << PD3);

  // INT0 und INT1: fallende Flanke
  // ISC01=1, ISC00=0 -> INT0 fallende Flanke
  // ISC11=1, ISC10=0 -> INT1 fallende Flanke
  EICRA |=  (1 << ISC01) | (1 << ISC11);
  EICRA &= ~((1 << ISC00) | (1 << ISC10));
  EIMSK |=  (1 << INT0) | (1 << INT1);

  sei();

  while(true)
  {
    if (taster1_flag)
    {
      taster1_flag = 0;

      _delay_ms(ENTPRELL_MS);
      if (!(PIND & (1 << PD2)))        // Taster 1 noch gedrueckt?
      {
        PORTB |= (1 << PB5);           // LED einschalten

        while (!(PIND & (1 << PD2)));  // warten bis losgelassen
        _delay_ms(ENTPRELL_MS);
      }
    }

    if (taster2_flag)
    {
      taster2_flag = 0;

      _delay_ms(ENTPRELL_MS);
      if (!(PIND & (1 << PD3)))        // Taster 2 noch gedrueckt?
      {
        PORTB &= ~(1 << PB5);          // LED ausschalten

        while (!(PIND & (1 << PD3)));  // warten bis losgelassen
        _delay_ms(ENTPRELL_MS);
      }
    }
  }
}
