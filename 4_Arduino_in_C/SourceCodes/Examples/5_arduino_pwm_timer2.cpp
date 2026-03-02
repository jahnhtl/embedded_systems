#include <Arduino.h>

void setup()
{
  // reset timer 2 since Arduino uses this one for the tone() function 
  TCCR2A = 0;
  TCCR2B = 0;
  TIMSK2 = 0;
  
  TCCR2A |= (1 << COM2A1) | (1 << WGM21) | (1 << WGM20); // Fast PWM mode
  TCCR2B |= (1 << CS20);

  DDRB |= (1 << PB3); // Digital Pin 11 -> OC2A = PWM Output

  OCR2A = 127;  
}

void loop()
{
  // PWM runs complete in hardware
}
