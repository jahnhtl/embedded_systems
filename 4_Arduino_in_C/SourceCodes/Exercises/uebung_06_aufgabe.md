# Übung 6 – 5-stufige Helligkeitssteuerung (Polling)

**Thema:** Stufenweise PWM-Steuerung, OCR0A-Werte
**Voraussetzungen:** Übungen 2 und 5
**Neue Konzepte:** Stufen-Array, OCR0A-Werte für definierte Helligkeiten, Status-LED

---

## Aufgabenstellung

Die LED soll in **5 definierten Helligkeitsstufen** gesteuert werden:
ein Taster erhöht die Helligkeit, der andere verringert sie.
In dieser Übung noch per einfachem **Polling** (Interrupts folgen in Übung7).

### Hardware
| Pin | Funktion |
|-----|----------|
| PD2 | Taster 1 (Pull-up) — Helligkeit **rauf** |
| PD3 | Taster 2 (Pull-up) — Helligkeit **runter** |
| PD4 | LED_STATUS (leuchtet wenn Helligkeit > 0) |
| PD6 | LED_PWM (OC0A, PWM-Helligkeit) |

### Anforderungen
1. Timer0 Fast-PWM auf PD6 (wie Übung5)
2. Taster per **Polling** abfragen (keine Interrupts)
3. Einfache Entprellung durch `_delay_ms(200)` nach Tasterdruck
4. **5 Helligkeitsstufen** mit folgenden OCR0A-Werten:

| Stufe | OCR0A | Helligkeit |
|-------|-------|-----------|
| 0     | 0     | 0 %       |
| 1     | 64    | 25 %      |
| 2     | 128   | 50 %      |
| 3     | 191   | 75 %      |
| 4     | 255   | 100 %     |

5. **Grüne LED (PD4):** Leuchtet wenn Helligkeit > 0 (Stufe ≥ 1), aus bei Stufe 0
6. Unter Stufe 0 und über Stufe 4 kann nicht gegangen werden (Begrenzung)

### Hinweise

**Stufen-Array verwenden:**
```c
uint8_t stufen[5] = {0, 64, 128, 191, 255};
uint8_t stufe = 0;  // aktueller Index (0-4)
```

**Begrenzung:**
```c
if (stufe < 4) stufe++;   // erhöhen – aber max. 4
if (stufe > 0) stufe--;   // verringern – aber min. 0
```

**OCR0A aktualisieren:**
```c
OCR0A = stufen[stufe];
```

**Polling mit einfacher Entprellung:**
```c
if (!(PIND & (1 << PD2))) {    // Taster 1 gedrückt?
    stufe++;                    // Stufe erhöhen
    OCR0A = stufen[stufe];
    _delay_ms(200);             // Entprellung (Vereinfachung)
}
```

> **Hinweis:** Diese einfache Entprellung mit `_delay_ms()` blockiert die Hauptschleife.
> Das Flag-Pattern aus Übung 4 ist die robustere Lösung (kommt in einer späteren Übung).

---

## Schritt 1: Flussdiagramm

Zeichne ein Flussdiagramm für die obige Aufgabenstellung.

---

## Schritt 2: Code-Implementierung

```c
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Helligkeitsstufen
    uint8_t stufen[5] = {0, 64, 128, 191, 255};
    uint8_t stufe = 0;

    // TODO: GPIO konfigurieren
    //   PD2, PD3 als Eingang + Pull-up (Taster)
    //   PD4 als Ausgang (LED_STATUS)
    //   PD6 als Ausgang (LED_PWM, OC0A)

    // TODO: Timer0 Fast-PWM konfigurieren (wie Übung5)
    OCR0A = stufen[stufe];  // Startwert: 0%

    while (1) {
        // Taster 1 – Helligkeit rauf
        if (!(PIND & (1 << PD2))) {
            // TODO: Stufe erhöhen (begrenzt auf max. 4)
            // TODO: OCR0A aktualisieren
            _delay_ms(200);
        }
        // Taster 2 – Helligkeit runter
        if (!(PIND & (1 << PD3))) {
            // TODO: Stufe verringern (begrenzt auf min. 0)
            // TODO: OCR0A aktualisieren
            _delay_ms(200);
        }
        // TODO: LED_STATUS (PD4) steuern
        //   Stufe > 0 → PD4 einschalten
        //   Stufe == 0 → PD4 ausschalten
    }
    return 0;
}
```

**Fragen zur Reflexion:**
1. Warum ist `_delay_ms(200)` eine schlechte Entprellung für ein Gesamtsystem (Stichwort: Reaktionszeit)?
2. Welchen OCR0A-Wert würde man für genau 33% Duty Cycle verwenden?
3. Was ist der Unterschied zwischen OCR0A und TCNT0?
