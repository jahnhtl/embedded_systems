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

### Fragen zu Übung 9

> **Hinweis:** Die Antworten sind im Abgabe-Dokument schriftlich festzuhalten.

1. **Zeitberechnung:** Berechne `DEBOUNCE_OCR` selbst: Welcher Wert ergibt sich für eine Entprellzeit von 20 ms bei 16 MHz und Prescaler 1024? Zeige jeden Rechenschritt.

2. **Zustandsautomat:** Beschreibe den Ablauf in eigenen Worten: Was passiert vom ersten Tastendruck bis zum Loslassen? Gehe dabei auf alle 4 Zustände ein. Zeichne ein eigenes Flussdiagramm und füge dieses in die Abgabe mit ein.

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

