# Übungsbeispiel 2a: Countdown mit Lauflicht

## Aufgabenstellung

Programmiere einen Arduino-Sketch, der einen visuellen Countdown mit einem Lauflicht über drei LEDs darstellt.

### Anforderungen:
- Drei LEDs sind an Pin 11, Pin 12 und Pin 13 angeschlossen
- Der Countdown läuft von 3 bis 0:
  - **3**: LED an Pin 13 leuchtet (1000ms)
  - **2**: LED an Pin 12 leuchtet (1000ms)
  - **1**: LED an Pin 11 leuchtet (1000ms)
  - **0**: Alle LEDs blinken 3x schnell (200ms an/aus)
- Danach beginnt der Countdown wieder von vorne
- Verwende `millis()` für die Zeitsteuerung (kein `delay()` außer beim Blinken)

### Hinweise:
- Nutze `millis()` um die Zeit zu messen
- Eine Variable speichert die aktuelle Countdown-Position (0-3)
- Beim Blinken darf kurz `delay()` verwendet werden

---

## Schritt 1: Flussdiagramm

Erstelle ein Flussdiagramm, das den Ablauf des Programms zeigt:


---

## Schritt 2: Code-Implementierung

Schreibe den Arduino-Code, der die Anforderungen erfüllt.

Achte auf:
- Verwendung von `millis()` für das Lauflicht
- Korrekte Zustandsverwaltung des Countdowns
- Saubere Code-Struktur mit `setup()` und `loop()`
