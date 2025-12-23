# Übungsbeispiel 2: LED-Helligkeitssteuerung mit Potentiometer

## Aufgabenstellung

Programmiere einen Arduino-Sketch, der die Helligkeit einer LED über ein Potentiometer steuert und den aktuellen Wert kontinuierlich auf dem Serial Monitor ausgibt.

### Anforderungen:
- Eine LED ist an Pin 9 angeschlossen (PWM-fähiger Pin)
- Ein Potentiometer ist an Pin A0 angeschlossen
- Die LED-Helligkeit soll proportional zur Potentiometer-Stellung sein
- Der aktuelle Potentiometer-Wert (0-1023) und die LED-Helligkeit (0-255) sollen alle 500ms auf dem Serial Monitor ausgegeben werden
- Baudrate: 115200

### Hinweise:
- Potentiometer liefert Werte von 0-1023 (10-Bit ADC)
- PWM für LED-Helligkeit nutzt Werte von 0-255 (8-Bit)
- Wertebereich muss umgerechnet werden (Mapping mit map Funktion - siehe [Arduino Referenz](https://www.arduino.cc/reference/en/language/functions/math/map/) oder überlege dir eine passende simple Division)

---

## Schritt 1: Flussdiagramm

Erstelle ein Flussdiagramm, das den Ablauf des Programms zeigt:



---

## Schritt 2: Code-Implementierung

Schreibe den Arduino-Code, der die Anforderungen erfüllt.

Achte auf:
- Korrekte Pin-Konfiguration (OUTPUT für LED)
- Werteumrechnung von 0-1023 auf 0-255
- Übersichtliche serielle Ausgabe
- Saubere Code-Struktur mit `setup()` und `loop()`
