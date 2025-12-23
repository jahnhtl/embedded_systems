# Übungsbeispiel 1b: LED-Steuerung mit Taster und serieller Eingabe

## Aufgabenstellung

Programmiere einen Arduino-Sketch, der eine LED sowohl über einen Taster als auch über die serielle Schnittstelle steuern kann.

### Anforderungen:
- Eine LED ist an Pin 13 angeschlossen
- Ein Taster ist an Pin 2 angeschlossen (mit internem Pull-Up-Widerstand, d.h. normaler Zustand = HIGH, gedrückt = LOW)
- Beim Drücken des Tasters soll die LED ihren Zustand wechseln (Toggle: an → aus, aus → an)
- Über die serielle Schnittstelle soll die LED ebenfalls steuerbar sein:
  - Eingabe '1': LED einschalten
  - Eingabe '0': LED ausschalten
- Der aktuelle LED-Status soll bei jeder Änderung auf dem Serial Monitor ausgegeben werden
- Baudrate: 115200

### Hinweise:
- Achte auf Prellen des Tasters (einfache Lösung mit `delay()`)
- Die serielle Eingabe muss kontinuierlich überprüft werden

---

## Schritt 1: Flussdiagramm

Erstelle ein Flussdiagramm, das den Ablauf des Programms zeigt:


---

## Schritt 2: Code-Implementierung

Schreibe den Arduino-Code, der die Anforderungen erfüllt.

Achte auf:
- Korrekte Pin-Konfiguration (INPUT_PULLUP für Taster)
- Saubere Code-Struktur mit `setup()` und `loop()`
- Logische Zustandsverwaltung der LED
