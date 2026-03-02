# Übung 3 – Externe Interrupts mit INT0

**Thema:** Hardware-Interrupts, ISR, Flankensteuerung
**Voraussetzungen:** Übungen 1 und 2
**Neue Konzepte:** `EICRA`, `EIMSK`, `sei()`, `ISR(INT0_vect)`, `<avr/interrupt.h>`

---

## Aufgabenstellung

Bisher wurde der Taster per **Polling** abgefragt (Hauptschleife prüft ständig den Pin).
Jetzt soll ein **Interrupt** den Tasterdruck erkennen: Der Mikrocontroller führt seine Hauptaufgabe aus und wird durch den Taster **unterbrochen**.

### Hardware
| Pin | Funktion |
|-----|----------|
| PD4 | LED (mit 220 Ω Vorwiderstand gegen GND) |
| PD2 | Taster (gegen GND) → **INT0** |

> PD2 ist beim ATmega328P fest mit dem externen Interrupt **INT0** verbunden.

### Anforderungen
1. PD2 als Eingang mit Pull-up konfigurieren (wie Übung2)
2. PD4 als Ausgang konfigurieren (LED)
3. INT0 auf **fallende Flanke** konfigurieren (Taster gedrückt = HIGH → LOW)
4. INT0 freigeben (EIMSK)
5. Globale Interrupts freigeben (`sei()`)
6. In der **ISR**: LED auf PD4 toggeln
7. Hauptschleife ist leer (der µC wartet auf Interrupts)

> **Hinweis:** Keine Entprellung in dieser Übung — das kommt in Übung 4!
> Es ist zu beobachten, dass ein einzelner Tasterdruck mehrere Interrupts auslösen kann.

### Register-Übersicht

**EICRA** – External Interrupt Control Register A (steuert Flankenerkennung):
| ISC01 | ISC00 | Auslöser |
|-------|-------|----------|
| 0 | 0 | LOW-Pegel |
| 0 | 1 | jede Flanke |
| 1 | 0 | fallende Flanke ← das wollen wir |
| 1 | 1 | steigende Flanke |

**EIMSK** – External Interrupt Mask Register:
- Bit `INT0` auf 1 setzen → INT0 freigeben

**ISR-Syntax:**
```c
#include <avr/interrupt.h>

ISR(INT0_vect) {
    // Wird bei Interrupt ausgeführt
}
```

---

## Schritt 1: Flussdiagramm

Zeichne **zwei** getrennte Abläufe:
1. **Hauptprogramm:** Initialisierung → Endlosschleife (leer)
2. **ISR INT0:** LED toggeln → Rückkehr

---

## Schritt 2: Code-Implementierung

```c
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(INT0_vect) {
    // TODO: LED auf PD4 toggeln
}

int main(void) {
    // TODO: PD4 als Ausgang
    // TODO: PD2 als Eingang + Pull-up

    // TODO: EICRA – INT0 auf fallende Flanke
    // TODO: EIMSK – INT0 freigeben
    // TODO: sei() – globale Interrupts freigeben

    while (1) {
        // Hauptschleife leer – Interrupt übernimmt die Arbeit
    }
    return 0;
}
```

**Fragen zur Reflexion:**
1. Was bedeutet ISR? Wer ruft die ISR auf?
2. Was passiert, wenn `sei()` nicht aufgerufen wird?
3. Beobachtung des Prellens: Wie viele Toggl-Vorgänge passieren bei einem einzelnen Tasterdruck? Was ist an der LED zu sehen?
4. Warum darf eine ISR möglichst kurz sein?
