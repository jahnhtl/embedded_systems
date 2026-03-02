# Übung 1 – LED blinken mit DDR/PORT-Registern

**Thema:** GPIO-Konfiguration ohne Arduino-Bibliothek
**Voraussetzungen:** C-Grundlagen, Arduino Uno
**Neue Konzepte:** `DDRD`, `PORTD`, `_delay_ms()`, Bit-Operatoren

---

## Aufgabenstellung

Eine LED soll **ohne** Arduino-Bibliotheksfunktionen blinken.
Anstatt `pinMode()` und `digitalWrite()` zu verwenden, erfolgt der Zugriff direkt auf die Hardware-Register des ATmega328P.

### Hardware
| Pin | Funktion |
|-----|----------|
| PD4 | LED (mit 220 Ω Vorwiderstand gegen GND) |

### Anforderungen
1. PD4 als **digitalen Ausgang** über das Register `DDRD` konfigurieren
2. LED leuchtet **500 ms**, dann ist sie **500 ms** aus → Frequenz: **1 Hz**
3. **Verboten:** `pinMode()`, `digitalWrite()`, `delay()`, `analogWrite()`
4. **Erlaubt:** `_delay_ms()` aus `<util/delay.h>`
5. `F_CPU` muss auf `16000000UL` definiert sein (vor dem Include von `<util/delay.h>`)

### Hinweise
- **DDRD** (Data Direction Register D): Bit = 1 → Ausgang, Bit = 0 → Eingang
- **PORTD** (Port D Output Register): Bit = 1 → HIGH (5V), Bit = 0 → LOW (0V)
- Bit setzen:   `REGISTER |=  (1 << BITx)`
- Bit löschen:  `REGISTER &= ~(1 << BITx)`
- Bit toggeln:  `REGISTER ^=  (1 << BITx)`

> **Warum Register?**
> Die Arduino-Funktionen `pinMode()` und `digitalWrite()` kosten viele CPU-Zyklen.
> Direkter Registerzugriff ist ca. 50× schneller!

---

## Schritt 1: Flussdiagramm

Zeichne ein Flussdiagramm für die obige Aufgabenstellung.

---

## Schritt 2: Code-Implementierung

Erstelle ein PlatformIO Projekt in Visual Studio Code und erstelle das Programm in der Datei `main.cpp`.

Der Code soll folgende Struktur haben:

```c
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // TODO: PD4 als Ausgang konfigurieren (DDRD)

    while (1) {
        // TODO: LED einschalten (PORTD)
        _delay_ms(500);
        // TODO: LED ausschalten (PORTD)
        _delay_ms(500);
    }
    return 0;
}
```

**Fragen zur Reflexion:**
1. Was passiert, wenn `F_CPU` nicht definiert wird?
2. Welchen Wert hat `DDRD` nach dem Reset des Mikrocontrollers?
3. Wie können **zwei** LEDs gleichzeitig mit einem einzigen Register-Schreibvorgang eingeschaltet werden?
