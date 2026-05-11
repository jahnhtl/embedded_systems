# Lösung: Entprellung von Tastern

---

## Theorie: Was ist Prellen?

Mechanische Taster bestehen aus Metallkontakten, die beim Betätigen kurzzeitig unkontrolliert hin- und herspringen – dieses Verhalten nennt man Prellen. Es tritt typischerweise für 5 bis 20 ms auf, sowohl beim Drücken als auch beim Loslassen. Ein Mikrocontroller, der im Mikrosekundentakt arbeitet, interpretiert dieses Signalrauschen als viele schnelle Tastendrücke hintereinander. Das führt dazu, dass ein einzelner physischer Tastendruck mehrere ungewollte Interrupts oder Zustandswechsel auslöst. Ohne Entprellung ist eine zuverlässige Erkennung von Tastendrücken daher nicht möglich.

---

## Übung 8 – Entprellung mit `_delay_ms`

**Quelldatei:** `SourceCodes/Examples/8_externer_interrupt_int0_entprellung_delay.cpp`

### Frage 1 – Erklärung `volatile`

`taster_flag` muss als `volatile` deklariert werden, weil es innerhalb der ISR (`INT0_vect`) gesetzt wird und in der `main`-Schleife gelesen wird – also von zwei verschiedenen Ausführungskontexten aus zugegriffen wird.

Ohne `volatile` darf der Compiler annehmen, dass die Variable sich im normalen Programmfluss nicht ändert. Er könnte den Wert daher in ein CPU-Register cachen und nie erneut aus dem RAM lesen. Die Hauptschleife würde `taster_flag` dann dauerhaft als `0` sehen, obwohl die ISR es auf `1` gesetzt hat – der Tastendruck würde nie erkannt.

`volatile` zwingt den Compiler, die Variable bei jedem Zugriff direkt aus dem RAM zu lesen.

---

### Frage 2 – Fallende Flanke: EICRA-Bits

Laut ATmega328P-Datenblatt steuern **ISC01** und **ISC00** die Flankenerkennung von INT0:

| ISC01 | ISC00 | Bedeutung         |
|-------|-------|-------------------|
|   0   |   0   | Low-Level-Interrupt |
|   0   |   1   | Pegelwechsel      |
|   1   |   0   | **Fallende Flanke** |
|   1   |   1   | Steigende Flanke  |

Für eine fallende Flanke werden folgende zwei Zeilen benötigt:

```c
EICRA |= (1 << ISC01);   // ISC01 = 1
EICRA &= ~(1 << ISC00);  // ISC00 = 0
```

- `ISC01 = 1`: notwendige Bedingung für eine flankengetriggerte Erkennung.
- `ISC00 = 0`: wählt die **fallende** Flanke (HIGH → LOW), da der Taster low-aktiv ist.

---

### Frage 3 – Prellzeit `ENTPRELL_MS`

`ENTPRELL_MS` hat den Wert **20 ms** (siehe Zeile 12 im Quellcode).

Dieser Wert wurde gewählt, weil mechanische Taster typischerweise 5–20 ms prellen. 20 ms deckt den üblichen Worst Case ab:

- **1 ms** wäre zu kurz – Preller dauern länger, es käme zu Fehlerkennung.
- **100 ms** wäre zu lang – schnelle, bewusste Tastendrücke würden ignoriert, da das Programm noch auf das Abklingen des vorherigen Drucks wartet.

20 ms ist ein bewährter Kompromiss zwischen Zuverlässigkeit und Reaktionszeit.

---

### Frage 4 – Bestätigungscheck nach `_delay_ms`

Nach der Wartezeit prüft das Programm erneut, ob `PD2` noch LOW ist:

```c
if (!(PIND & (1 << PD2)))  // Taster noch gedrückt?
```

Diese Prüfung ist notwendig, um echte Tastendrücke von Prellern zu unterscheiden. Ein Preller kann ebenfalls eine fallende Flanke und damit einen Interrupt auslösen, auch wenn danach kein dauerhafter Kontakt besteht. Nach 20 ms Wartezeit ist der Taster bei einem echten Druck noch LOW, bei einem Preller bereits wieder HIGH.

Ohne diese Prüfung würde jeder Preller als gültiger Tastendruck gewertet – die LED würde unkontrolliert toggeln.

---

### Frage 5 – Nachteil der blockierenden `while`-Schleife

```c
while (!(PIND & (1 << PD2)));  // warten bis Taster losgelassen
```

Diese Schleife blockiert die CPU vollständig, bis der Taster losgelassen wird. In einer realen Anwendung bedeutet das:

**Nachteil:** Alle anderen Aufgaben – z. B. Sensorauslesen, Kommunikation über UART, Aktualisieren von Ausgaben – werden so lange verzögert oder verpasst. Hält der Nutzer den Taster z. B. 2 Sekunden gedrückt, ist der Mikrocontroller in dieser Zeit für nichts anderes verfügbar. Das macht das System unresponsiv und ist in Mehraufgaben-Anwendungen nicht akzeptabel.

---

## Übung 9 – Saubere Entprellung per Timer1 (nicht-blockierend)

**Quelldatei:** `SourceCodes/Examples/9_externer_interrupt_int0_entprellung_sauber.cpp`

### Frage 1 – Zeitberechnung `DEBOUNCE_OCR`

Gegeben: f_CPU = 16 MHz, Prescaler = 1024, Zielzeit = 20 ms

**Schritt 1:** Timer-Frequenz berechnen:

```
f_Timer = f_CPU / Prescaler = 16.000.000 / 1024 = 15.625 Hz
```

**Schritt 2:** Anzahl der Ticks für 20 ms:

```
Ticks = f_Timer × t = 15.625 × 0,020 s = 312,5 → 312 Ticks
```

**Schritt 3:** OCR1A-Wert (CTC zählt von 0 bis OCR1A, also 312 Schritte → OCR1A = 311):

```
OCR1A = Ticks - 1 = 312 - 1 = 311
```

**Kontrolle:** Tatsächliche Zeit = 312 / 15.625 = **19,968 ms** (Fehler < 0,2 %) ✓

---

### Frage 2 – Zustandsautomat: Ablauf vom Tastendruck bis zum Loslassen

Der Automat hat vier Zustände:

**1. IDLE**
Der Normalzustand. INT0 ist aktiv und wartet auf eine **fallende Flanke** (Taster wird gedrückt). Die Hauptschleife tut nichts.

**2. DEBOUNCE_PRESS**
INT0 hat eine fallende Flanke erkannt. INT0 wird sofort deaktiviert, um Preller zu ignorieren. Timer1 wird gestartet. Nach 20 ms feuert der Timer-Interrupt.
- Ist `PD2` noch LOW → echter Druck: LED toggeln, Zustand → PRESSED
- Ist `PD2` schon wieder HIGH → Preller: Zustand → IDLE, INT0 wieder aktivieren

**3. PRESSED**
Der Tastendruck wurde bestätigt. INT0 wird nun für eine **steigende Flanke** aktiviert (Taster wird losgelassen). Gewartet wird auf das Loslassen.

**4. DEBOUNCE_RELEASE**
INT0 hat eine steigende Flanke erkannt. INT0 wird wieder deaktiviert. Timer1 startet erneut. Nach 20 ms:
- Ist `PD2` HIGH → Taster stabil losgelassen: Zustand → IDLE, INT0 wieder für fallende Flanke aktivieren
- Ist `PD2` noch LOW → Preller beim Loslassen: Zustand → PRESSED, INT0 weiter auf steigende Flanke warten

**Flussdiagramm:**

```
        ┌──────────────────────────────────────┐
        │                                      │
        ▼                                      │
┌─────────────┐   fallende Flanke   ┌──────────────────┐
│    IDLE     │────────────────────▶│  DEBOUNCE_PRESS  │
└─────────────┘                     └──────────────────┘
        ▲                                    │
        │                          Timer 20ms abgelaufen
        │                                    │
        │                   ┌────────────────┴────────────────┐
        │              PD2=LOW (echter Druck)            PD2=HIGH (Preller)
        │                   │                                  │
        │                   ▼                                  │
        │          ┌────────────────┐                          │
        │          │    PRESSED     │                          │
        │          └────────────────┘                          │
        │                   │                                  │
        │          steigende Flanke                            │
        │                   │                                  │
        │                   ▼                                  │
        │          ┌──────────────────┐                        │
        │          │ DEBOUNCE_RELEASE │                        │
        │          └──────────────────┘                        │
        │                   │                                  │
        │         Timer 20ms abgelaufen                        │
        │                   │                                  │
        │    ┌──────────────┴──────────────┐                   │
        │ PD2=HIGH (losgelassen)      PD2=LOW (noch gedrückt)  │
        │    │                             │                   │
        └────┘                        (→ PRESSED)              │
                                                               │
        ◀──────────────────────────────────────────────────────┘
```

---

### Frage 3 – INT0 deaktivieren in `INT0_vect`

```c
EIMSK &= ~(1 << INT0);  // INT0 deaktivieren
```

Diese Zeile ist notwendig, um **Preller während der Entprellzeit zu ignorieren**. Direkt nach dem ersten Flankenereignis beginnt die Kontaktfläche zu prellen und erzeugt weitere schnelle Flanken. Wäre INT0 weiterhin aktiv, würde jede Prellerflanke erneut die ISR auslösen, den Timer neu starten und den Zustandsautomaten stören.

Durch das sofortige Deaktivieren von INT0 werden alle weiteren Flanken während der 20 ms Entprellzeit vollständig ignoriert. Erst nach Ablauf des Timers wird INT0 wieder gezielt aktiviert.

---

### Frage 4 – Vergleich der Methoden

| Kriterium | Übung 8 (`_delay_ms`) | Übung 9 (Timer1, nicht-blockierend) |
|-----------|-----------------------|-------------------------------------|
| Entprellzeit gesamt | 2 × 20 ms (Press + Release) | 2 × 20 ms (Press + Release) |
| CPU blockiert während Entprellung? | **Ja** (`_delay_ms` + `while`-Schleife) | **Nein** (Timer-ISR übernimmt) |
| Eignet sich für parallele Aufgaben? | **Nein** – Hauptschleife blockiert | **Ja** – Hauptschleife vollständig frei |
| Code-Komplexität | **Gering** – einfach zu verstehen | **Hoch** – Zustandsautomat, mehrere ISRs |

---

### Frage 5 – `else`-Zweig bei `DEBOUNCE_PRESS`

```c
else  // Fehlausloesung (Preller, kein echter Druck)
{
    state = IDLE;
    int0_enable_falling();
}
```

Dieser Fall tritt auf, wenn nach Ablauf der 20 ms Entprellzeit `PD2` **nicht mehr LOW ist** – der Taster also bereits wieder HIGH ist. Das bedeutet, dass kein echter Tastendruck stattgefunden hat, sondern lediglich ein kurzer Preller eine fallende Flanke erzeugt hat.

Das Programm wertet dies als Fehlauslösung und setzt den Zustand zurück auf **IDLE**. Anschließend wird INT0 wieder für die nächste fallende Flanke aktiviert, sodass das System bereit ist für einen echten Tastendruck. Die LED wird **nicht** getogglelt.

---

### Frage 6 – EIFR-Flag löschen

```c
EIFR |= (1 << INTF0);  // pending Flag löschen
```

**EIFR** (External Interrupt Flag Register) enthält das Flag `INTF0`, das gesetzt wird, sobald auf INT0 die konfigurierte Flanke erkannt wurde – **unabhängig davon, ob INT0 aktiviert ist oder nicht**.

Beim AVR löscht man ein solches Flag, indem man eine **1** hineinschreibt (nicht 0).

**Warum ist das wichtig?** Während INT0 deaktiviert war (Entprellzeit), können Preller weitere Flanken erzeugt haben, die INTF0 gesetzt haben. Würde man INT0 direkt wieder aktivieren ohne das Flag zu löschen, würde sofort ein Interrupt ausgelöst – obwohl gar keine neue Flanke aufgetreten ist. Durch das Löschen des Flags vor der Aktivierung wird sichergestellt, dass nur **neue** Flanken nach der Aktivierung berücksichtigt werden.

---

## Reflexionsfrage

**Welche Methode würdest du in einem eigenen Projekt verwenden?**

Für ein reales Projekt würde ich **Übung 9 (Timer1, nicht-blockierend)** bevorzugen. Sobald ein Projekt mehr als eine einzige Aufgabe hat – z. B. gleichzeitig eine serielle Schnittstelle bedienen, Sensoren auslesen oder mehrere LEDs steuern –, ist eine blockierende Implementierung mit `_delay_ms` inakzeptabel. Die Timer-basierte Lösung lässt die Hauptschleife vollständig frei und skaliert problemlos auf komplexere Systeme.

**Übung 8** ist hingegen für einfache Lernprojekte oder Prototypen sinnvoll, da der Code deutlich leichter zu verstehen und zu debuggen ist. Als Einstieg in das Thema Entprellung ist sie gut geeignet, sollte aber in produktiven Projekten durch eine nicht-blockierende Variante ersetzt werden.
