#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// Pulsierende LED (Breathing Effect) via Timer0 Fast PWM
// OC0B = PD5 (Arduino Pin 5)
//
// OCR0B wird schrittweise von 0 -> 255 -> 0 veraendert.
// Schrittweite und Delay bestimmen die Pulsationsgeschwindigkeit:
//   5 ms x 255 Schritte = ~1,3 s pro Richtung -> ~0,4 Hz Pulsrate

#define SCHRITT_DELAY_MS  5   // Pause zwischen zwei Helligkeitsschritten

int main(void)
{
  // Timer0 Fast PWM auf OC0B (PD5)
  // WGM01=1, WGM00=1  -> Fast PWM (TOP = 0xFF)
  // COM0B1=1          -> Non-inverting: OC0B bei Compare-Match loeschen
  // CS00=1            -> Prescaler 1 (f_PWM = 16 MHz / (1 * 256) = 62,5 kHz)
  TCCR0A |= (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
  TCCR0B |= (1 << CS00);

  DDRD |= (1 << PD5);   // PD5 als Ausgang (OC0B)

  uint8_t helligkeit = 0;
  int8_t richtung = 1;   // +1: aufhellen, -1: abdunkeln

  while(true)
  {
    OCR0B = helligkeit;

    _delay_ms(SCHRITT_DELAY_MS);

    helligkeit = helligkeit + richtung;

    if (helligkeit >= 255)
    {
      helligkeit = 255;
      richtung = -1;
    }
    if (helligkeit <= 0)
    {
      helligkeit = 0;
      richtung = 1;
    }
  }
}
