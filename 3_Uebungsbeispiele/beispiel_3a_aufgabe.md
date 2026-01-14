# Übungsbeispiel 3a: LED-Blinken mit variabler Frequenz

## Aufgabenstellung

Programmiere einen Arduino-Sketch, der eine LED mit einstellbarer Blinkfrequenz steuert. Die Frequenz wird über die serielle Schnittstelle eingestellt.

### Anforderungen:
- Eine LED ist an Pin 13 angeschlossen
- Die LED blinkt kontinuierlich (an/aus)
- Über die serielle Schnittstelle kann die Blinkzeit in Millisekunden eingegeben werden (z.B. "500" für 500ms an, 500ms aus)
- Standardwert beim Start: 1000ms
- Verwende `millis()` für die Zeitsteuerung (kein `delay()`)
- Bei jeder Änderung der Blinkzeit soll diese auf dem Serial Monitor ausgegeben werden
- Baudrate: 115200

### Hinweise:
- Nutze `Serial.parseInt()` um Zahlen von der seriellen Schnittstelle zu lesen
- Die LED soll ohne `delay()` blinken
- Keine Arrays, keine oldState-Variable

---

## Schritt 1: Flussdiagramm

Erstelle ein Flussdiagramm, das den Ablauf des Programms zeigt:


---

## Schritt 2: Code-Implementierung

Schreibe den Arduino-Code, der die Anforderungen erfüllt.

Achte auf:
- Verwendung von `millis()` statt `delay()`
- Saubere Verarbeitung der seriellen Eingabe
- Saubere Code-Struktur mit `setup()` und `loop()`
