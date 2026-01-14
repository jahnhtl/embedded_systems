# Übungsbeispiel 54: Komplexe LED-Steuerung mit mehreren Eingabequellen

## Aufgabenstellung

Programmiere ein Arduino-Sketch, das eine LED über verschiedene Eingabequellen steuern kann: Potentiometer für Helligkeit, Taster für Modi und serielle Befehle für direkte Steuerung.

### Anforderungen:
- Eine LED ist an Pin 9 angeschlossen (PWM-fähiger Pin)
- Ein Potentiometer ist an Pin A0 angeschlossen
- Zwei Taster sind an Pin 2 und Pin 3 angeschlossen (mit internem Pull-Up)
- Drei Betriebsmodi:
  - **Modus 0 (Manuell)**: LED-Helligkeit wird über serielle Eingabe gesteuert (0-255)
  - **Modus 1 (Potentiometer)**: LED-Helligkeit folgt Potentiometer-Stellung
  - **Modus 2 (Blinken)**: LED blinkt mit Helligkeit vom Potentiometer
- **Taster 1 (Pin 2)**: Wechselt zwischen den Modi (0→1→2→0...)
- **Taster 2 (Pin 3)**: Invertiert die Helligkeit im aktuellen Modus
- Serielle Befehle:
  - Zahl 0-255: Setzt Helligkeit direkt (nur in Modus 0)
  - 'm': Zeigt aktuellen Modus und Einstellungen an
- Der aktuelle Modus, Helligkeitswert und Invertierungsstatus sollen bei Änderungen ausgegeben werden
- Im Poti-Modus: Werte alle 1000ms ausgeben
- Baudrate: 115200

### Hinweise:
- Drei verschiedene Modi erfordern strukturierte Fallunterscheidung
- `Serial.parseInt()` kann für Zahleneingabe genutzt werden
- Blinken im Modus 2 mit 500ms Intervall
- Invertierung funktioniert in allen Modi

---

## Schritt 1: Flussdiagramm

Erstelle ein Flussdiagramm, das den Ablauf des Programms zeigt:



---

## Schritt 2: Code-Implementierung

Schreibe den Arduino-Code, der die Anforderungen erfüllt.

Achte auf:
- Klare Trennung der drei Modi
- Zustandsvariablen für Modus, Helligkeit, Invertierung
- Übersichtliche Code-Struktur (evtl. Hilfsfunktionen)
- Aussagekräftige serielle Ausgaben
- Saubere Code-Struktur mit `setup()` und `loop()`
