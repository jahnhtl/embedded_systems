/*
 * Übung 9 – Inaktivitäts-Timeout nach 60 Sekunden
 * Thema: Overflow-Zähler, Timeout-Logik, bedingte LED-Steuerung
 *
 * Hardware:
 *   PD2 – Taster 1 (INT0, Pull-up) → Helligkeit rauf
 *   PD3 – Taster 2 (INT1, Pull-up) → Helligkeit runter
 *   PD4 – LED_STATUS (aktiver Betrieb)
 *   PD6 – LED_PWM (OC0A, PWM)
 *   PD7 – LED_ROT (blinkt 1 Hz NUR während Timeout)
 *
 * Kombiniert alle Konzepte: GPIO, Interrupts, Entprellung, PWM und Timer1-Timeout
 *
 * === TIMER1 BERECHNUNG ===
 * Prescaler 256: f_Timer = 16.000.000 / 256 = 62.500 Hz
 * Preload = 65.536 - 62.500 = 3.036 → 1-Sekunden-Überlauf
 * Timeout = 60 Überläufe × 1 s = 60 Sekunden
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* === GLOBALE VARIABLEN === */
volatile uint8_t taster1_flag  = 0;
volatile uint8_t taster2_flag  = 0;
volatile uint8_t overflow_zaehler = 0;
volatile uint8_t timeout_aktiv = 0;

uint8_t stufen[5] = {0, 64, 128, 191, 255};
uint8_t stufe     = 0;

/* === ISRs === */
ISR(INT0_vect) { taster1_flag = 1; }
ISR(INT1_vect) { taster2_flag = 1; }

/* =========================================================
 * INITIALISIERUNGSFUNKTIONEN
 * ========================================================= */

void gpio_init(void) {
    DDRD &= ~((1 << PD2) | (1 << PD3));    // PD2, PD3: Eingang
    PORTD |=  (1 << PD2) | (1 << PD3);     // Pull-up
    DDRD |= (1 << PD4) | (1 << PD6) | (1 << PD7); // Ausgänge
}

void timer0_pwm_init(void) {
    /* Fast-PWM, Non-inverting OC0A, Prescaler 64
     * f_PWM = 16.000.000 / (64 × 256) = 976,6 Hz */
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01) | (1 << CS00);
    OCR0A  = stufen[stufe];
}

void timer1_init(void) {
    /* Normal-Mode, Prescaler 256
     * Preload = 65.536 - 62.500 = 3.036 → 1 s Überlauf */
    TCCR1B = (1 << CS12);
    TCNT1  = 3036;
}

void interrupt_init(void) {
    EICRA |=  (1 << ISC01);
    EICRA &= ~(1 << ISC00);   // INT0: fallende Flanke
    EICRA |=  (1 << ISC11);
    EICRA &= ~(1 << ISC10);   // INT1: fallende Flanke
    EIMSK |= (1 << INT0) | (1 << INT1);
}

/* =========================================================
 * HILFSFUNKTIONEN
 * ========================================================= */

void update_status_led(void) {
    if (stufe > 0 && !timeout_aktiv) {
        PORTD |=  (1 << PD4);
    } else {
        PORTD &= ~(1 << PD4);
    }
}

uint8_t debounce_pind(uint8_t pin) {
    _delay_ms(20);
    if (!(PIND & (1 << pin))) {
        while (!(PIND & (1 << pin)));
        return 1;
    }
    return 0;
}

// Timeout zurücksetzen (nach Tastendruck aufrufen)
void reset_timeout(void) {
    overflow_zaehler = 0;
    TCNT1 = 3036;            // Timer neu starten
    timeout_aktiv = 0;
    PORTD &= ~(1 << PD7);   // LED_ROT ausschalten
}

/* =========================================================
 * HAUPTPROGRAMM
 * ========================================================= */

int main(void) {
    gpio_init();
    timer0_pwm_init();
    timer1_init();
    interrupt_init();
    sei();

    while (1) {
        /* === TASTER 1: Helligkeit rauf === */
        if (taster1_flag) {
            taster1_flag = 0;
            if (debounce_pind(PD2)) {
                if (stufe < 4) {
                    stufe++;
                    OCR0A = stufen[stufe];
                }
                reset_timeout();     // Timer zurücksetzen
                update_status_led();
            }
        }

        /* === TASTER 2: Helligkeit runter === */
        if (taster2_flag) {
            taster2_flag = 0;
            if (debounce_pind(PD3)) {
                if (stufe > 0) {
                    stufe--;
                    OCR0A = stufen[stufe];
                }
                reset_timeout();     // Timer zurücksetzen
                update_status_led();
            }
        }

        /* === TIMER1 OVERFLOW POLLING ===
         *
         * Jeder Überlauf = 1 Sekunde
         * Nach 60 Überläufen → Timeout aktivieren
         * Bei Timeout: LED_ROT toggeln (1 Hz)
         */
        if (TIFR1 & (1 << TOV1)) {
            TIFR1 |= (1 << TOV1);   // Flag löschen
            TCNT1 = 3036;            // Preload neu laden

            if (timeout_aktiv) {
                /* Timeout aktiv: rote LED toggeln (1 Hz) */
                PORTD ^= (1 << PD7);
            } else {
                overflow_zaehler++;
                if (overflow_zaehler >= 60) {
                    /* Timeout auslösen */
                    timeout_aktiv = 1;
                    stufe = 0;
                    OCR0A = stufen[stufe];   // PWM-LED auf 0%
                    update_status_led();
                }
            }
        }
    }

    return 0;
}

/*
 * === REFLEXION ===
 * 1. TCNT1 in reset_timeout(): Ohne Neuladen könnte die aktuelle Zählperiode
 *    schon fast abgelaufen sein → ersten Zähler nach Reset feuert fast sofort.
 *    Mit Neuladen: volle 1 Sekunde für die erste Periode nach Reset.
 *
 * 2. Ohne timeout_aktiv = 0 beim Tastendruck: timeout_aktiv bleibt 1,
 *    LED blinkt weiter, obwohl Taste gedrückt wurde.
 *
 * 3. Timeout auf 30 s: overflow_zaehler >= 30 (1 Änderung!). Preload bleibt.
 *
 * 4. Rote LED toggelt bei jedem Überlauf = 1 Hz. Eine Periode = 2 Überläufe.
 *    Für 2 Hz: zweiten Zähler einführen und nur jeden 2. Überlauf toggeln.
 */
