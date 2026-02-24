#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Taster 1 = PD2 (Arduino Pin 2, INT0, Pull-up, low-aktiv) -> heller
// Taster 2 = PD3 (Arduino Pin 3, INT1, Pull-up, low-aktiv) -> dunkler
// LED       = PD6 (Arduino Pin 6, OC0A, PWM via Timer0)
//
//   Stufe 0:   0% -> OCR0A =   0
//   Stufe 1:  25% -> OCR0A =  64
//   Stufe 2:  50% -> OCR0A = 128
//   Stufe 3:  75% -> OCR0A = 191
//   Stufe 4: 100% -> OCR0A = 255

#define ENTPRELL_MS   20
#define ANZAHL_STUFEN  5

const uint8_t helligkeitswerte[ANZAHL_STUFEN] = {0, 64, 128, 191, 255};

volatile uint8_t taster1_flag = 0;   // heller
volatile uint8_t taster2_flag = 0;   // dunkler

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
  // Timer0 Fast PWM auf OC0A (PD6)
  TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
  TCCR0B |= (1 << CS00);
  DDRD   |= (1 << PD6);
  OCR0A   = 0;   // Startzustand: 0%

  // PD2, PD3 als Eingaenge mit Pull-up
  DDRD  &= ~((1 << PD2) | (1 << PD3));
  PORTD |=   (1 << PD2) | (1 << PD3);

  // INT0 und INT1: fallende Flanke
  EICRA |=  (1 << ISC01) | (1 << ISC11);
  EICRA &= ~((1 << ISC00) | (1 << ISC10));
  EIMSK |=  (1 << INT0) | (1 << INT1);

  sei();

  uint8_t stufe = 0;

  while(true)
  {
    if (taster1_flag)                        // heller
    {
      taster1_flag = 0;

      _delay_ms(ENTPRELL_MS);
      if (!(PIND & (1 << PD2)))
      {
        if (stufe < ANZAHL_STUFEN - 1)
          stufe++;
        OCR0A = helligkeitswerte[stufe];

        while (!(PIND & (1 << PD2)));
        _delay_ms(ENTPRELL_MS);
      }
    }

    if (taster2_flag)                        // dunkler
    {
      taster2_flag = 0;

      _delay_ms(ENTPRELL_MS);
      if (!(PIND & (1 << PD3)))
      {
        if (stufe > 0)
          stufe--;
        OCR0A = helligkeitswerte[stufe];

        while (!(PIND & (1 << PD3)));
        _delay_ms(ENTPRELL_MS);
      }
    }
  }
}
