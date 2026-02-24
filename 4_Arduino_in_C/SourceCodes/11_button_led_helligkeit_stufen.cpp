#include <Arduino.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Taster = PD2 (Arduino Pin 2, INT0, Pull-up, low-aktiv)
// LED    = PD6 (Arduino Pin 6, OC0A, PWM via Timer0)
//
// Jeder Tastendruck erhoht die Helligkeit um 25%.
// Nach 100% springt die LED zurueck auf 0%.
//   Stufe 0:   0% -> OCR0A =   0
//   Stufe 1:  25% -> OCR0A =  64
//   Stufe 2:  50% -> OCR0A = 128
//   Stufe 3:  75% -> OCR0A = 191
//   Stufe 4: 100% -> OCR0A = 255

#define ENTPRELL_MS  20
#define ANZAHL_STUFEN 5

const uint8_t helligkeitswerte[ANZAHL_STUFEN] = {0, 64, 128, 191, 255};

volatile uint8_t taster_flag = 0;

ISR(INT0_vect)
{
  taster_flag = 1;
}

int main(void)
{
  // Timer0 Fast PWM auf OC0A (PD6)
  TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
  TCCR0B |= (1 << CS00);
  DDRD   |= (1 << PD6);
  OCR0A   = 0;   // Startzustand: 0%

  // PD2 als Eingang mit Pull-up (Taster)
  DDRD  &= ~(1 << PD2);
  PORTD |=  (1 << PD2);

  // INT0: fallende Flanke
  EICRA |=  (1 << ISC01);
  EICRA &= ~(1 << ISC00);
  EIMSK |=  (1 << INT0);

  sei();

  uint8_t stufe = 0;

  while(true)
  {
    if (taster_flag)
    {
      taster_flag = 0;

      _delay_ms(ENTPRELL_MS);

      if (!(PIND & (1 << PD2)))         // Taster noch gedrueckt?
      {
        stufe = (stufe + 1) % ANZAHL_STUFEN;   // 0->1->2->3->4->0->...
        OCR0A = helligkeitswerte[stufe];

        while (!(PIND & (1 << PD2)));   // warten bis losgelassen
        _delay_ms(ENTPRELL_MS);
      }
    }
  }
}
