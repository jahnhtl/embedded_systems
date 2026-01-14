# Übungsbeispiel 3b: Zwei unabhängig blinkende LEDs

## Aufgabenstellung

Programmiere einen Arduino-Sketch, der zwei LEDs mit unterschiedlichen, unabhängigen Blinkfrequenzen steuert.

### Anforderungen:
- Zwei LEDs sind an Pin 12 und Pin 13 angeschlossen
- Beide LEDs blinken unabhängig voneinander
- LED 1 (Pin 12): Blinkt mit 300ms Intervall
- LED 2 (Pin 13): Blinkt mit 700ms Intervall
- Verwende `millis()` für die Zeitsteuerung (kein `delay()`)
- Bei jedem Zustandswechsel soll der Status der jeweiligen LED auf dem Serial Monitor ausgegeben werden
- Baudrate: 115200

### Hinweise:
- Jede LED benötigt eigene Zeitvariablen
- Keine Arrays, keine oldState-Variable
- Die LEDs sollen wirklich unabhängig voneinander blinken

---

## Schritt 1: Flussdiagramm

Erstelle ein Flussdiagramm, das den Ablauf des Programms zeigt:


---

## Schritt 2: Code-Implementierung

Schreibe den Arduino-Code, der die Anforderungen erfüllt.

Achte auf:
- Separate Zeitvariablen für jede LED
- Verwendung von `millis()` statt `delay()`
- Saubere Code-Struktur mit `setup()` und `loop()`
