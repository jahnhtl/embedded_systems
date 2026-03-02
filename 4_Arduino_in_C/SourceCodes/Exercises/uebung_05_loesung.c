/*
 * Übung 5 – Timer0 Fast-PWM konfigurieren
 * Thema: PWM-Signal auf OC0A (PD6), Frequenzberechnung
 *
 * Hardware:
 *   PD6 – LED_PWM (OC0A – direkt mit Timer0 Komparator verbunden!)
 *
 * === PFLICHTBERECHNUNG (im Code dokumentieren!) ===
 *
 * Fast-PWM Frequenz:
 *   f_PWM = F_CPU / (Prescaler × 256)
 *   f_PWM = 16.000.000 / (64 × 256)
 *   f_PWM = 16.000.000 / 16.384
 *   f_PWM = 976,6 Hz
 *
 * → Kein Flimmern sichtbar (> 50 Hz Richtwert für LEDs)
 *
 * Duty Cycle für verschiedene OCR0A-Werte:
 *   OCR0A =   0 →   0% (LED aus)
 *   OCR0A =  64 →  25%
 *   OCR0A = 128 →  50%
 *   OCR0A = 191 →  75%
 *   OCR0A = 255 → 100% (LED voll an)
 */

#define F_CPU 16000000UL
#include <avr/io.h>

int main(void) {
    /*
     * === PD6 als Ausgang konfigurieren ===
     * OC0A ist intern mit PD6 verbunden, aber DDRD muss trotzdem
     * manuell auf Ausgang gesetzt werden!
     */
    DDRD |= (1 << PD6);

    /*
     * === Timer0 Fast-PWM konfigurieren ===
     *
     * TCCR0A – Timer/Counter Control Register 0 A:
     *
     *   COM0A1=1, COM0A0=0 → Non-inverting Mode auf OC0A:
     *     - OC0A wird gelöscht wenn TCNT0 == OCR0A (Duty-Cycle-Ende)
     *     - OC0A wird gesetzt am TOP (= 255) → steigende Flanke am Anfang
     *
     *   WGM01=1, WGM00=1 → Fast-PWM Mode (WGM02:0 = 0b011)
     *     - Zählt von 0 bis 255 (TOP = 0xFF), dann Reset → sehr hohe Frequenz
     */
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);

    /*
     * TCCR0B – Timer/Counter Control Register 0 B:
     *
     *   CS01=1, CS00=1 → Prescaler 64
     *   (CS02=0, CS01=1, CS00=1 aus Prescaler-Tabelle)
     */
    TCCR0B = (1 << CS01) | (1 << CS00);

    /*
     * OCR0A – Output Compare Register 0 A:
     * Bestimmt den Duty Cycle.
     * 128 / 255 ≈ 50% → LED bei halber Helligkeit
     */
    OCR0A = 128;

    /*
     * Ab hier läuft der Timer automatisch im Hintergrund.
     * Die Hardware erzeugt das PWM-Signal ohne CPU-Beteiligung!
     */
    while (1) {
        // Hauptschleife leer – PWM läuft hardwareseitig
    }

    return 0;
}

/*
 * === ERWEITERUNGSAUFGABE – OCR0A Werte und Helligkeit ===
 *   OCR0A =   0 →   0% → LED aus
 *   OCR0A =  64 →  25% → LED dunkel
 *   OCR0A = 128 →  50% → LED mittel
 *   OCR0A = 191 →  75% → LED hell
 *   OCR0A = 255 → 100% → LED maximal
 *
 * === REFLEXION ===
 * 1. Prescaler 1024: f_PWM = 16MHz / (1024 × 256) = 61 Hz → noch knapp ok,
 *    aber schon nahe am sichtbaren Flimmer-Bereich.
 *
 * 2. Kein Flimmern ab ca. 50-100 Hz (individuelle Wahrnehmungsgrenze).
 *    Prescaler 64 → 976 Hz → sehr sicher.
 *
 * 3. analogWrite() verwendet intern dieselben Timer, aber:
 *    - Ändert ggf. Timer-Einstellungen unerwartet
 *    - Nicht kompatibel mit paralleler Timer-Nutzung (z.B. Timer1 für Timeout)
 *    - In reinen C-Projekten: Arduino-Bibliothek vermeiden → direkte Registerzugriffe!
 */
