/*
 * Übung 8 – Timer1: Sekundentakt per Overflow-Polling
 * Thema: Timer1 Normal-Mode, TCNT1-Preload, TOV1-Flag
 *
 * Hardware:
 *   PD7 – LED_ROT (toggelt jede Sekunde)
 *
 * === PFLICHTBERECHNUNG (im Code dokumentieren!) ===
 *
 * Timer-Taktfrequenz nach Prescaler 256:
 *   f_Timer = F_CPU / Prescaler = 16.000.000 / 256 = 62.500 Hz
 *
 * Ticks pro Sekunde = 62.500
 *
 * Timer1 ist 16-Bit → zählt von Preload bis 65.535, dann Überlauf (TOV1)
 *   Maximale Zählschritte = 65.536 (0 bis 65535 + Überlauf bei 0)
 *
 * Für genau 1 Sekunde = 62.500 Ticks:
 *   Preload = 65.536 - 62.500 = 3.036
 *
 * → Jedes Mal nach Überlauf: TCNT1 = 3036 neu laden!
 */

#define F_CPU 16000000UL
#include <avr/io.h>

int main(void) {
    /* === GPIO === */
    DDRD |= (1 << PD7);   // PD7 als Ausgang (LED_ROT)

    /* === TIMER1 KONFIGURIEREN ===
     *
     * Normal-Mode: kein WGM-Bit gesetzt (Standard nach Reset)
     * Timer zählt von TCNT1 bis 65535, dann Überlauf → TCNT1 = 0, TOV1 = 1
     *
     * TCCR1B – Prescaler 256:
     *   CS12=1, CS11=0, CS10=0
     *
     * Prescaler-Tabelle Timer1:
     *   CS12=0, CS11=0, CS10=1 → Prescaler 1
     *   CS12=0, CS11=1, CS10=0 → Prescaler 8
     *   CS12=0, CS11=1, CS10=1 → Prescaler 64
     *   CS12=1, CS11=0, CS10=0 → Prescaler 256  ← wir wollen das
     *   CS12=1, CS11=0, CS10=1 → Prescaler 1024
     */
    TCCR1B = (1 << CS12);

    /* Preload setzen: TCNT1 ist 16-Bit → direkte Zuweisung */
    TCNT1 = 3036;

    while (1) {
        /* === TOV1-FLAG POLLEN ===
         *
         * TIFR1: Timer/Counter 1 Interrupt Flag Register
         * TOV1-Bit: wird 1 beim Überlauf von TCNT1
         *
         * Prüfen: TIFR1 & (1 << TOV1) → ≠ 0 wenn Überlauf aufgetreten
         */
        if (TIFR1 & (1 << TOV1)) {

            /* === TOV1 LÖSCHEN ===
             * WICHTIG: TOV1 wird durch Schreiben einer 1 gelöscht!
             * (Nicht durch Schreiben einer 0 – das ist ein häufiger Fehler!)
             */
            TIFR1 |= (1 << TOV1);   // Flag löschen

            /* === TCNT1 NEU LADEN ===
             * Ohne Neuladen: Nächster Überlauf nach 65.536 Ticks ≈ 1,049 s
             * Mit Neuladen 3036: Überlauf nach 62.500 Ticks = exakt 1 s
             */
            TCNT1 = 3036;

            /* === LED TOGGELN === */
            PORTD ^= (1 << PD7);   // XOR: 0→1, 1→0
        }
    }

    return 0;
}

/*
 * === REFLEXION ===
 * 1. Polling vs. Interrupt: Polling prüft das Flag aktiv in der Hauptschleife.
 *    Interrupt-basiert: TOV1 löst ISR aus (TIMSK1 |= (1 << TOIE1)).
 *    Polling ist einfacher, belegt aber CPU-Zeit.
 *
 * 2. Timer1 (16-Bit) statt Timer0 (8-Bit): Timer0 läuft von 0-255 (62.500/256 ≈ 244 Hz).
 *    Für 1-Sekunden-Takt zu klein (max ~16 ms bei Prescaler 1024).
 *    Timer1 (16-Bit): bis zu ~4 Sekunden ohne Preload möglich.
 *
 * 3. Preload für 0,5 Sekunden (Prescaler 256):
 *    f_Timer = 62.500 Hz → Ticks für 0,5 s = 31.250
 *    Preload = 65.536 - 31.250 = 34.286
 *
 * 4. Ohne TCNT1 Neuladen: erste Periode = (65536-3036)/62500 = 1,0 s (zufällig ok).
 *    Zweite Periode = 65536/62500 = 1,049 s → Timer driftet!
 *    Mit Neuladen: jede Periode exakt 1,0 s.
 */
