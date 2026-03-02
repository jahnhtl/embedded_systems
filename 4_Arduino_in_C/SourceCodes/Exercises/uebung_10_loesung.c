/*
 * Übung 10 – Lüftergeschwindigkeits-Steuerung mit Timeout
 * Thema: Vollständige Systemintegration mit anderer Pinbelegung
 *
 * Hardware (ANDERE PINS als Übung7–Übung9!):
 *   PD2 – TASTER_SCHNELLER (INT0, Pull-up)
 *   PD3 – TASTER_LANGSAMER (INT1, Pull-up)
 *   PB0 – LED_BETRIEB (grün, leuchtet wenn Lüfter läuft)
 *   PD5 – LUEFTER (OC0B, PWM) ← OC0B statt OC0A!
 *   PB1 – LED_WARNUNG (gelb, blinkt 2 Hz bei Timeout)
 *
 * === PFLICHTBERECHNUNGEN ===
 *
 * Timer0 Fast-PWM (OC0B):
 *   f_PWM = F_CPU / (Prescaler × 256) = 16.000.000 / (64 × 256) = 976,6 Hz
 *
 * Timer1 – 0,5-Sekunden-Überlauf (Prescaler 256):
 *   f_Timer = 16.000.000 / 256 = 62.500 Hz
 *   Ticks für 0,5 s = 62.500 / 2 = 31.250
 *   Preload = 65.536 - 31.250 = 34.286
 *
 * Timeout: 15 s / 0,5 s = 30 Überläufe
 * Blinken:  bei jedem Überlauf toggeln → 1 Toggle alle 0,5 s → 2 Hz
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* === GLOBALE VARIABLEN === */
volatile uint8_t taster_auf_flag = 0;
volatile uint8_t taster_ab_flag  = 0;
volatile uint8_t overflow_zaehler = 0;
volatile uint8_t timeout_aktiv   = 0;

uint8_t stufen[5] = {0, 64, 128, 191, 255};
uint8_t stufe     = 0;

/* === ISRs === */
ISR(INT0_vect) { taster_auf_flag = 1; }
ISR(INT1_vect) { taster_ab_flag  = 1; }

/* =========================================================
 * INITIALISIERUNGSFUNKTIONEN
 * ========================================================= */

void gpio_init(void) {
    /* Port D: PD2, PD3 Eingang + Pull-up; PD5 Ausgang */
    DDRD &= ~((1 << PD2) | (1 << PD3));   // Eingänge
    PORTD |=  (1 << PD2) | (1 << PD3);    // Pull-up
    DDRD |=  (1 << PD5);                   // PD5: Ausgang (OC0B)

    /* Port B: PB0 und PB1 als Ausgänge
     * WICHTIG: PB → DDRB / PORTB (nicht DDRD / PORTD!) */
    DDRB |= (1 << PB0) | (1 << PB1);      // LED_BETRIEB, LED_WARNUNG
}

void timer0_pwm_init(void) {
    /* Fast-PWM auf OC0B (PD5)
     *
     * UNTERSCHIED zu Übung7–Übung9:
     *   COM0B1=1, COM0B0=0 (statt COM0A1 / COM0A0)
     *   → Non-inverting auf OC0B
     *
     * WGM02:0 = 0b011 (Fast-PWM) → wie bisher
     * Prescaler 64: CS01=1, CS00=1 → wie bisher
     *
     * f_PWM = 16.000.000 / (64 × 256) = 976,6 Hz
     */
    TCCR0A = (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01) | (1 << CS00);   // Prescaler 64
    OCR0B  = stufen[stufe];               // WICHTIG: OCR0B statt OCR0A!
}

void timer1_init(void) {
    /* Normal-Mode, Prescaler 256
     * Preload = 65.536 - 31.250 = 34.286 → 0,5-Sekunden-Überlauf
     * (Unterschied zu Übung8/9: 34286 statt 3036!) */
    TCCR1B = (1 << CS12);
    TCNT1  = 34286;
}

void interrupt_init(void) {
    /* INT0: fallende Flanke (ISC01=1, ISC00=0) */
    EICRA |=  (1 << ISC01);
    EICRA &= ~(1 << ISC00);
    /* INT1: fallende Flanke (ISC11=1, ISC10=0) */
    EICRA |=  (1 << ISC11);
    EICRA &= ~(1 << ISC10);
    /* INT0 und INT1 freigeben */
    EIMSK |= (1 << INT0) | (1 << INT1);
}

/* =========================================================
 * HILFSFUNKTIONEN
 * ========================================================= */

// LED_BETRIEB (PB0) aktualisieren
void update_leds(void) {
    if (stufe > 0 && !timeout_aktiv) {
        PORTB |=  (1 << PB0);   // LED_BETRIEB ein
    } else {
        PORTB &= ~(1 << PB0);   // LED_BETRIEB aus
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

void reset_timeout(void) {
    overflow_zaehler = 0;
    TCNT1 = 34286;           // 0,5-s-Preload neu laden!
    timeout_aktiv = 0;
    PORTB &= ~(1 << PB1);   // LED_WARNUNG aus (PB1)
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
        /* === TASTER_SCHNELLER: Stufe erhöhen === */
        if (taster_auf_flag) {
            taster_auf_flag = 0;
            if (debounce_pind(PD2)) {
                if (stufe < 4) {
                    stufe++;
                    OCR0B = stufen[stufe];   // OCR0B (nicht OCR0A!)
                }
                reset_timeout();
                update_leds();
            }
        }

        /* === TASTER_LANGSAMER: Stufe verringern === */
        if (taster_ab_flag) {
            taster_ab_flag = 0;
            if (debounce_pind(PD3)) {
                if (stufe > 0) {
                    stufe--;
                    OCR0B = stufen[stufe];
                }
                reset_timeout();
                update_leds();
            }
        }

        /* === TIMER1 OVERFLOW POLLING (0,5-Sekunden-Takt) ===
         *
         * Jeder Überlauf = 0,5 s
         * 30 Überläufe = 15 Sekunden → Timeout
         * Bei Timeout: LED_WARNUNG toggelt jedes Mal → 2 Hz
         */
        if (TIFR1 & (1 << TOV1)) {
            TIFR1 |= (1 << TOV1);   // TOV1-Flag löschen (1 schreiben!)
            TCNT1 = 34286;           // 0,5-s-Preload neu laden

            if (timeout_aktiv) {
                /* Timeout aktiv: Warn-LED toggeln (alle 0,5 s → 2 Hz) */
                PORTB ^= (1 << PB1);
            } else {
                overflow_zaehler++;
                if (overflow_zaehler >= 30) {   // 30 × 0,5 s = 15 s
                    timeout_aktiv = 1;
                    stufe  = 0;
                    OCR0B  = stufen[stufe];      // Lüfter aus
                    update_leds();
                }
            }
        }
    }

    return 0;
}

/*
 * === REFLEXION ===
 * 1. OC0B und OC0A teilen denselben Timer0 → gleiche Frequenz zwingend.
 *    Man kann OCR0A und OCR0B auf unterschiedliche Duty Cycles setzen,
 *    aber nicht auf unterschiedliche Frequenzen.
 *
 * 2. Überläufe für 1 Minute: 60 s / 0,5 s = 120 Überläufe.
 *
 * 3. 4 Hz mit Preload 34286: Preload für 0,25 s:
 *    Ticks = 62500/4 = 15625 → Preload = 65536-15625 = 49911
 *    Aber: dann ändert sich auch der Timeout (1 min / 0,25 s = 240 Überläufe).
 *
 * 4. PB0/PB1 → DDRB/PORTB: Port B und Port D sind getrennte Register-Sets.
 *    DDRD steuert nur PD0–PD7, DDRB nur PB0–PB5.
 */
