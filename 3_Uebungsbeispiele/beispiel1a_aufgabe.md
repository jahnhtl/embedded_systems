# Übungsbeispiel 1a: LED-Steuerung über serielle Eingabe

## Aufgabenstellung

Programmiere einen Arduino-Sketch, der eine LED ausschließlich über die serielle Schnittstelle steuert.

### Anforderungen:
- Eine LED ist an Pin 13 angeschlossen
- Über die serielle Schnittstelle soll die LED steuerbar sein:
  - Eingabe '1': LED einschalten
  - Eingabe '0': LED ausschalten
- Der aktuelle LED-Status soll bei jeder Änderung auf dem Serial Monitor ausgegeben werden
- Baudrate: 115200

### Hinweise:
- Die serielle Eingabe muss kontinuierlich überprüft werden (`Serial.available()`)
- Einzelzeichen lesen mit `Serial.read()`

---

## Schritt 1: Flussdiagramm

Erstelle ein Flussdiagramm, das den Ablauf des Programms zeigt:


---

## Schritt 2: Code-Implementierung

Schreibe den Arduino-Code, der die Anforderungen erfüllt.

Achte auf:
- Korrekte Pin-Konfiguration (OUTPUT für LED)
- Saubere Code-Struktur mit `setup()` und `loop()`
