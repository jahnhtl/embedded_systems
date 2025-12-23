# Übungsbeispiel 4: LED-Helligkeitssteuerung mit Invertierung

## Aufgabenstellung

Programmiere einen Arduino-Sketch, der die Helligkeit einer LED über ein Potentiometer steuert. Ein Taster soll die Helligkeit invertieren können.

### Anforderungen:
- Eine LED ist an Pin 9 angeschlossen (PWM-fähiger Pin)
- Ein Potentiometer ist an Pin A0 angeschlossen
- Ein Taster ist an Pin 2 angeschlossen (mit internem Pull-Up)
- Die LED-Helligkeit soll proportional zur Potentiometer-Stellung sein
- Mit dem Taster soll zwischen normalem und invertiertem Modus umgeschaltet werden:
  - **Normal-Modus**: Poti rechts = hell, links = dunkel
  - **Invertiert-Modus**: Poti rechts = dunkel, links = hell
- Der aktuelle Modus und die Helligkeitswerte sollen alle 500ms auf dem Serial Monitor ausgegeben werden
- Baudrate: 115200

### Hinweise:
- Invertierung: `helligkeitInv = 255 - helligkeit`
- Taster mit Pull-Up (gedrückt = LOW)
- Einfache Entprellung mit delay()

---

## Schritt 1: Flussdiagramm

Erstelle ein Flussdiagramm, das den Ablauf des Programms zeigt:

---

## Schritt 2: Code-Implementierung

Schreibe den Arduino-Code, der die Anforderungen erfüllt.

Achte auf:
- Zustandsvariable für Invertierung
- Korrekte Berechnung der invertierten Helligkeit
- Übersichtliche serielle Ausgabe mit Modusanzeige
- Saubere Code-Struktur mit `setup()` und `loop()`
