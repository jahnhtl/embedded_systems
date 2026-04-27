#include <Arduino.h>
#include <avr/interrupt.h>

// INT0 = PD2 (Arduino Pin 2) -> Taster (mit Pull-up, low-aktiv)
// LED  = PB5 (Arduino Pin 13, onboard LED)
//
// Saubere Entprellung per Timer1 (nicht-blockierend):
//   Zustandsautomat mit 4 Zustaenden:
//     IDLE             -> INT0 (fallende Flanke) wartet auf Tastendruck
//     DEBOUNCE_PRESS   -> INT0 deaktiviert, Timer1 laeuft 20 ms
//     PRESSED          -> Tastendruck bestaetigt; INT0 (steigende Flanke)
//                         wartet auf Loslassen
//     DEBOUNCE_RELEASE -> INT0 deaktiviert, Timer1 laeuft 20 ms
//
// Zeitberechnung (20 ms Entprellzeit):
//   f_CPU = 16 MHz, Prescaler 1024 -> f_Timer = 15.625 kHz
//   Ticks = 15625 * 0.020 = 312  ->  OCR1A = 312 - 1 = 311
//   Tatsaechliche Zeit: 312 / 15625 = 19,97 ms (Fehler < 0,2 %)

#define DEBOUNCE_OCR  311    // OCR1A fuer ~20 ms (Prescaler 1024, 16 MHz)

typedef enum { IDLE, DEBOUNCE_PRESS, PRESSED, DEBOUNCE_RELEASE } State;
static volatile State state = IDLE;

// ---- Hilfsfunktionen -------------------------------------------------------

static void timer1_start(void)
{
  TCNT1  = 0;
  OCR1A  = DEBOUNCE_OCR;
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC, Prescaler 1024
  TIFR1  |= (1 << OCF1A);                             // pending Flag loeschen
  TIMSK1 |= (1 << OCIE1A);                            // Compare-Match freigeben
}

static void timer1_stop(void)
{
  TCCR1B  = 0;
  TIMSK1 &= ~(1 << OCIE1A);
}

static void int0_enable_falling(void)
{
  EICRA |=  (1 << ISC01);
  EICRA &= ~(1 << ISC00);  // fallende Flanke
  EIFR  |=  (1 << INTF0);  // pending Flag loeschen
  EIMSK |=  (1 << INT0);
}

static void int0_enable_rising(void)
{
  EICRA |= (1 << ISC01) | (1 << ISC00);  // steigende Flanke
  EIFR  |= (1 << INTF0);
  EIMSK |= (1 << INT0);
}

// ---- ISRs ------------------------------------------------------------------

// INT0: Flanke erkannt -> INT0 sperren, Entprell-Timer starten
ISR(INT0_vect)
{
  EIMSK &= ~(1 << INT0);  // INT0 deaktivieren bis Entprellung abgeschlossen

  if (state == IDLE)
    state = DEBOUNCE_PRESS;
  else if (state == PRESSED)
    state = DEBOUNCE_RELEASE;

  timer1_start();
}

// Timer1 Compare-Match: 20 ms abgelaufen -> Pin-Zustand pruefen
ISR(TIMER1_COMPA_vect)
{
  timer1_stop();

  if (state == DEBOUNCE_PRESS)
  {
    if (!(PIND & (1 << PD2)))    // Taster noch gedrueckt (LOW)?
    {
      PORTB ^= (1 << PB5);       // LED toggeln
      state = PRESSED;
      int0_enable_rising();      // jetzt auf Loslassen warten
    }
    else                         // Fehlausloesung (Preller, kein echter Druck)
    {
      state = IDLE;
      int0_enable_falling();
    }
  }
  else if (state == DEBOUNCE_RELEASE)
  {
    if (PIND & (1 << PD2))       // Taster stabil losgelassen (HIGH)?
    {
      state = IDLE;
      int0_enable_falling();     // bereit fuer naechsten Tastendruck
    }
    else                         // noch gedrueckt: weiter auf Loslassen warten
    {
      state = PRESSED;
      int0_enable_rising();
    }
  }
}

// ---- main ------------------------------------------------------------------

int main(void)
{
  DDRB  |= (1 << PB5);    // PB5 als Ausgang (LED)
  DDRD  &= ~(1 << PD2);   // PD2 als Eingang
  PORTD |=  (1 << PD2);   // interner Pull-up

  int0_enable_falling();
  sei();

  while (true)
  {
    // Hauptschleife vollstaendig frei - gesamte Logik in ISRs
  }
}
