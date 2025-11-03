# Flussdiagramme - Erste Übungen

Diese Aufgabensammlung hilft dir dabei, die Grundlagen der Programmlogik mit Flussdiagrammen zu verstehen und zu üben.

## Inhaltsverzeichnis
- [Einfache Aufgaben](#einfache-aufgaben)
  - [1.1 Quadratzahl berechnen](#11-quadratzahl-berechnen)
  - [1.2 Countdown](#12-countdown)
  - [1.3 Parkhausverwaltung](#13-parkhausverwaltung)
  - [1.4 Reaktionsspiel](#14-reaktionsspiel)
- [Schwere Aufgaben](#schwere-aufgaben)
  - [2.1 Passwortüberprüfung](#21-passwortüberprüfung)
  - [2.2 Ampel mit Fußgängerübergang](#22-ampel-mit-fußgängerübergang)
  - [2.3 Getränkeautomat](#23-getränkeautomat)

---

## Einfache Aufgaben

### 1.1 Quadratzahl berechnen

**Aufgabe:** Ein Programm liest eine Zahl ein, berechnet deren Quadrat und gibt das Ergebnis aus.

---

### 1.2 Countdown

**Aufgabe:** Ein Programm liest eine Zahl ein (z.B. 5). Von dieser Zahl wird rückwärts gezählt (5, 4, 3, 2, 1). Danach wird "Ready for Takeoff!" ausgegeben.

---

### 1.3 Parkhausverwaltung

**Aufgabe:** Ein Mikrocontroller soll ein Parkhaus mit 50 Stellplätzen überwachen.
- Bei jeder Einfahrt wird ein Platz abgezogen
- Bei jeder Ausfahrt wird ein Platz addiert
- Wenn das Parkhaus voll ist, erscheint „Besetzt"
- Wenn noch Plätze vorhanden sind, erscheint „Frei"

**Wichtige Elemente:**
- Endlosschleife (Parkhaus läuft kontinuierlich)
- Zählervariable für freie Plätze
- Verschiedene Ausgaben je nach Status

---

### 1.4 Reaktionsspiel

**Aufgabe:** Ein Arduino schaltet eine LED zufällig nach einigen Sekunden ein.
- Sobald die LED leuchtet, muss der Spieler so schnell wie möglich einen Taster drücken
- Das Programm misst die Reaktionszeit
- Wenn die Zeit < 0,5 Sekunden ist, erscheint „Sehr schnell"
- Wenn die Zeit zwischen 0,5 und 1,5 Sekunden liegt, erscheint „Normal"
- Sonst erscheint „Langsam"

**Besonderheiten:**
- Zeitmessung zwischen zwei Ereignissen
- Verzweigungen mit mehreren Bedingungen
- Zufallsgenerator für Wartezeit

---

## Schwere Aufgaben

### 2.1 Passwortüberprüfung

**Aufgabe:** Ein Programm soll prüfen, ob ein korrektes Passwort eingegeben wird.
- Der Benutzer hat maximal 3 Versuche
- Bei falscher Eingabe erscheint die Meldung „Falsch, nochmal versuchen!"
- Bei korrekter Eingabe wird „Zugang erlaubt" ausgegeben und das Programm endet sofort
- Nach 3 falschen Versuchen erscheint „Zugang verweigert"

**Komplexitätselemente:**
- Schleife mit Bedingung (max. 3 Versuche)
- Zählervariable
- Vorzeitiger Ausstieg bei Erfolg
- Verschachtelte Bedingungen

---

### 2.2 Ampel mit Fußgängerübergang

**Aufgabe:** Ein Arduino steuert eine Verkehrsampel (Rot – Gelb – Grün).
- Solange kein Fußgänger den Taster drückt, läuft die normale Ampelsteuerung (Rot → Rot-Gelb → Grün → Gelb → Rot)
- Wird der Taster gedrückt, soll die Ampel nach der aktuellen Grünphase auf Rot schalten und für 10 Sekunden Rot bleiben (damit Fußgänger die Straße überqueren können). Danach läuft die Ampelschaltung normal weiter

**Erweiterte Konzepte:**
- Zustandsmaschine (verschiedene Ampelphasen)
- Externe Interrupts (Taster)
- Zeitgesteuerte Abläufe
- Unterschiedliche Verzweigungen je nach Zustand

---

### 2.3 Getränkeautomat

**Aufgabe:** Wie bei einem echten Getränkeautomaten soll zunächst das Getränk gewählt werden. Danach soll der Preis angezeigt werden und geprüft werden ob genügend Geld eingeworfen wurde. Wenn der benötigte Betrag noch nicht erreicht wurde, erscheint die Meldung „Bitte mehr Geld einwerfen". Der Vorgang wiederholt sich, bis ein Getränk erfolgreich gekauft und ausgegeben wurde oder der Benutzer „Abbrechen" drückt.

**Komplexeste Aufgabe mit:**
- Mehreren Schleifen
- Geldrückgabe-Logik
- Benutzerinteraktion mit Abbruchmöglichkeit
- Verschiedene Getränkepreise
- Wechselgeld-Berechnung

---

## Tipps zum Lösen der Aufgaben

### Allgemeine Vorgehensweise:
1. **Problemverständnis:** Lies die Aufgabe mehrmals und verstehe alle Anforderungen
2. **Eingaben/Ausgaben identifizieren:** Was muss eingegeben/ausgegeben werden?
3. **Schritte auflisten:** Welche Schritte sind nötig?
4. **Bedingungen finden:** Wo müssen Entscheidungen getroffen werden?
5. **Schleifen erkennen:** Welche Vorgänge wiederholen sich?

### Flussdiagramm-Elemente:
- **Oval:** Start/Ende
- **Rechteck:** Prozess/Berechnung
- **Rechteck oder Parallelogramm:** Eingabe/Ausgabe
- **Raute:** Entscheidung (Ja/Nein)
- **Pfeile:** Programmfluss

### Häufige Muster:
- **Zählschleifen:** Für bestimmte Anzahl Wiederholungen
- **Bedingungsschleifen:** Solange eine Bedingung erfüllt ist
- **Verzweigungen:** If-Then-Else Strukturen
- **Zustandsmaschinen:** Verschiedene Modi/Zustände

---

## Übungstipps

1. **Beginne mit den einfachen Aufgaben** und arbeite dich vor
2. **Zeichne die Flussdiagramme von Hand** bevor du sie digital erstellst
3. **Verfolge den Programmablauf** mit Beispieldaten durch
4. **Teste Grenzfälle:** Was passiert bei unerwarteten Eingaben?
5. **Verwende sprechende Variablennamen** in deinen Diagrammen

Viel Erfolg beim Üben!