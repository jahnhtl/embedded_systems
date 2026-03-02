/*
 * Übung 6 – 5-stufige Helligkeitssteuerung (Polling)
 * Thema: Stufen-Array, OCR0A-Werte, Status-LED
 *
 * Hardware:
 *   PD2 – Taster 1 (Pull-up) → Helligkeit rauf
 *   PD3 – Taster 2 (Pull-up) → Helligkeit runter
 *   PD4 – LED_STATUS (leuchtet wenn Helligkeit > 0)
 *   PD6 – LED_PWM (OC0A, PWM)
 *
 * Hinweis: Entprellung hier mit _delay_ms(200) — vereinfacht.
 * Robuste Entprellung (Flag-Pattern) folgt in Übung7!
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    /*
     * Helligkeitsstufen:
     * Index:  0    1    2    3    4
     * OCR0A:  0   64  128  191  255
     * Duty:   0%  25%  50%  75% 100%
     */
    uint8_t stufen[5] = {0, 64, 128, 191, 255};
    uint8_t stufe = 0;   // Startindex: 0% (LED aus)

    /* === GPIO KONFIGURIEREN === */
    // PD2, PD3: Eingang + Pull-up (Taster)
    DDRD &= ~((1 << PD2) | (1 << PD3));
    PORTD |=  (1 << PD2) | (1 << PD3);

    // PD4: Ausgang (LED_STATUS)
    DDRD |= (1 << PD4);

    // PD6: Ausgang (LED_PWM, OC0A)
    DDRD |= (1 << PD6);

    /* === TIMER0 FAST-PWM KONFIGURIEREN ===
     * f_PWM = 16.000.000 / (64 × 256) = 976,6 Hz */
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01) | (1 << CS00);   // Prescaler 64
    OCR0A  = stufen[stufe];                // Startwert: 0%

    while (1) {
        /* === TASTER 1: Helligkeit rauf === */
        if (!(PIND & (1 << PD2))) {        // Gedrückt? (active-low)
            if (stufe < 4) {
                stufe++;
                OCR0A = stufen[stufe];
            }
            _delay_ms(200);                // Einfache Entprellung (blockierend)
        }

        /* === TASTER 2: Helligkeit runter === */
        if (!(PIND & (1 << PD3))) {
            if (stufe > 0) {
                stufe--;
                OCR0A = stufen[stufe];
            }
            _delay_ms(200);
        }

        /* === LED_STATUS AKTUALISIEREN ===
         * Leuchtet wenn Helligkeit > 0 (stufe >= 1) */
        if (stufe > 0) {
            PORTD |=  (1 << PD4);   // LED_STATUS ein
        } else {
            PORTD &= ~(1 << PD4);   // LED_STATUS aus
        }
    }

    return 0;
}

/*
 * === REFLEXION ===
 * 1. _delay_ms(200): Hauptschleife blockiert 200 ms → in dieser Zeit
 *    werden keine anderen Ereignisse (Taster2, Timer, ...) verarbeitet.
 *    Bei 1 Taster noch tolerierbar, bei mehreren Ereignissen problematisch.
 *
 * 2. 33% Duty Cycle: OCR0A = 255 × 0,33 = 84 (gerundet)
 *
 * 3. OCR0A vs. TCNT0:
 *    OCR0A: Output Compare Register – Vergleichswert für Duty Cycle
 *    TCNT0: Timer Counter – aktueller Zählerwert (0-255, läuft automatisch)
 */
