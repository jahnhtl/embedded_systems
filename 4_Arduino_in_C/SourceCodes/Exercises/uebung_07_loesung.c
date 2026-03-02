/*
 * Übung 7 – Interrupts + Entprellung + PWM kombiniert
 * Thema: Vollständige Helligkeitssteuerung mit Flag-Pattern
 *
 * Hardware:
 *   PD2 – Taster 1 (INT0, Pull-up) → Helligkeit rauf
 *   PD3 – Taster 2 (INT1, Pull-up) → Helligkeit runter
 *   PD4 – LED_STATUS (leuchtet wenn Helligkeit > 0)
 *   PD6 – LED_PWM (OC0A, PWM)
 *
 * Kombiniert alle Konzepte: Interrupts, Entprellung und PWM
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* === GLOBALE VARIABLEN === */
volatile uint8_t taster1_flag = 0;
volatile uint8_t taster2_flag = 0;

uint8_t stufen[5] = {0, 64, 128, 191, 255};
uint8_t stufe = 0;

/* === ISRs – nur Flag setzen! === */
ISR(INT0_vect) { taster1_flag = 1; }
ISR(INT1_vect) { taster2_flag = 1; }

/* =========================================================
 * INITIALISIERUNGSFUNKTIONEN
 * ========================================================= */

void gpio_init(void) {
    // PD2, PD3: Eingang + Pull-up
    DDRD &= ~((1 << PD2) | (1 << PD3));
    PORTD |=  (1 << PD2) | (1 << PD3);

    // PD4: Ausgang (LED_STATUS)
    DDRD |= (1 << PD4);

    // PD6: Ausgang (LED_PWM, OC0A)
    DDRD |= (1 << PD6);
}

void timer0_pwm_init(void) {
    /* Fast-PWM, Non-inverting auf OC0A, Prescaler 64
     * f_PWM = 16.000.000 / (64 × 256) = 976,6 Hz */
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01) | (1 << CS00);
    OCR0A  = stufen[stufe];   // Startwert
}

void interrupt_init(void) {
    // INT0 auf fallende Flanke (ISC01=1, ISC00=0)
    EICRA |=  (1 << ISC01);
    EICRA &= ~(1 << ISC00);

    // INT1 auf fallende Flanke (ISC11=1, ISC10=0)
    EICRA |=  (1 << ISC11);
    EICRA &= ~(1 << ISC10);

    // INT0 und INT1 freigeben
    EIMSK |= (1 << INT0) | (1 << INT1);
}

/* =========================================================
 * HILFSFUNKTIONEN
 * ========================================================= */

// LED_STATUS basierend auf aktueller Stufe aktualisieren
void update_leds(void) {
    if (stufe > 0) {
        PORTD |=  (1 << PD4);   // LED_STATUS ein
    } else {
        PORTD &= ~(1 << PD4);   // LED_STATUS aus
    }
}

// Entprellung für Taster an Pin 'pin' in Port D
// Gibt 1 zurück wenn echter Tasterdruck, 0 bei Preller
uint8_t debounce_pind(uint8_t pin) {
    _delay_ms(20);
    if (!(PIND & (1 << pin))) {         // Noch gedrückt nach 20 ms?
        while (!(PIND & (1 << pin)));   // Warten bis losgelassen
        return 1;
    }
    return 0;   // Preller – ignorieren
}

/* =========================================================
 * HAUPTPROGRAMM
 * ========================================================= */

int main(void) {
    gpio_init();
    timer0_pwm_init();
    interrupt_init();
    sei();   // Globale Interrupts freigeben

    while (1) {
        /* === TASTER 1: Helligkeit rauf === */
        if (taster1_flag) {
            taster1_flag = 0;
            if (debounce_pind(PD2)) {
                if (stufe < 4) {
                    stufe++;
                    OCR0A = stufen[stufe];
                }
                update_leds();
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
                update_leds();
            }
        }
    }

    return 0;
}

/*
 * === REFLEXION ===
 * 1. Entprellung in Hauptschleife: ISR darf nicht blockieren.
 *    _delay_ms in ISR würde alle Interrupts für 20 ms sperren.
 *
 * 2. Beide Taster gleichzeitig: Beide Flags gesetzt. Die Hauptschleife
 *    verarbeitet sie nacheinander (Reihenfolge: taster1 zuerst).
 *
 * 3. cli(): Sperrt alle Interrupts (I-Bit = 0). Verwendung z.B. beim
 *    atomaren Lesen/Schreiben von 16-Bit-Variablen.
 *
 * 4. Timer0 für Entprellung: Möglich, aber Timer0 ist schon für PWM genutzt.
 *    Timer1 wäre verfügbar → komplexer, aber nicht-blockierend.
 */
