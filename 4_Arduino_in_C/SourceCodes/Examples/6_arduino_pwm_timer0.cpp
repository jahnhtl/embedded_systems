#include <Arduino.h>

int main(void)
{
  // since we are using our own main function - no need to reset the timer0 registers before

  TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); // Fast PWM mode
  TCCR0B |= (1 << CS00);

  DDRD |= (1 << PD6); // Digital Pin 6 -> OC0A = PWM Output

  OCR0A = 127;

  while(true)
  {
    // PWM runs complete in hardware
  }
}
