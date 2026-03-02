/*
 * Übung 2 – Taster mit Pull-up lesen (Polling)
 * Thema: Digitaler Eingang, interner Pull-up, active-low Logik
 *
 * Hardware:
 *   PD4 – LED (Ausgang, mit Vorwiderstand gegen GND)
 *   PD2 – Taster (Eingang, eine Seite an PD2, andere an GND)
 *
 * Active-low: Taster gedrückt → PD2 = LOW (= 0)
 *             Taster los      → PD2 = HIGH (= 1, durch Pull-up)
 */

#define F_CPU 16000000UL
#include <avr/io.h>

int main(void) {
    /*
     * === INITIALISIERUNG ===
     *
     * PD4 als Ausgang (LED):
     *   DDRD |= (1 << PD4) → Bit 4 auf 1
     */
    DDRD |= (1 << PD4);

    /*
     * PD2 als Eingang (Taster):
     *   DDRD &= ~(1 << PD2) → Bit 2 auf 0 (Eingang)
     *   Nach Reset ist DDRD = 0, also wäre dieser Schritt nicht nötig.
     *   Trotzdem explizit setzen → klarer Code, robuster bei Änderungen!
     */
    DDRD &= ~(1 << PD2);

    /*
     * Internen Pull-up aktivieren:
     *   Wenn DDRD.Bit = 0 (Eingang), dann aktiviert PORTD.Bit = 1
     *   den internen Pull-up-Widerstand (~50 kΩ gegen VCC).
     *   → PD2 liegt im Ruhezustand auf HIGH (5V)
     */
    PORTD |= (1 << PD2);

    while (1) {
        /*
         * === TASTER ABFRAGEN ===
         *
         * PIND: Pin Input Register D – liest physischen Pin-Zustand
         *   PIND & (1 << PD2):
         *     → 0 (= 0b00000000) wenn Taster gedrückt (PD2 = LOW)
         *     → ≠ 0              wenn Taster losgelassen (PD2 = HIGH)
         *
         * !(PIND & (1 << PD2)):
         *     → 1 wenn gedrückt → LED ein
         *     → 0 wenn los      → LED aus
         */
        if (!(PIND & (1 << PD2))) {
            // Taster gedrückt → LED einschalten
            PORTD |= (1 << PD4);
        } else {
            // Taster losgelassen → LED ausschalten
            PORTD &= ~(1 << PD4);
        }
    }

    return 0;
}

/*
 * === REFLEXION ===
 * 1. Ohne Pull-up: floating pin → undefinierter Zustand (zufällig 0 oder 1)
 *    → LED flackert ohne Tasterdruck → "floating pin" Problem
 *
 * 2. Active-high (Pull-down): Logik umkehren:
 *    if (PIND & (1 << PD2)) { LED ein } → Taster gedrückt = HIGH
 *
 * 3. Unterschied PORTD vs. PIND:
 *    PORTD: Schreiben → steuert Ausgang (oder Pull-up bei Eingang)
 *    PIND:  Lesen     → aktueller physischer Zustand des Pins
 */
