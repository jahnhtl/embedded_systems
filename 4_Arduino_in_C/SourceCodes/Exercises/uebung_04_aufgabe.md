# Übung 4 – Software-Entprellung mit Flag-Pattern

**Thema:** Robuste Interrupt-Entprellung, volatile Flags
**Voraussetzungen:** Übung 3
**Neue Konzepte:** `volatile`, Flag-Pattern, Entprellsequenz, UART-Ausgabe (alles vorgegeben)

---

## Aufgabenstellung

In Übung 3 ist zu beobachten, dass mechanische Taster **prellen** — ein einzelner Tasterdruck löst mehrere Interrupts aus.
Die Lösung: Das **Flag-Pattern**. Die ISR setzt nur ein Flag (Merker), die Hauptschleife prüft und entprellt.
Da wir das Prellen "sehen" wollen benötigen wir die Serielle Schnittstelle ebenfalls in C - diese Implementierung ist komplett vorgegeben und muss nicht selbst konfiguriert werden. Alles vorhanden inklusive Funktionen
    - `uart_puts("Text\n")` schreibt einen String ähnlich zu `Serial.println("Text")`
    - `uart_put_uint8(counter)` schreibt einen integer ähnlich zu `Serial.print(counter)`

**Das Flag-Pattern ist der robuste Standard für Interrupt-Entprellung!**

### Hardware
| Pin | Funktion |
|-----|----------|
| PD2 | Taster 1 (INT0, Pull-up) — zählt **hoch** |
| PD3 | Taster 2 (INT1, Pull-up) — zählt **runter** |
| PD4 | LED (leuchtet wenn Zähler > 0) |

### Anforderungen
1. **ISR (INT0 und INT1):** Setzt nur das jeweilige Flag (`taster1_flag` / `taster2_flag`)
2. **Hauptschleife** führt bei gesetztem Flag die Entprellsequenz durch:
   - Flag zurücksetzen
   - 20 ms warten (`_delay_ms(20)`)
   - Pin nochmals prüfen: noch LOW? (Taster wirklich gedrückt?)
   - Warten bis Taster **losgelassen** wird (`while (!(PIND & (1 << PDx)))`)
   - Dann: Zähler verändern
3. Taster1 erhöht Zähler (max. 255), Taster2 verringert (min. 0)
4. Zählerstand per **UART** auf seriellem Monitor ausgeben (9600 Baud)
5. LED leuchtet wenn Zähler > 0

### Das Flag-Pattern im Detail

```
ISR(INT0_vect):          Hauptschleife:
  taster1_flag = 1;        if (taster1_flag) {
                               taster1_flag = 0;
                               _delay_ms(20);          // Wartezeit
                               if (!(PIND & (1<<PD2))) // Noch gedrückt?
                                   while (!(PIND & (1<<PD2))); // Loslassen abwarten
                                   zaehler++;           // Verarbeiten
                               }
                           }
```

### Hinweise

**volatile:** Variablen, die in ISR und Hauptschleife geteilt werden, müssen `volatile` sein:
```c
volatile uint8_t taster1_flag = 0;
```

**Warum volatile?** Der Compiler optimiert sonst den Lesezugriff weg, weil er nicht weiß, dass die ISR den Wert ändern kann.

**UART initialisieren (9600 Baud, 16 MHz):**
```c
// UBRR = F_CPU / (16 × BAUD) - 1 = 16000000 / (16 × 9600) - 1 = 103
UBRR0H = 0;
UBRR0L = 103;
UCSR0B = (1 << TXEN0);                    // Sender aktivieren
UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8 Datenbits, 1 Stoppbit
```

**UART-Zeichen senden:**
```c
void uart_putc(char c) {
    while (!(UCSR0A & (1 << UDRE0)));  // Warten bis Puffer frei
    UDR0 = c;
}
```

---

## Schritt 1: Flussdiagramm

Zeichne zwei Abläufe:
1. **ISR:** Flag setzen → Ende
2. **Hauptschleife:** Flag gesetzt? → Entprellsequenz → Zähler ändern → UART-Ausgabe → LED aktualisieren

---

## Schritt 2: Code-Implementierung

```c
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t taster1_flag = 0;
volatile uint8_t taster2_flag = 0;
uint8_t zaehler = 0;

/* =========================================================
 * UART-Hilfsfunktionen (fertig – nicht verändern)
 * UBRR = F_CPU / (16 × BAUD) - 1 = 16000000 / (16 × 9600) - 1 = 103
 * ========================================================= */
void uart_init(void) {
    UBRR0H = 0;
    UBRR0L = 103;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_putc(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uart_puts(const char *s) {
    while (*s) uart_putc(*s++);
}

void uart_put_uint8(uint8_t val) {
    if (val >= 100) uart_putc('0' + val / 100);
    if (val >= 10)  uart_putc('0' + (val / 10) % 10);
    uart_putc('0' + val % 10);
}

/* ========================================================= */

ISR(INT0_vect) {
    // TODO: taster1_flag setzen
}

ISR(INT1_vect) {
    // TODO: taster2_flag setzen
}

int main(void) {
    uart_init();
    // TODO: GPIO konfigurieren (PD2, PD3 Eingang+Pull-up, PD4 Ausgang)
    // TODO: INT0 und INT1 auf fallende Flanke (EICRA)
    // TODO: INT0 und INT1 freigeben (EIMSK)
    // TODO: sei()

    uart_puts("Zaehler: 0\r\n");

    while (1) {
        if (taster1_flag) {
            // TODO: Entprellsequenz für Taster1 + Zähler erhöhen
            uart_puts("Zaehler: ");
            uart_put_uint8(zaehler);
            uart_puts("\r\n");
        }
        if (taster2_flag) {
            // TODO: Entprellsequenz für Taster2 + Zähler verringern
            uart_puts("Zaehler: ");
            uart_put_uint8(zaehler);
            uart_puts("\r\n");
        }
        // TODO: LED aktualisieren (PD4 ein wenn zaehler > 0)
    }
    return 0;
}
```

**Fragen zur Reflexion:**
1. Was passiert, wenn `volatile` weggelassen wird?
2. Warum wartet man auf das Loslassen des Tasters, bevor man den Zähler ändert?
3. Warum ist die 20-ms-Verzögerung in der **Hauptschleife** und nicht in der ISR?
4. Wie wird der INT1-Interrupt konfiguriert? (Welche Bits in EICRA?)
