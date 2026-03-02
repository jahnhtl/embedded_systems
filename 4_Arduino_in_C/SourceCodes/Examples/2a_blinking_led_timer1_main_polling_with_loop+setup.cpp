#include <Arduino.h>

void setup();
void loop();
void init_timer1_polling(void);

int main()
{
  setup();
  
  while (true)
  {
    loop();
  }
}

void setup()
{
  // Pin PB5 als Ausgang konfigurieren
  DDRB = DDRB | (1 << PB5);

  // Timer1 konfigurieren -> Overflow alle 1 Sekunden
  init_timer1_polling(void)

}

void init_timer1_polling(void) {
  /*
   * Timer1 im Normal-Mode – Overflow-Flag wird per Polling ausgewertet
   *
   * BERECHNUNG – Vorladewert für exakt 1 Overflow pro Sekunde:
   * ----------------------------------------------------------
   * Timer1 zählt von TCNT1 bis 65535, dann Overflow (TOV1-Flag).
   * Benötigte Ticks für 1 s = f_CPU / Prescaler = 16.000.000 / 256 = 62.500
   * Vorladewert TCNT1 = 65536 - 62500 = 3036
   *
   * Probe: 62.500 Ticks × 256 / 16.000.000 Hz = 1,000 s  ✓
   *
   * Für 20 Sekunden Timeout: 20 Overflows zählen
   * Für 0,5-Hz-Blinken (1 s ein / 1 s aus): LED bei jedem Overflow toggeln
   *
   * REGISTER-KONFIGURATION:
   * -----------------------
   * TCCR1A = 0    → Normal-Mode, keine OC-Pins
   * TCCR1B:
   *   CS12=1      → Prescaler 256
   * TIMSK1 = 0    → kein Interrupt! (Polling via TIFR1)
   */
  TCCR1A = 0;
  TCCR1B = (1 << CS12);   // Normal-Mode, Prescaler 256

  TCNT1  = 3036; // Vorladewert für 1-Sekunden-Overflow
  // TIMSK1 bleibt 0 → kein Interrupt, TOV1 wird per Polling geprüft
}

void loop()
{
  static bool blink_zustand = false;

  // TOV1-Flag prüfen (gesetzt wenn Timer1 übergelaufen)
  if (TIFR1 & (1 << TOV1)) {
    TIFR1  = (1 << TOV1);       // Flag löschen (durch Schreiben einer 1)
    TCNT1  = 3036;              // Timer nachladen für nächste Sekunde

    // LED im 1-Sekunden-Takt toggeln (nur solange Timeout aktiv)
    blink_zustand = !blink_zustand;
    if (blink_zustand) {
      PORTB |=  (1 << PB5);
    } else {
      PORTB &= ~(1 << PB5);
    }
}