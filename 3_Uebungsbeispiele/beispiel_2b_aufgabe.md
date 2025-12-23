# Übungsbeispiel 2b: LED-Lauflicht mit Taster-Steuerung

## Aufgabenstellung

Programmiere einen Arduino-Sketch, der ein Lauflicht über drei LEDs erzeugt. Die Richtung (vorwärts/rückwärts) und die Geschwindigkeit sollen über Taster steuerbar sein.

### Anforderungen:
- Drei LEDs sind an Pin 11, Pin 12 und Pin 13 angeschlossen
- Zwei Taster sind an Pin 2 und Pin 3 angeschlossen (mit internem Pull-Up)
- Die LEDs sollen abwechselnd blinken (Lauflicht-Effekt)
- **Taster 1 (Pin 2)**: Richtung umkehren (Vorwärts: LED1→LED2→LED3, Rückwärts: LED3→LED2→LED1)
- **Taster 2 (Pin 3)**: Geschwindigkeit ändern (schnell: 200ms, langsam: 800ms)
- Bei jedem Tastendruck soll der aktuelle Zustand auf dem Serial Monitor ausgegeben werden
- Baudrate: 115200

### Hinweise:
- Taster sind mit Pull-Up konfiguriert (gedrückt = LOW)
- Einfache Entprellung mit delay()
- Geschwindigkeit zwischen zwei Modi wechseln

---

## Schritt 1: Flussdiagramm

Erstelle ein Flussdiagramm, das den Ablauf des Programms zeigt:

---

## Schritt 2: Code-Implementierung

Schreibe den Arduino-Code, der die Anforderungen erfüllt.

Achte auf:
- Zustandsvariablen für Richtung und Geschwindigkeit
- Logische Umschaltung der LEDs
- Klare Ausgabe der aktuellen Einstellungen
- Saubere Code-Struktur mit `setup()` und `loop()`
