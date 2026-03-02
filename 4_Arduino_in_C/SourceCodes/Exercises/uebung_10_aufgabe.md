# Übung 10 – Lüftergeschwindigkeits-Steuerung

**Thema:** Vollständige Systemintegration
**Voraussetzungen:** Alle Übungen 1–9
**Neue Konzepte:** OC0B statt OC0A, 0,5-Sekunden-Überlauf, 2-Hz-Blinken, 15-Sekunden-Timeout

---

## Aufgabenstellung

Zu entwickeln ist eine **Lüftersteuerung** mit automatischer Abschaltung.
Diese Aufgabe trainiert alle bisher gelernten Konzepte auf einem anderen Hardware-Setup —
um das Gelernte sicher auf unterschiedliche Pinbelegungen übertragen zu können.

**Hinweis:** Die Pinbelegung und Werte unterscheiden sich bewusst von den Vorübungen!

### Szenario
Ein Industrielüfter soll in 5 Geschwindigkeitsstufen per Taster eingestellt werden.
Eine grüne Betriebs-LED signalisiert, dass der Lüfter läuft.
Nach **15 Sekunden** Inaktivität schaltet der Lüfter automatisch ab, und eine gelbe Warn-LED blinkt mit **2 Hz**, um auf die Abschaltung hinzuweisen.

### Hardware
| Pin | Funktion |
|-----|----------|
| PD2 | TASTER_SCHNELLER (INT0, Pull-up) |
| PD3 | TASTER_LANGSAMER (INT1, Pull-up) |
| PB0 | LED_BETRIEB (grün: Lüfter läuft) |
| **PD5** | LUEFTER (**OC0B**, PWM-Ausgang!) |
| PB1 | LED_WARNUNG (gelb: blinkt 2 Hz bei Timeout) |

> **Achtung:** Der Lüfter ist an **PD5 (OC0B)** — nicht an PD6 (OC0A) wie bisher!
> Das erfordert andere Register-Bits als in den Vorübungen.

### Anforderungen

**1. GPIO:**
- PD2, PD3: Eingang + Pull-up (Taster)
- PB0: Ausgang (LED_BETRIEB)
- PD5: Ausgang (OC0B, Lüfter)
- PB1: Ausgang (LED_WARNUNG)

**2. Timer0 Fast-PWM auf OC0B (PD5):**
- Fast-PWM Modus (WGM02:0 = 0b011) wie bisher
- **Non-inverting auf OC0B**: COM0B1=1, COM0B0=0 (statt COM0A1!)
- Prescaler 64 (wie bisher)
- OCR-Register: **OCR0B** (statt OCR0A!)
- Frequenz berechnen und dokumentieren

**3. 5 Geschwindigkeitsstufen:**
- OCR0B ∈ {0, 64, 128, 191, 255} → 0%, 25%, 50%, 75%, 100%

**4. Interrupts + Entprellung:**
- INT0 (PD2, fallende Flanke): `taster_auf_flag` setzen
- INT1 (PD3, fallende Flanke): `taster_ab_flag` setzen
- Flag-Pattern mit 20 ms in der Hauptschleife (wie Übung7)

**5. Timer1 – 0,5-Sekunden-Überlauf:**

**Pflichtberechnung (im Code dokumentieren!):**
```
f_Timer = 16.000.000 / 256 = 62.500 Hz
Ticks für 0,5 s = 62.500 / 2 = 31.250
Preload = 65.536 - 31.250 = 34.286
```

- TCCR1B: Prescaler 256 (CS12=1)
- TCNT1 = 34286
- TOV1-Flag pollen und löschen

**6. Timeout nach 15 Sekunden:**
- 15 s / 0,5 s = **30 Überläufe**
- Bei Timeout: Stufe auf 0, LED_BETRIEB aus, LED_WARNUNG blinkt
- LED_WARNUNG toggelt bei **jedem Überlauf** (alle 0,5 s → 2 Hz Blinken)

**7. Tastendruck → Timeout zurücksetzen:**
- `overflow_zaehler = 0`
- `TCNT1 = 34286`
- `timeout_aktiv = 0`
- LED_WARNUNG aus

### Unterschiede zu den Vorübungen (Tabelle)

| Merkmal | Ü7–Ü9 (Vorübungen) | Ü10 (diese Aufgabe) |
|---------|---------------------|----------------------|
| PWM-Pin | PD6 (OC0A) | **PD5 (OC0B)** |
| PWM-Register | OCR0A, COM0A1 | **OCR0B, COM0B1** |
| Status-LED | PD4 | **PB0** |
| Warn-LED | PD7 | **PB1** |
| Preload Timer1 | 3036 (1 s) | **34286 (0,5 s)** |
| Timeout | 20 Überläufe = 20 s | **30 Überläufe = 15 s** |
| Blinkfrequenz | 1 Hz | **2 Hz** |

---

## Schritt 1: Flussdiagramm

Zeichne das vollständige Systemflussdiagramm (wie bei Übung9, aber mit den neuen Werten).
Beschrifte klar:
- Welche Register gesetzt werden (Initialisierung)
- Timeout-Logik mit korrekter Überlauf-Anzahl (30)
- Beide ISRs

---

## Schritt 2: Code-Implementierung

Implementiere das vollständige System von Grund auf (nicht auf Übung9 aufbauen — neue Pinbelegung!).
Empfohlene Funktionsstruktur:

```c
void gpio_init(void)       // PD2/PD3 Eingang, PB0/PD5/PB1 Ausgang
void timer0_pwm_init(void) // OC0B Fast-PWM, Prescaler 64
void timer1_init(void)     // Normal-Mode, Prescaler 256, Preload 34286
void interrupt_init(void)  // INT0/INT1 fallende Flanke
void update_leds(void)     // LED_BETRIEB (PB0) basierend auf Stufe
void reset_timeout(void)   // Overflow-Zähler, TCNT1, timeout_aktiv zurücksetzen
```

Achte auf:
- `DDRB` für PB0 und PB1 (nicht DDRD!)
- `PORTB` für PB0 und PB1
- `OCR0B` statt `OCR0A`
- `COM0B1` statt `COM0A1` in TCCR0A
- Preload 34286 statt 3036

**Fragen zur Reflexion:**
1. Warum kann man OC0B und OC0A nicht gleichzeitig auf verschiedenen Frequenzen betreiben?
2. Berechne: Wie viele Überläufe bräuchte man für einen Timeout von 1 Minute mit Preload 34286?
3. Welche Änderungen wären nötig, um die Warn-LED mit 4 Hz blinken zu lassen (ohne Preload zu ändern)?
4. Warum werden PB0 und PB1 über `DDRB`/`PORTB` gesteuert, nicht über `DDRD`/`PORTD`?
