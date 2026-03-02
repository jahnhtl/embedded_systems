# Übung 2 – Taster mit Pull-up lesen (Polling)

**Thema:** Digitaler Eingang mit internem Pull-up, Polling
**Voraussetzungen:** Übung 1
**Neue Konzepte:** Eingang konfigurieren, Pull-up, `PIND`, active-low Logik

---

## Aufgabenstellung

Ein Taster soll ausgelesen und in Abhängigkeit des Tasterzustands eine LED geschaltet werden — alles ohne Arduino-Bibliotheksfunktionen.

### Hardware
| Pin | Funktion |
|-----|----------|
| PD4 | LED (mit 220 Ω Vorwiderstand gegen GND) |
| PD2 | Taster (einer Seite an PD2, andere Seite an GND) |

### Anforderungen
1. PD4 als **Ausgang** konfigurieren (LED)
2. PD2 als **Eingang** mit internem **Pull-up-Widerstand** konfigurieren (Taster)
3. **Polling** in der Hauptschleife: Ist der Taster gedrückt → LED ein, sonst → LED aus
4. Keine Arduino-Funktionen (`pinMode()`, `digitalRead()`, `digitalWrite()` verboten)

### Hinweise

**Eingang konfigurieren:**
- `DDRD &= ~(1 << PD2)` → Bit auf 0 = Eingang (nach Reset schon 0, trotzdem explizit setzen!)

**Internen Pull-up aktivieren:**
- `PORTD |= (1 << PD2)` → Bei DDR=0 aktiviert ein 1-Bit den internen Pull-up (~50 kΩ gegen VCC)

**Taster lesen:**
- `PIND` ist das **Pin Input Register** — es liest den aktuellen physischen Zustand
- `PIND & (1 << PD2)` → 0 wenn gedrückt, ≠ 0 wenn losgelassen

**Warum active-low?**
> Mit Pull-up liegt der Pin im Ruhezustand auf HIGH (5V).
> Wenn der Taster gedrückt wird, verbindet er den Pin mit GND → PIN liest LOW.
> Daher: Taster gedrückt = logische 0 → "**active-low**"

---

## Schritt 1: Flussdiagramm

Zeichne ein Flussdiagramm für die obige Aufgabenstellung.

---

## Schritt 2: Code-Implementierung

```c
#define F_CPU 16000000UL
#include <avr/io.h>

int main(void) {
    // TODO: PD4 als Ausgang (LED)
    // TODO: PD2 als Eingang + Pull-up (Taster)

    while (1) {
        // TODO: Taster abfragen (PIND)
        // Gedrückt → LED ein, losgelassen → LED aus
    }
    return 0;
}
```

**Fragen zur Reflexion:**
1. Was passiert ohne Pull-up am Taster-Pin? (Stichwort: floating pin)
2. Wie muss der Code geändert werden, wenn der Taster active-high wäre (Pull-down statt Pull-up)?
3. Welchen Unterschied gibt es zwischen `PORTD` und `PIND`?
