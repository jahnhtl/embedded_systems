#include <Arduino.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// INT0 = PD2 (Arduino Pin 2) -> Taster (mit Pull-up, low-aktiv)
// LED  = PB5 (Arduino Pin 13, onboard LED)
//
// Saubere Entprellung per Mehrfach-Abtastung:
//   Den Pin ENTPRELL_SAMPLES mal im Abstand von je 1 ms lesen.
//   Nur wenn mindestens 80% der Messungen LOW ergeben, gilt der
//   Tastendruck als bestaetigt. Gleiches Prinzip beim Loslassen.

#define ENTPRELL_SAMPLES  10   // Anzahl Abtastungen
#define ENTPRELL_DELAY_MS  1   // Pause zwischen zwei Abtastungen in ms

// Gibt 1 zurueck wenn der Taster stabil gedrueckt ist (>= 80% LOW-Pegel)
static uint8_t taster_gedrueckt(void)
{
  uint8_t low_count = 0;
  for (uint8_t i = 0; i < ENTPRELL_SAMPLES; i++)
  {
    if (!(PIND & (1 << PD2)))
      low_count++;
    _delay_ms(ENTPRELL_DELAY_MS);
  }
  return low_count >= (ENTPRELL_SAMPLES * 4 / 5);
}

// Gibt 1 zurueck wenn der Taster stabil losgelassen ist (>= 80% HIGH-Pegel)
static uint8_t taster_losgelassen(void)
{
  uint8_t high_count = 0;
  for (uint8_t i = 0; i < ENTPRELL_SAMPLES; i++)
  {
    if (PIND & (1 << PD2))
      high_count++;
    _delay_ms(ENTPRELL_DELAY_MS);
  }
  return high_count >= (ENTPRELL_SAMPLES * 4 / 5);
}

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

      if (taster_gedrueckt())            // Tastendruck bestaetigt?
      {
        PORTB ^= (1 << PB5);            // LED toggeln

        while (!taster_losgelassen());  // warten bis stabil losgelassen
      }
    }
  }
}
