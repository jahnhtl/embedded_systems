# Flussdiagramm Basics - Grundlagen

Flussdiagramme (auch Programmablaufpläne oder PAP genannt) sind eine grafische Darstellung von Programmabläufen. Sie helfen dabei, die Logik eines Programms zu verstehen und zu planen, bevor der eigentliche Code geschrieben wird.

## Inhaltsverzeichnis
- [Grundlegende Elemente](#grundlegende-elemente)
- [EVA-Prinzip (Eingabe-Verarbeitung-Ausgabe)](#eva-prinzip-eingabe-verarbeitung-ausgabe)
- [Verzweigungen (If-Then-Else)](#verzweigungen-if-then-else)
- [Schleifen](#schleifen)
- [Beispiele zum Üben](#beispiele-zum-üben)

---

## Grundlegende Elemente

Für die meisten Programme benötigst du nur vier verschiedene Grundelemente:

### 🟢 Start/Ende (Oval)
```mermaid
flowchart TD
    A((Start)) --> B[...] --> C((Ende))
```
- **Start:** Zeigt den Beginn des Programms
- **Ende:** Zeigt das Ende des Programms
- **Form:** Oval oder abgerundetes Rechteck

### 📦 Aktion/Prozess (Rechteck)
```mermaid
flowchart TD
    A[Berechne Summe = a + b]
    B[Zähler = Zähler + 1]
    C[LED einschalten]
```
- **Verwendung:** Berechnungen, Zuweisungen, Aktionen
- **Form:** Rechteck
- **Beispiele:** 
  - `Zähler = 0`
  - `Summe = a + b`
  - `LED einschalten`

**Hinweis:** Eine Aktion oder ein Prozess hat immer ein Verb als Beschreibung mit dabei. z.B. Timer *starten*

### 📋 Ein-/Ausgabe (Rechteck oder Parallelogramm)
```mermaid
flowchart TD
    A[Eingabe: Zahl a]
    B[Ausgabe: Ergebnis]
    C[Ausgabe: 'Hallo Welt']
```
- **Verwendung:** Daten einlesen oder ausgeben
- **Form:** Rechteck (vereinfacht)
- **Beispiele:**
  - `Eingabe: Name`
  - `Ausgabe: Ergebnis`
  - `Ausgabe: 'Fehler!'`

### 💎 Verzweigung/Entscheidung (Raute)
```mermaid
flowchart TD
    A{Zahl > 0?} -->|Ja| B[Positive Zahl]
    A -->|Nein| C[Negative oder Null]
```
- **Verwendung:** Ja/Nein-Entscheidungen, Bedingungen prüfen
- **Form:** Raute (Diamant)
- **Ausgänge:** Meist "Ja" und "Nein" oder "Wahr" und "Falsch"

---

## EVA-Prinzip (Eingabe-Verarbeitung-Ausgabe)

Das **EVA-Prinzip** ist das Grundmuster der meisten Programme:
1. **E**ingabe: Daten werden eingelesen
2. **V**erarbeitung: Daten werden bearbeitet/berechnet
3. **A**usgabe: Ergebnis wird ausgegeben

### Beispiel 1: Einfache Addition
```mermaid
flowchart TD
    A((Start)) --> B[Eingabe: Zahl a]
    B --> C[Eingabe: Zahl b]
    C --> D[Summe = a + b]
    D --> E[Ausgabe: Summe]
    E --> F((Ende))
```

**Erklärung:**
- **Eingabe:** Zwei Zahlen werden eingelesen
- **Verarbeitung:** Die Zahlen werden addiert
- **Ausgabe:** Das Ergebnis wird angezeigt

### Beispiel 2: Kreisumfang berechnen
```mermaid
flowchart TD
    A((Start)) --> B[Eingabe: Radius]
    B --> C[Umfang = 2 × π × Radius]
    C --> D[Ausgabe: Umfang]
    D --> E((Ende))
```

**Erklärung:**
- **E:** Radius eingeben
- **V:** Umfang mit Formel berechnen
- **A:** Ergebnis ausgeben

---

## Verzweigungen (If-Then-Else)

Verzweigungen ermöglichen es, verschiedene Programmwege je nach Bedingung zu durchlaufen.

### Beispiel 1: Einfache Verzweigung
**Aufgabe:** Prüfe, ob eine Zahl positiv oder negativ ist.

```mermaid
flowchart TD
    A((Start)) --> B[Eingabe: Zahl]
    B --> C{Zahl > 0?}
    C -->|Ja| D[Ausgabe: 'Positive Zahl']
    C -->|Nein| E[Ausgabe: 'Negative Zahl oder Null']
    D --> F((Ende))
    E --> F
```

### Beispiel 2: Mehrfache Verzweigung
**Aufgabe:** Notenbewertung (1-5)

```mermaid
flowchart TD
    A((Start)) --> B[Eingabe: Punkte]
    B --> C{Punkte >= 90?}
    C -->|Ja| D[Note = 1]
    C -->|Nein| E{Punkte >= 80?}
    E -->|Ja| F[Note = 2]
    E -->|Nein| G{Punkte >= 70?}
    G -->|Ja| H[Note = 3]
    G -->|Nein| I{Punkte >= 60?}
    I -->|Ja| J[Note = 4]
    I -->|Nein| K[Note = 5]
    D --> L[Ausgabe: Note]
    F --> L
    H --> L
    J --> L
    K --> L
    L --> M((Ende))
```

### Beispiel 3: Ampel-Steuerung
**Aufgabe:** Ampelfarbe bestimmen

```mermaid
flowchart TD
    A((Start)) --> B[Eingabe: Farbe]
    B --> C{Farbe = 'Rot'?}
    C -->|Ja| D[Ausgabe: 'Stopp']
    C -->|Nein| E{Farbe = 'Gelb'?}
    E -->|Ja| F[Ausgabe: 'Achtung']
    E -->|Nein| G{Farbe = 'Grün'?}
    G -->|Ja| H[Ausgabe: 'Fahren']
    G -->|Nein| I[Ausgabe: 'Ungültige Farbe']
    D --> J((Ende))
    F --> J
    H --> J
    I --> J
```

---

## Schleifen

Schleifen ermöglichen es, bestimmte Programmteile mehrfach auszuführen.

### Beispiel 1: Zählschleife (von 1 bis 5)
**Aufgabe:** Gib die Zahlen von 1 bis 5 aus.

```mermaid
flowchart TD
    A((Start)) --> B[Zähler = 1]
    B --> C{Zähler <= 5?}
    C -->|Ja| D[Ausgabe: Zähler]
    D --> E[Zähler = Zähler + 1]
    E --> C
    C -->|Nein| F((Ende))
```

**Ablauf:**
1. Zähler startet bei 1
2. Prüfe: Ist Zähler ≤ 5?
3. Wenn ja: Zähler ausgeben, um 1 erhöhen, zurück zu Schritt 2
4. Wenn nein: Schleife beenden

### Beispiel 2: Benutzer-gesteuerte Schleife
**Aufgabe:** Zahlen eingeben, bis 0 eingegeben wird.

```mermaid
flowchart TD
    A((Start)) --> B[Eingabe: Zahl]
    B --> C{Zahl ≠ 0?}
    C -->|Ja| D[Ausgabe: 'Zahl verarbeitet']
    D --> B
    C -->|Nein| E[Ausgabe: 'Programm beendet']
    E --> F((Ende))
```

### Beispiel 3: Summe berechnen (1+2+3+...+n)
**Aufgabe:** Berechne die Summe aller Zahlen von 1 bis n.

```mermaid
flowchart TD
    A((Start)) --> B[Eingabe: n]
    B --> C[Summe = 0]
    C --> D[Zähler = 1]
    D --> E{Zähler <= n?}
    E -->|Ja| F[Summe = Summe + Zähler]
    F --> G[Zähler = Zähler + 1]
    G --> E
    E -->|Nein| H[Ausgabe: Summe]
    H --> I((Ende))
```

### Beispiel 4: LED-Blinken (Arduino)
**Aufgabe:** LED 10 mal blinken lassen.

```mermaid
flowchart TD
    A((Start)) --> B[Zähler = 0]
    B --> C{Zähler < 10?}
    C -->|Ja| D[LED einschalten]
    D --> E[Warte 1 Sekunde]
    E --> F[LED ausschalten]
    F --> G[Warte 1 Sekunde]
    G --> H[Zähler = Zähler + 1]
    H --> C
    C -->|Nein| I((Ende))
```

---

## Beispiele zum Üben

### Übung 1: Gerade oder ungerade?
**Aufgabe:** Prüfe, ob eine eingegebene Zahl gerade oder ungerade ist.
**Hinweis:** Eine Zahl ist gerade, wenn sie durch 2 teilbar ist (Rest = 0).

<details>
<summary>Lösung anzeigen</summary>

```mermaid
flowchart TD
    A((Start)) --> B[Eingabe: Zahl]
    B --> C{Zahl % 2 = 0?}
    C -->|Ja| D[Ausgabe: 'Gerade']
    C -->|Nein| E[Ausgabe: 'Ungerade']
    D --> F((Ende))
    E --> F
```
</details>

### Übung 2: Maximum von drei Zahlen
**Aufgabe:** Finde die größte von drei eingegebenen Zahlen.

<details>
<summary>Lösung anzeigen</summary>

```mermaid
flowchart TD
    A((Start)) --> B[Eingabe: a, b, c]
    B --> C{a >= b?}
    C -->|Ja| D{a >= c?}
    C -->|Nein| E{b >= c?}
    D -->|Ja| F[Maximum = a]
    D -->|Nein| G[Maximum = c]
    E -->|Ja| H[Maximum = b]
    E -->|Nein| I[Maximum = c]
    F --> J[Ausgabe: Maximum]
    G --> J
    H --> J
    I --> J
    J --> K((Ende))
```
</details>

### Übung 3: Fakultät berechnen
**Aufgabe:** Berechne n! = 1 × 2 × 3 × ... × n

<details>
<summary>Lösung anzeigen</summary>

```mermaid
flowchart TD
    A((Start)) --> B[Eingabe: n]
    B --> C[Fakultät = 1]
    C --> D[Zähler = 1]
    D --> E{Zähler <= n?}
    E -->|Ja| F[Fakultät = Fakultät × Zähler]
    F --> G[Zähler = Zähler + 1]
    G --> E
    E -->|Nein| H[Ausgabe: Fakultät]
    H --> I((Ende))
```
</details>

---

## 🎯 Wichtige Tipps

### Beim Erstellen von Flussdiagrammen:
1. **Beginne immer mit "Start"** und ende mit "Ende"
2. **Verwende klare, eindeutige Beschriftungen** in den Elementen
3. **Pfeile zeigen die Richtung** des Programmflusses
4. **Jede Raute braucht mindestens zwei Ausgänge** (Ja/Nein)
5. **Vermeide sich kreuzende Linien** - das macht das Diagramm unübersichtlich

### Häufige Fehler vermeiden:
- ❌ Vergessene Start/Ende-Elemente
- ❌ Rauten ohne klare Ja/Nein-Ausgänge
- ❌ Endlosschleifen (kein Ausstieg aus der Schleife)
- ❌ Unklare Beschriftungen wie "Mache etwas"

### Testmethode:
**"Trockenlauf"** - Gehe das Flussdiagramm mit Beispieldaten durch:
1. Wähle konkrete Testwerte
2. Folge Schritt für Schritt dem Diagramm
3. Notiere alle Zwischenergebnisse
4. Prüfe, ob das erwartete Ergebnis herauskommt

---

## 🚀 Nächste Schritte

Wenn du diese Grundlagen verstanden hast, kannst du mit den [ersten Übungen](erste_uebungen.md) beginnen!

**Übungsreihenfolge:**
1. **EVA-Programme** (einfache Ein-/Ausgabe mit Berechnung)
2. **Einfache Verzweigungen** (if-then-else)
3. **Schleifen** (for/while)
4. **Kombinierte Programme** (Verzweigungen + Schleifen)

Viel Erfolg beim Programmieren! 💻