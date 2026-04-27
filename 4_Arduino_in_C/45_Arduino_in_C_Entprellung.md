# Entprellung von Tastern – Selbststudium

**Thema:** Taster entprellen mit externem Interrupt INT0  
**Voraussetzungen:** IKapitel 20 (Hardware-Interrupts), `EICRA`, `EIMSK`, `volatile`-Flags  
**Neue Konzepte:** Prellen, Entprellung per `_delay_ms`, Entprellung per Mehrfach-Abtastung, Hilfsfunktionen

---

## Theorie: Was ist Prellen?

Mechanische Taster bestehen aus zwei Metallkontakten, die beim Drücken zusammengeführt werden. Beim Schließen oder Öffnen prellen die Kontakte kurzzeitig hin und her – typischerweise für **5…20 ms**. Für einen Mikrocontroller, der in Mikrosekunden-Takt arbeitet, sieht das wie viele schnelle Tastendrücke hintereinander aus.

```
Ideales Signal:          Reales Signal (mit Prellen):

     ──┐                     ──┐ ┌┐┌┐┌─────
       └─────                  └┘└┘└┘
```

Ohne Entprellung reagiert die Interrupt-ISR mehrfach auf einen einzigen Tastendruck.

### Ursachen

- Federkraft und Massenträgheit der Kontakte
- Rauigkeit der Kontaktoberflächen
- Übergangswiderstände beim Schließen

### Lösungsansätze

| Methode | Prinzip | Aufwand |
|---------|---------|---------|
| RC-Glied + Schmitt-Trigger | Hardwarelösung: Kondensator glättet das Signal | Hardwarekomponenten nötig |
| `_delay_ms` nach ISR | Prellzeit abwarten, dann Zustand prüfen | Einfach, blockiert kurz |
| Mehrfach-Abtastung | Mehrmals sampeln, Mehrheitswert | Robuster, deterministisch |
| Timer-basiert | Regelmäßige Abtastung im Timer-Interrupt | Nicht-blockierend |

---

## Schaltung

```
Arduino Uno

  PD2 (Pin 2) ──┤ Taster ├── GND
                             (interner Pull-up aktiv → Low-aktiv)

  PB5 (Pin 13) ── Onboard LED (bereits auf der Platine)
```

> **Merke:** `PORTD |= (1 << PD2)` aktiviert den internen Pull-up.  
> Im Ruhezustand liegt `PD2` auf HIGH, beim Drücken auf LOW (fallende Flanke).

---

## Übung 8 – Entprellung mit `_delay_ms`

**Quelldatei:** `SourceCodes/Examples/8_externer_interrupt_int0_entprellung_delay.cpp`

### Funktionsprinzip

```
ISR(INT0_vect) wird ausgelöst
    → taster_flag = 1 setzen

In der Hauptschleife:
    → _delay_ms(20)           // Prellzeit abklingen lassen
    → PD2 nochmals prüfen     // Ist der Taster noch gedrückt?
        → ja: LED toggeln
              warten bis Taster losgelassen (while-Schleife)
              _delay_ms(20)   // Prellen beim Loslassen abwarten
        → nein: Fehlauslösung verwerfen
```

### Fragen zu Übung 8

> **Hinweis:** Die Antworten sind im Abgabe-Dokument schriftlich festzuhalten.

1. **Erklärung `volatile`:** Warum muss `taster_flag` als `volatile` deklariert werden? Was würde ohne `volatile` passieren?

2. **Fallende Flanke:** Erkläre, welche Bits in `EICRA` für eine fallende Flanke auf INT0 gesetzt werden müssen. Schreibe die beiden nötigen Zeilen auf und begründe den Wert jedes Bits.

3. **Prellzeit:** Welchen Wert hat `ENTPRELL_MS`? Warum wartet man genau diese Zeit und nicht z. B. nur 1 ms oder 100 ms?

4. **Bestätigungscheck:** Nach dem Warten prüft das Programm erneut, ob `PD2` noch LOW ist. Warum ist diese Prüfung notwendig? Was passiert ohne sie?

5. **Blockierung:** Die `while (!(PIND & (1 << PD2)))` Schleife blockiert die CPU. Nenne einen Nachteil dieses Verhaltens in einer realen Anwendung.

---

## Übung 9 – Saubere Entprellung per Timer1 (nicht-blockierend)

**Quelldatei:** `SourceCodes/Examples/9_externer_interrupt_int0_entprellung_sauber.cpp`

### Funktionsprinzip

Statt die CPU mit `_delay_ms` zu blockieren, übernimmt **Timer1 im CTC-Mode** die Entprellzeit. Die gesamte Logik steckt in zwei ISRs – die Hauptschleife bleibt vollständig frei.

Ein **Zustandsautomat** mit 4 Zuständen steuert den Ablauf:

```
                 INT0 (fallend)
  ┌──────────────────────────────────────────────────────────┐
  │                                                          │
  ▼                                                          │
IDLE ──[INT0↓]──► DEBOUNCE_PRESS ──[Timer 20 ms]──► pin LOW?
                                                      │       │
                                                   ja │    nein│ (Fehlauslösung)
                                                      ▼       ▼
                                                  PRESSED   IDLE
                                                      │
                                             [INT0↑]──┘
                                                      │
                                                      ▼
                                              DEBOUNCE_RELEASE ──[Timer 20 ms]──► pin HIGH?
                                                                                    │       │
                                                                                 ja │    nein│
                                                                                    ▼       ▼
                                                                                  IDLE  PRESSED
```

**Zeitberechnung (20 ms Entprellzeit):**
```
f_CPU = 16 MHz, Prescaler 1024  →  f_Timer = 15.625 kHz
Ticks = 15625 × 0,020 = 312     →  OCR1A = 312 − 1 = 311
Tatsächliche Zeit: 312 / 15625 = 19,97 ms  (Fehler < 0,2 %)
```

### Quellcode

```c
#include <Arduino.h>
#include <avr/interrupt.h>

// INT0 = PD2 (Arduino Pin 2) -> Taster (mit Pull-up, low-aktiv)
// LED  = PB5 (Arduino Pin 13, onboard LED)

#define DEBOUNCE_OCR  311    // OCR1A fuer ~20 ms (Prescaler 1024, 16 MHz)

typedef enum { IDLE, DEBOUNCE_PRESS, PRESSED, DEBOUNCE_RELEASE } State;
static volatile State state = IDLE;

static void timer1_start(void)
{
  TCNT1  = 0;
  OCR1A  = DEBOUNCE_OCR;
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC, Prescaler 1024
  TIFR1  |= (1 << OCF1A);                             // pending Flag loeschen
  TIMSK1 |= (1 << OCIE1A);                            // Compare-Match freigeben
}

static void timer1_stop(void)
{
  TCCR1B  = 0;
  TIMSK1 &= ~(1 << OCIE1A);
}

static void int0_enable_falling(void)
{
  EICRA |=  (1 << ISC01);
  EICRA &= ~(1 << ISC00);  // fallende Flanke
  EIFR  |=  (1 << INTF0);  // pending Flag loeschen
  EIMSK |=  (1 << INT0);
}

static void int0_enable_rising(void)
{
  EICRA |= (1 << ISC01) | (1 << ISC00);  // steigende Flanke
  EIFR  |= (1 << INTF0);
  EIMSK |= (1 << INT0);
}

// INT0: Flanke erkannt -> INT0 sperren, Entprell-Timer starten
ISR(INT0_vect)
{
  EIMSK &= ~(1 << INT0);  // INT0 deaktivieren

  if (state == IDLE)
    state = DEBOUNCE_PRESS;
  else if (state == PRESSED)
    state = DEBOUNCE_RELEASE;

  timer1_start();
}

// Timer1 Compare-Match: 20 ms abgelaufen -> Pin-Zustand pruefen
ISR(TIMER1_COMPA_vect)
{
  timer1_stop();

  if (state == DEBOUNCE_PRESS)
  {
    if (!(PIND & (1 << PD2)))    // Taster noch gedrueckt (LOW)?
    {
      PORTB ^= (1 << PB5);       // LED toggeln
      state = PRESSED;
      int0_enable_rising();      // auf Loslassen warten
    }
    else                         // Fehlausloesung: kein echter Druck
    {
      state = IDLE;
      int0_enable_falling();
    }
  }
  else if (state == DEBOUNCE_RELEASE)
  {
    if (PIND & (1 << PD2))       // Taster stabil losgelassen (HIGH)?
    {
      state = IDLE;
      int0_enable_falling();
    }
    else                         // noch gedrueckt: weiter warten
    {
      state = PRESSED;
      int0_enable_rising();
    }
  }
}

int main(void)
{
  DDRB  |= (1 << PB5);    // PB5 als Ausgang (LED)
  DDRD  &= ~(1 << PD2);   // PD2 als Eingang
  PORTD |=  (1 << PD2);   // interner Pull-up

  int0_enable_falling();
  sei();

  while (true)
  {
    // Hauptschleife vollstaendig frei - gesamte Logik in ISRs
  }
}
```

### Fragen zu Übung 9

> **Hinweis:** Die Antworten sind im Abgabe-Dokument schriftlich festzuhalten.

1. **Zeitberechnung:** Berechne `DEBOUNCE_OCR` selbst: Welcher Wert ergibt sich für eine Entprellzeit von 20 ms bei 16 MHz und Prescaler 1024? Zeige jeden Rechenschritt.

2. **Zustandsautomat:** Beschreibe den Ablauf in eigenen Worten: Was passiert vom ersten Tastendruck bis zum Loslassen? Gehe dabei auf alle 4 Zustände ein.

3. **INT0 deaktivieren:** In `INT0_vect` wird sofort `EIMSK &= ~(1 << INT0)` ausgeführt. Warum ist das notwendig? Was würde ohne diese Zeile passieren?

4. **Vergleich der Methoden:** Fülle die folgende Tabelle aus:

   | Kriterium | Übung 8 (`_delay_ms`) | Übung 9 (Timer1, nicht-blockierend) |
   |-----------|-----------------------|-------------------------------------|
   | Entprellzeit gesamt | | |
   | CPU blockiert während Entprellung? | | |
   | Eignet sich für parallele Aufgaben? | | |
   | Code-Komplexität | | |

5. **Fehlauslösung:** In `TIMER1_COMPA_vect` gibt es den `else`-Zweig bei `DEBOUNCE_PRESS`. Wann tritt dieser Fall auf, und was macht das Programm dann?

6. **EIFR-Flag:** In `int0_enable_falling()` und `int0_enable_rising()` wird `EIFR |= (1 << INTF0)` geschrieben. Was bewirkt das, und warum ist es vor dem Aktivieren von INT0 wichtig?

---

## Abgabe

Erstelle ein Dokument (PDF oder Markdown) mit folgendem Inhalt:

### Pflichtbestandteile

- [ ] **Kapitel „Theorie":** Erkläre in eigenen Worten (3–5 Sätze), was Prellen ist und warum es ein Problem darstellt.
- [ ] **Übung 8 – Antworten:** Alle 6 Fragen beantwortet (inkl. Flussdiagramm als Foto/Scan oder digital).
- [ ] **Übung 9 – Antworten:** Alle 6 Fragen beantwortet (inkl. ausgefüllter Vergleichstabelle).
- [ ] **Reflexionsfrage:** Welche der beiden Methoden würdest du in einem eigenen Projekt verwenden, und warum?

### Abgabeformat

Dateiname: `Nachname_Vorname_Entprellung.pdf`  
Abgabe: Moodle → Kurs Embedded Systems → Aufgabe „Entprellung"

> **Abgabefrist:** nächste Unterrichtsstunde

---

## Anhang: Registerübersicht

### EICRA – External Interrupt Control Register A

| Bit | Name  | Funktion |
|-----|-------|----------|
| 3   | ISC11 | INT1 Flankenkonfiguration |
| 2   | ISC10 | INT1 Flankenkonfiguration |
| 1   | ISC01 | INT0 Flankenkonfiguration |
| 0   | ISC00 | INT0 Flankenkonfiguration |

Konfiguration INT0:

| ISC01 | ISC00 | Auslösung |
|-------|-------|-----------|
| 0     | 0     | Low-Level |
| 0     | 1     | Jede Flanke |
| 1     | 0     | Fallende Flanke |
| 1     | 1     | Steigende Flanke |

### EIMSK – External Interrupt Mask Register

| Bit | Name | Funktion |
|-----|------|----------|
| 1   | INT1 | Aktiviert INT1 |
| 0   | INT0 | Aktiviert INT0 |

```c
EIMSK |= (1 << INT0);   // INT0 aktivieren
```

### PIND – Port D Input Pins Register

Lesen des aktuellen Pegels an PD2:
```c
if (!(PIND & (1 << PD2)))   // LOW → Taster gedrückt
if  (PIND & (1 << PD2))     // HIGH → Taster losgelassen
```
