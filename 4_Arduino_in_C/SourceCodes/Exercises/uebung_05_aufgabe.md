# Übung 5 – Timer0 Fast-PWM konfigurieren

**Thema:** PWM-Signal mit Timer0, Frequenzberechnung
**Voraussetzungen:** Übung 1 (Register-Grundlagen)
**Neue Konzepte:** `TCCR0A`, `TCCR0B`, `OCR0A`, Fast-PWM, Prescaler, Frequenzformel

---

## Aufgabenstellung

Ein PWM-Signal (Pulsweitenmodulation) ermöglicht die stufenlose Helligkeitssteuerung einer LED.
Timer0 des ATmega328P wird im **Fast-PWM-Modus** direkt über Register konfiguriert.

### Hardware
| Pin | Funktion |
|-----|----------|
| PD6 | LED_PWM (OC0A — PWM-Ausgang von Timer0!) |

> **Wichtig:** PWM funktioniert nur auf dem OC0A-Pin (**PD6**). Dieser Pin ist direkt mit dem Timer0-Komparator verbunden.

### Anforderungen
1. Timer0 im **Fast-PWM-Modus** konfigurieren (WGM02:0 = 0b011)
2. **Non-inverting Output** auf OC0A aktivieren (COM0A1=1, COM0A0=0)
3. **Prescaler 64** einstellen (CS01=1, CS00=1)
4. **OCR0A = 128** setzen (ca. 50% Duty Cycle)
5. PD6 als **Ausgang** konfigurieren (DDRD)
6. **PWM-Frequenz berechnen** und im Code kommentieren

### Pflichtberechnung (im Code als Kommentar dokumentieren!)

```
f_PWM = F_CPU / (Prescaler × 256)
f_PWM = 16.000.000 / (64 × 256)
f_PWM = 16.000.000 / 16.384
f_PWM = ??? Hz
```

> Diese Frequenz muss im Code als Kommentar dokumentiert werden!

### Register-Übersicht

**TCCR0A** – Timer/Counter Control Register 0 A:
| Bit | Name | Funktion |
|-----|------|----------|
| 7 | COM0A1 | Output Compare Mode (1 = non-inverting) |
| 6 | COM0A0 | Output Compare Mode |
| 1 | WGM01  | Waveform Generation Mode Bit 1 |
| 0 | WGM00  | Waveform Generation Mode Bit 0 |

Für Fast-PWM, non-inverting: `TCCR0A = (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);`

**TCCR0B** – Timer/Counter Control Register 0 B:
| CS02 | CS01 | CS00 | Prescaler |
|------|------|------|-----------|
| 0    | 0    | 0    | kein Takt (Timer gestoppt) |
| 0    | 0    | 1    | 1 (kein Prescaler) |
| 0    | 1    | 0    | 8 |
| 0    | 1    | 1    | **64** ← wir brauchen das |
| 1    | 0    | 0    | 256 |
| 1    | 0    | 1    | 1024 |

**OCR0A** – Output Compare Register 0 A:
- Wert 0–255 bestimmt den Duty Cycle
- `Duty Cycle = OCR0A / 255 × 100%`
- OCR0A = 128 → ≈ 50%

---

## Schritt 1: Flussdiagramm

Einfaches Flussdiagramm:
- Initialisierung: DDRD, TCCR0A, TCCR0B, OCR0A setzen
- Hauptschleife: leer (PWM läuft automatisch im Hintergrund)

---

## Schritt 2: Code-Implementierung

```c
#define F_CPU 16000000UL
#include <avr/io.h>

int main(void) {
    // PD6 als Ausgang konfigurieren (OC0A muss Ausgang sein!)
    // TODO: DDRD

    // Timer0 Fast-PWM konfigurieren
    // TODO: TCCR0A (Fast-PWM + Non-inverting)
    // TODO: TCCR0B (Prescaler 64)

    // Duty Cycle setzen (ca. 50%)
    // TODO: OCR0A

    // PWM-Frequenz (Pflichtkommentar!):
    // f_PWM = 16.000.000 / (64 × 256) = ??? Hz

    while (1) {
        // Timer läuft automatisch – Hauptschleife leer
    }
    return 0;
}
```

**Erweiterungsaufgabe:** Ändere `OCR0A` auf verschiedene Werte und beobachte die Helligkeit:
- OCR0A = 0   → ?% Helligkeit
- OCR0A = 64  → ?% Helligkeit
- OCR0A = 128 → ?% Helligkeit
- OCR0A = 191 → ?% Helligkeit
- OCR0A = 255 → ?% Helligkeit

**Fragen zur Reflexion:**
1. Was passiert, wenn Prescaler 1024 verwendet wird? Berechne die neue PWM-Frequenz.
2. Ab welcher PWM-Frequenz ist kein Flimmern mehr sichtbar (Daumenwert)?
3. Warum sollte in reinen C-Projekten **kein** `analogWrite()` verwendet werden?
