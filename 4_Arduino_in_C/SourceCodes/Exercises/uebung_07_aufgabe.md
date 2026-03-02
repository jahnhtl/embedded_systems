# Übung 7 – Interrupts + Entprellung + PWM kombiniert

**Thema:** Vollständige Helligkeitssteuerung mit Interrupt-Entprellung
**Voraussetzungen:** Übungen 4, 5 und 6
**Neue Konzepte:** INT1 konfigurieren, Kombination aller bisherigen Konzepte

---

## Aufgabenstellung

In dieser Übung werden alle bisherigen Konzepte kombiniert:
- **INT0/INT1** (Übung3) mit **Flag-Pattern-Entprellung** (Übung4)
- **5-Stufen-PWM** (Übung6)
- **Grüne Status-LED**

Das Ergebnis ist die erste vollständige Kombination aller bisherigen Konzepte!

### Hardware
| Pin | Funktion |
|-----|----------|
| PD2 | Taster 1 (INT0, Pull-up) — Helligkeit **rauf** |
| PD3 | Taster 2 (INT1, Pull-up) — Helligkeit **runter** |
| PD4 | LED_STATUS (leuchtet wenn Helligkeit > 0) |
| PD6 | LED_PWM (OC0A, PWM-Helligkeit) |

### Anforderungen
1. **INT0** (PD2, fallende Flanke) → `taster1_flag` setzen
2. **INT1** (PD3, fallende Flanke) → `taster2_flag` setzen
3. **Entprellung** in der Hauptschleife (Flag-Pattern aus Übung4):
   - Flag zurücksetzen
   - 20 ms warten
   - Pin prüfen (noch LOW?)
   - Warten bis losgelassen
   - Dann Helligkeit ändern
4. **5 Helligkeitsstufen** mit OCR0A ∈ {0, 64, 128, 191, 255}
5. **Grüne LED** (PD4): An wenn Helligkeit > 0, aus bei Helligkeit = 0
6. Kein `_delay_ms()` in der Hauptschleife außer für Entprellung
7. Kein `delay()`, kein `analogWrite()`, keine Arduino-Bibliothek

### INT1 konfigurieren

INT1 verwendet die **Bits ISC11 und ISC10** in EICRA:

```c
// INT1 auf fallende Flanke:
EICRA |= (1 << ISC11);
EICRA &= ~(1 << ISC10);

// INT1 freigeben:
EIMSK |= (1 << INT1);
```

ISR für INT1:
```c
ISR(INT1_vect) {
    taster2_flag = 1;
}
```

### Struktur des Programms

```
Globale Variablen:
  volatile uint8_t taster1_flag, taster2_flag
  uint8_t stufe (0-4)
  uint8_t stufen[] = {0, 64, 128, 191, 255}

ISR(INT0_vect): taster1_flag = 1
ISR(INT1_vect): taster2_flag = 1

Hilfsfunktionen:
  void gpio_init(void)
  void timer0_pwm_init(void)
  void interrupt_init(void)
  void update_leds(void)
  void debounce_taster1(void)
  void debounce_taster2(void)

main():
  Initialisierungen aufrufen
  sei()
  Hauptschleife: Flags prüfen → entprellen → LED aktualisieren
```

---

## Schritt 1: Flussdiagramm

Zeichne das vollständige Flussdiagramm mit:
- ISR(INT0_vect) und ISR(INT1_vect)
- Hauptschleife mit beiden Flag-Abfragen und Entprellsequenzen

---

## Schritt 2: Code-Implementierung

Implementiere das Programm mit den oben beschriebenen Funktionen.
Orientiere dich an den Lösungen aus Übung4 (Entprellung) und Übung6 (PWM + Stufen).

Achte besonders auf:
- Korrekte `volatile`-Deklaration der Flags
- `OCR0A` direkt nach Stufenänderung aktualisieren
- Grüne LED wird **in der Hauptschleife** nach jeder Änderung aktualisiert

**Fragen zur Reflexion:**
1. Warum ist die Entprellung in der Hauptschleife und nicht in der ISR?
2. Was passiert, wenn beide Taster gleichzeitig gedrückt werden?
3. Welche Funktion hat `cli()` und wann würde man es verwenden?
4. Könnte man Timer0 für Entprellung statt `_delay_ms()` verwenden? Was wäre der Vorteil?
