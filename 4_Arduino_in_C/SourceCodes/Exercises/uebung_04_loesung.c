/*
 * Übung 4 – Software-Entprellung mit Flag-Pattern
 * Thema: Robuste Entprellung, volatile Flags, UART-Ausgabe
 *
 * Hardware:
 *   PD2 – Taster 1 (INT0, Pull-up) → zählt hoch
 *   PD3 – Taster 2 (INT1, Pull-up) → zählt runter
 *   PD4 – LED (leuchtet wenn Zähler > 0)
 *
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* === GLOBALE VARIABLEN ===
 * volatile: Compiler darf diese Variablen NICHT wegoptimieren,
 * da sie zwischen ISR und Hauptschleife geteilt werden! */
volatile uint8_t taster1_flag = 0;
volatile uint8_t taster2_flag = 0;
uint8_t zaehler = 0;

/* =========================================================
 * UART-Hilfsfunktionen (Register-Ebene, kein Serial.begin)
 * ========================================================= */

// UART initialisieren: 9600 Baud, 8N1
// UBRR = F_CPU / (16 × BAUD) - 1 = 16000000 / (16 × 9600) - 1 = 103
void uart_init(void) {
    UBRR0H = 0;
    UBRR0L = 103;
    UCSR0B = (1 << TXEN0);                    // Sender aktivieren
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8 Datenbits, 1 Stoppbit
}

// Einzelnes Zeichen senden
void uart_putc(char c) {
    while (!(UCSR0A & (1 << UDRE0)));  // Warten bis Sendepuffer frei
    UDR0 = c;
}

// String senden
void uart_puts(const char *s) {
    while (*s) uart_putc(*s++);
}

// Unsigned Integer (0-255) als Dezimalzahl senden
void uart_put_uint8(uint8_t val) {
    if (val >= 100) uart_putc('0' + val / 100);
    if (val >= 10)  uart_putc('0' + (val / 10) % 10);
    uart_putc('0' + val % 10);
}

/* =========================================================
 * INTERRUPT SERVICE ROUTINEN
 * ========================================================= */

// ISR setzt NUR das Flag – keine Verarbeitung hier!
ISR(INT0_vect) {
    taster1_flag = 1;
}

ISR(INT1_vect) {
    taster2_flag = 1;
}

/* =========================================================
 * ENTPRELL-HILFSFUNKTION
 * ========================================================= */

/*
 * Entprellsequenz (Flag-Pattern):
 * 1. Flag zurücksetzen
 * 2. 20 ms warten (Prellzeit überbrücken)
 * 3. Pin prüfen: noch LOW? (bestätigt echten Tasterdruck)
 * 4. Warten bis Taster losgelassen wird
 * 5. Rückgabe: 1 wenn echter Tasterdruck, 0 wenn Fehlauslösung
 */
uint8_t debounce(volatile uint8_t *flag, uint8_t pin) {
    *flag = 0;
    _delay_ms(20);
    if (!(PIND & (1 << pin))) {         // Noch gedrückt?
        while (!(PIND & (1 << pin)));   // Warten bis losgelassen
        return 1;                        // Echter Tasterdruck bestätigt
    }
    return 0;                            // Preller – ignorieren
}

/* =========================================================
 * HAUPTPROGRAMM
 * ========================================================= */

int main(void) {
    // === GPIO ===
    DDRD |=  (1 << PD4);                    // PD4 Ausgang (LED)
    DDRD &= ~((1 << PD2) | (1 << PD3));     // PD2, PD3 Eingang
    PORTD |=  (1 << PD2) | (1 << PD3);      // Pull-up aktivieren

    // === UART ===
    uart_init();

    // === INT0: fallende Flanke (ISC01=1, ISC00=0) ===
    EICRA |=  (1 << ISC01);
    EICRA &= ~(1 << ISC00);

    // === INT1: fallende Flanke (ISC11=1, ISC10=0) ===
    EICRA |=  (1 << ISC11);
    EICRA &= ~(1 << ISC10);

    // === INT0 und INT1 freigeben ===
    EIMSK |= (1 << INT0) | (1 << INT1);

    // === Globale Interrupts freigeben ===
    sei();

    uart_puts("Zaehler: 0\r\n");

    while (1) {
        // --- Taster 1: Zähler erhöhen ---
        if (taster1_flag) {
            if (debounce(&taster1_flag, PD2)) {
                if (zaehler < 255) zaehler++;
                uart_puts("Zaehler: ");
                uart_put_uint8(zaehler);
                uart_puts("\r\n");
            }
        }

        // --- Taster 2: Zähler verringern ---
        if (taster2_flag) {
            if (debounce(&taster2_flag, PD3)) {
                if (zaehler > 0) zaehler--;
                uart_puts("Zaehler: ");
                uart_put_uint8(zaehler);
                uart_puts("\r\n");
            }
        }

        // --- LED aktualisieren ---
        if (zaehler > 0) {
            PORTD |=  (1 << PD4);   // LED ein
        } else {
            PORTD &= ~(1 << PD4);   // LED aus
        }
    }

    return 0;
}

/*
 * === REFLEXION ===
 * 1. Ohne volatile: Compiler speichert Flag nur im Register (optimiert).
 *    Hauptschleife sieht nie die Änderung durch die ISR → Taster reagiert nicht.
 *
 * 2. Warten auf Loslassen: Verhindert mehrfaches Zählen bei langem Drücken.
 *    Ohne Loslassen-Warten: ein Druck zählt, dann beim nächsten Preller nochmal.
 *
 * 3. 20 ms im Hauptprogramm: In der ISR ist _delay_ms verboten (blockiert
 *    alle anderen Interrupts). ISR muss blitzschnell sein.
 *
 * 4. INT1: ISC11 und ISC10 in EICRA (Bits 3 und 2, für INT0 sind es Bits 1 und 0).
 */
