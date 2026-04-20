#include <Arduino.h>

void setup()
{
  // reset timer 1 
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  
  // Fast PWM mode 14: WGM13:0 = 1110 (TOP = ICR1)
  // WGM11=1, WGM10=0 in TCCR1A
  // WGM13=1, WGM12=1 in TCCR1B
  // COM1A1=1 for non-inverting PWM on OC1A
  TCCR1A |= (1 << COM1A1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10); // Prescaler 1024

  DDRB |= (1 << PB1); // Digital Pin 9 -> OC1A = PWM Output

  // 16MHz / 1024 / 1Hz = 15625 -> 1 second period
  ICR1 = 15625;   // TOP value for 1 second period
  OCR1A = 7812;   // 50% duty cycle
}

void loop()
{
  // PWM runs complete in hardware
}
