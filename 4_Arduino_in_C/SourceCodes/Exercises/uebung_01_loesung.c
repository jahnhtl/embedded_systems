/*
 * Übung 1 – LED blinken mit DDR/PORT-Registern
 * Thema: Grundlegende GPIO-Konfiguration ohne Arduino-Bibliothek
 *
 * Hardware:
 *   PD4 – LED (mit 220 Ω Vorwiderstand gegen GND)
 *
 * Pinbelegung ATmega328P Port D:
 *   Bit 4 = PD4 (Arduino: Digital Pin 4)
 */

#define F_CPU 16000000UL   // Taktfrequenz: 16 MHz (muss vor util/delay.h stehen!)
#include <avr/io.h>        // Registerdefinitionen (DDRD, PORTD, ...)
#include <util/delay.h>    // _delay_ms()

int main(void) {
    /*
     * === INITIALISIERUNG ===
     * DDRD: Data Direction Register Port D
     *   Bit auf 1 → Ausgang
     *   Bit auf 0 → Eingang (Reset-Zustand)
     *
     * PD4 als Ausgang: Bit 4 in DDRD setzen
     *   DDRD |= (1 << PD4)
     *        = DDRD |= 0b00010000
     */
    DDRD |= (1 << PD4);   // PD4 → Ausgang

    while (1) {
        /*
         * === HAUPTSCHLEIFE: LED blinken ===
         *
         * PORTD: Ausgabe-Register für Port D
         *   Bit auf 1 → Pin HIGH (5V) → LED leuchtet
         *   Bit auf 0 → Pin LOW  (0V) → LED aus
         */

        // LED einschalten: PD4 auf HIGH setzen
        PORTD |= (1 << PD4);
        _delay_ms(500);            // 500 ms warten

        // LED ausschalten: PD4 auf LOW setzen
        PORTD &= ~(1 << PD4);
        _delay_ms(500);            // 500 ms warten

        // → Frequenz: 1 Hz (1 Periode = 1000 ms)
    }

    return 0;
}

/*
 * === REFLEXION ===
 * 1. Ohne F_CPU-Definition: _delay_ms() berechnet falsche Wartezeiten
 *    (oder der Compiler gibt eine Warnung aus).
 *
 * 2. Nach Reset: DDRD = 0x00 → alle Pins als Eingang konfiguriert.
 *
 * 3. Zwei LEDs gleichzeitig einschalten:
 *    PORTD |= (1 << PD4) | (1 << PD5);   ← ein Schreibvorgang!
 *    Effizienter als zwei separate Befehle.
 */
