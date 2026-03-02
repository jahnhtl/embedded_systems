# Übung 8 – Timer1: Sekundentakt per Overflow-Polling

**Thema:** Timer1 im Normal-Mode, Overflow-Flag pollen
**Voraussetzungen:** Übungen 1 und 5 (Grundlagen Register)
**Neue Konzepte:** `TCCR1B`, `TCNT1` Preload, `TIFR1`, `TOV1`, Overflow-Berechnung

---

## Aufgabenstellung

Timer0 haben wir für PWM genutzt. Jetzt kommt **Timer1** — ein 16-Bit-Timer, ideal für längere Zeitintervalle.
Timer1 wird so konfiguriert, dass er exakt **jede Sekunde** überläuft (Normal-Mode + Overflow-Polling).

### Hardware
| Pin | Funktion |
|-----|----------|
| PD7 | LED_ROT (toggelt jede Sekunde) |

### Anforderungen
1. Timer1 im **Normal-Mode** (kein CTC, kein PWM)
2. **Prescaler 256**
3. **TCNT1 Preload = 3036** (für exakt 1-Sekunden-Überlauf)
4. In der Hauptschleife: **TOV1-Flag pollen** (kein Interrupt!)
5. Bei Überlauf: LED toggeln, **TCNT1 neu laden**, **TOV1-Flag löschen**

### Pflichtberechnung (im Code dokumentieren!)

```
Timer-Taktfrequenz nach Prescaler:
  f_Timer = F_CPU / Prescaler = 16.000.000 / 256 = 62.500 Hz

Ticks pro Sekunde = 62.500

Timer1 ist 16-Bit → zählt von TCNT1 bis 65.535, dann Überlauf
Gewünschte Zeitdauer = 1 Sekunde = 62.500 Ticks

Preload = 65.536 - 62.500 = 3.036
```

> **Hinweis:** Beim Reset startet TCNT1 bei 0. Das ergibt einen Überlauf nach
> 65.536 Ticks = 1,049 Sekunden — zu lang! Daher Preload = 3.036.

### Register-Übersicht

**TCCR1B** – Timer/Counter 1 Control Register B:
| Bit | Name | Funktion |
|-----|------|----------|
| 2   | CS12 | Clock Select |
| 1   | CS11 | Clock Select |
| 0   | CS10 | Clock Select |

Prescaler 256 → CS12=1, CS11=0, CS10=0:
```c
TCCR1B = (1 << CS12);
```

**TCNT1** – 16-Bit Zählerregister:
```c
TCNT1 = 3036;  // Preload setzen
```

**TIFR1** – Timer/Counter 1 Interrupt Flag Register:
- **TOV1-Bit**: wird 1 bei Überlauf
- Lesen: `TIFR1 & (1 << TOV1)` — 1 wenn Überlauf aufgetreten
- **Löschen: `TIFR1 |= (1 << TOV1)`** ← **Eine 1 schreiben** (kein = !)

> **Achtung: TOV1-Flag-Besonderheit!**
> Anders als andere Flags wird TOV1 durch Schreiben einer **1** gelöscht, nicht durch Schreiben einer 0.
> `TIFR1 = (1 << TOV1)` oder `TIFR1 |= (1 << TOV1)` — beides funktioniert.

### Ablauf in der Hauptschleife

```
Endlosschleife:
  Prüfe ob TOV1 gesetzt:
    if (TIFR1 & (1 << TOV1)) {
        TIFR1 |= (1 << TOV1);   // Flag löschen
        TCNT1 = 3036;            // Preload neu laden
        // LED toggeln
    }
```

---

## Schritt 1: Flussdiagramm

Zeichne ein Flussdiagramm für die obige Aufgabenstellung.

---

## Schritt 2: Code-Implementierung

```c
#define F_CPU 16000000UL
#include <avr/io.h>

int main(void) {
    // PD7 als Ausgang (LED_ROT)
    // TODO: DDRD

    // Timer1 konfigurieren
    // f_Timer = 16.000.000 / 256 = 62.500 Hz
    // Preload = 65.536 - 62.500 = 3.036
    // TODO: TCCR1B (Prescaler 256)
    // TODO: TCNT1 (Preload laden)

    while (1) {
        // TOV1-Flag pollen
        if (TIFR1 & (1 << TOV1)) {
            // TODO: TOV1-Flag löschen (TIFR1)
            // TODO: TCNT1 neu laden (3036)
            // TODO: PD7 toggeln (PORTD)
        }
    }
    return 0;
}
```

**Fragen zur Reflexion:**
1. Was ist der Unterschied zwischen Polling und Interrupt-basiertem Ansatz?
2. Warum verwendet man Timer1 statt Timer0 für lange Zeitintervalle?
3. Berechne den Preload für eine Zeitdauer von **0,5 Sekunden** (Prescaler 256).
4. Was passiert, wenn TCNT1 nach dem Überlauf nicht neu geladen wird?
