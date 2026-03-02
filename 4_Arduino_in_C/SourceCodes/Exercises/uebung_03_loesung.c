/*
 * Übung 3 – Externe Interrupts mit INT0
 * Thema: Hardware-Interrupt, ISR, Flankensteuerung
 *
 * Hardware:
 *   PD2 – Taster (INT0, Pull-up, gegen GND)
 *   PD4 – LED (toggelt bei Tasterdruck via ISR)
 *
 * Beobachtung: Ohne Entprellung kann ein Tasterdruck mehrere ISR-Aufrufe
 * auslösen → LED schaltet scheinbar zufällig. (Lösung in Übung4!)
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>   // ISR(), sei(), cli()

/*
 * === INTERRUPT SERVICE ROUTINE ===
 * Wird automatisch aufgerufen wenn INT0 auslöst (fallende Flanke an PD2).
 * ISR muss kurz sein → kein delay, keine langen Berechnungen!
 */
ISR(INT0_vect) {
    PORTD ^= (1 << PD4);    // LED toggeln (XOR: 0→1, 1→0)
}

int main(void) {
    /*
     * === INITIALISIERUNG ===
     */

    // PD4 als Ausgang (LED)
    DDRD |= (1 << PD4);

    // PD2 als Eingang mit Pull-up (Taster)
    DDRD  &= ~(1 << PD2);   // Eingang
    PORTD |=  (1 << PD2);   // Pull-up aktivieren

    /*
     * INT0 auf fallende Flanke konfigurieren:
     * EICRA – External Interrupt Control Register A
     *   ISC01=1, ISC00=0 → fallende Flanke
     *
     * Tabelle:
     *   ISC01=0, ISC00=0 → LOW-Pegel
     *   ISC01=0, ISC00=1 → jede Flanke
     *   ISC01=1, ISC00=0 → fallende Flanke ← das wollen wir
     *   ISC01=1, ISC00=1 → steigende Flanke
     */
    EICRA |=  (1 << ISC01);  // Bit ISC01 auf 1
    EICRA &= ~(1 << ISC00);  // Bit ISC00 auf 0

    /*
     * INT0 freigeben:
     * EIMSK – External Interrupt Mask Register
     *   INT0-Bit auf 1 → INT0 aktiv
     */
    EIMSK |= (1 << INT0);

    /*
     * Globale Interrupts freigeben:
     * sei() setzt das I-Bit im SREG → alle freigegebenen Interrupts aktiv
     */
    sei();

    while (1) {
        // Hauptschleife leer – der Interrupt übernimmt die Arbeit
        // Mikrocontroller kann hier andere Aufgaben ausführen
    }

    return 0;
}

/*
 * === REFLEXION ===
 * 1. ISR = Interrupt Service Routine. Aufgerufen von der Hardware
 *    (Interrupt-Controller) bei erkannter Flanke, NICHT vom Programm.
 *
 * 2. Ohne sei(): I-Bit im SREG = 0 → keine Interrupts ausgeführt
 *    → Taster hat keine Wirkung.
 *
 * 3. Prellen: Mechanische Kontakte öffnen/schließen mehrfach (~1-10 ms)
 *    → viele Flanken → viele ISR-Aufrufe → LED springt.
 *
 * 4. ISR kurz halten: Während ISR sind andere Interrupts gesperrt (default).
 *    Langer Code → verzögertes Reagieren auf andere Ereignisse.
 */
