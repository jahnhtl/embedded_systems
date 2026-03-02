# Übung 9 – Inaktivitäts-Timeout nach 60 Sekunden

**Thema:** Zeitgesteuertes Timeout, Overflow-Zähler, bedingte LED-Steuerung
**Voraussetzungen:** Übungen 7 und 8
**Neue Konzepte:** Overflow-Zähler, Timeout-Logik, Rote LED blinkt nur bei Timeout

---

## Aufgabenstellung

Das System aus Übung 7 (Helligkeitssteuerung) bekommt eine **automatische Abschaltung**:
Nach **60 Sekunden ohne Tastendruck** wird die PWM LED auf 0% gedimmt und eine rote LED beginnt zu blinken.
Jeder Tastendruck setzt den Timer zurück.

**Hier kommen alle Konzepte aus Übung1–Übung8 zusammen!**

### Hardware
| Pin | Funktion |
|-----|----------|
| PD2 | Taster 1 (INT0, Pull-up) — Helligkeit rauf |
| PD3 | Taster 2 (INT1, Pull-up) — Helligkeit runter |
| PD4 | LED_STATUS (aktiver Betrieb) |
| PD6 | LED_PWM (OC0A, PWM) |
| PD7 | LED_ROT (blinkt bei Timeout) |

### Anforderungen
1. Komplettes System aus Übung7: INT0/INT1 + Entprellung + 5-Stufen-PWM + grüne LED
2. **Timer1** (Normal-Mode, Prescaler 256, TCNT1=3036) läuft parallel mit
3. **Timeout-Zähler:** Bei jedem Timer1-Überlauf → `overflow_zaehler++`
4. Bei `overflow_zaehler >= 60` (= 60 Sekunden):
   - `timeout_aktiv = 1` setzen
   - `stufe = 0` setzen → LED_PWM auf 0%
5. **Bei timeout_aktiv:**
   - LED_ROT blinkt: bei jedem Timer1-Überlauf toggeln (= 1 Hz)
   - LED_STATUS aus
6. **Tastendruck:**
   - `overflow_zaehler = 0` zurücksetzen
   - `TCNT1 = 3036` neu laden
   - `timeout_aktiv = 0`
   - LED_ROT aus
7. Rote LED blinkt **NUR** wenn Timeout aktiv (nicht im Normalbetrieb!)

### Neue globale Variablen

```c
volatile uint8_t overflow_zaehler = 0;
volatile uint8_t timeout_aktiv    = 0;
```

### Timeout-Reset-Funktion

```c
void reset_timeout(void) {
    overflow_zaehler = 0;
    TCNT1 = 3036;       // Timer neu starten
    timeout_aktiv = 0;
    // Rote LED ausschalten
    PORTD &= ~(1 << PD7);
}
```

Aufruf: Am Ende der Entprellsequenz (nach jedem bestätigten Tastendruck).

### Timer1-Handling in der Hauptschleife

```c
if (TIFR1 & (1 << TOV1)) {
    TIFR1 |= (1 << TOV1);   // Flag löschen
    TCNT1 = 3036;            // Preload neu laden

    if (timeout_aktiv) {
        // Rote LED toggeln (1 Hz blinken)
        PORTD ^= (1 << PD7);
    } else {
        overflow_zaehler++;
        if (overflow_zaehler >= 60) {
            timeout_aktiv = 1;
            stufe = 0;
            OCR0A = stufen[stufe];
        }
    }
}
```

---

## Schritt 1: Flussdiagramm

Zeichne das vollständige Flussdiagramm:
- ISR (INT0/INT1)
- Hauptschleife: Taster-Handling + Timer1-Handling
- Timeout-Logik klar darstellen (mit Verzweigung: Timeout aktiv / nicht aktiv)

---

## Schritt 2: Code-Implementierung

Ausgehend von der Lösung aus Übung 7 sind folgende Ergänzungen nötig:
1. Timer1-Konfiguration (Prescaler 256, TCNT1=3036)
2. `overflow_zaehler` und `timeout_aktiv` Variablen
3. `reset_timeout()` Funktion
4. Timer1-Polling-Block in der Hauptschleife
5. `reset_timeout()` am Ende jeder Entprellsequenz aufrufen

**Fragen zur Reflexion:**
1. Warum muss `TCNT1` in `reset_timeout()` neu geladen werden?
2. Was passiert, wenn `timeout_aktiv` beim Tastendruck nicht auf 0 zurückgesetzt wird?
3. Wie wird der Timeout auf 30 Sekunden verlängert? Was ändert sich?
4. Warum toggelt die rote LED bei jedem Überlauf (und nicht bei jedem zweiten)?
