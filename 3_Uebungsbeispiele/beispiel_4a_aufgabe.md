# Übungsbeispiel 2a: LED-Lauflicht mit Geschwindigkeits-Taster

## Aufgabenstellung

Programmiere einen Arduino-Sketch, der ein Lauflicht über drei LEDs erzeugt. Die Richtung ist fest (vorwärts), die Geschwindigkeit wird über einen Taster umgeschaltet.

### Anforderungen:
- Drei LEDs sind an Pin 11, Pin 12 und Pin 13 angeschlossen
- Ein Taster ist an Pin 2 angeschlossen (mit internem Pull-Up)
- Die LEDs schalten nacheinander durch (LED1 → LED2 → LED3 → LED1 → ...)
- Der Taster schaltet zwischen zwei Geschwindigkeiten um:
  - Schnell: 200ms pro Schritt
  - Langsam: 800ms pro Schritt
- Bei jeder Umschaltung soll der aktuelle Zustand auf dem Serial Monitor ausgegeben werden
- Baudrate: 115200

### Hinweise:
- Taster ist mit Pull-Up konfiguriert (gedrückt = LOW)
- Einfache Entprellung mit `delay(300)`

---

## Schritt 1: Flussdiagramm

Erstelle ein Flussdiagramm, das den Ablauf des Programms zeigt:

---

## Schritt 2: Code-Implementierung

Schreibe den Arduino-Code, der die Anforderungen erfüllt.

Achte auf:
- Zustandsvariablen für Geschwindigkeit und aktuellen LED-Index
- Saubere Code-Struktur mit `setup()` und `loop()`
