# Übungsbeispiel 1a: LED-Steuerung über serielle Eingabe - Lösung

## Flussdiagramm

- Initialisierung (Pin, Serial) - optional
- Hauptschleife mit:
  - Serielle Eingabe prüfen
  - Zeichen auswerten ('1'/'0')
  - LED setzen und Status ausgeben

```mermaid
graph TD
    A["Start"] --> B["Initialisierung:<br/>Pin 13 als OUTPUT<br/>Serial.begin(115200)"]
    B --> D["Variable ledStatus = LOW"]

    D --> E{"Serielle Daten<br/>verfügbar?<br/>Serial.available()"}
    E -->|Ja| F["Zeichen lesen<br/>char c = Serial.read()"]
    F --> G{"c == '1'?"}
    G -->|Ja| H["ledStatus = HIGH"]
    G -->|Nein| I{"c == '0'?"}
    I -->|Ja| J["ledStatus = LOW"]
    H --> K["LED setzen<br/>digitalWrite(13, ledStatus)"]
    J --> K
    K --> L["Status ausgeben<br/>Serial.println"]
    L --> D
    I -->|Nein| D
    E -->|Nein| D
```

---

## Lösung

```cpp
// Pin-Definition
const int LED_PIN = 13;

// LED-Status
int ledStatus = LOW;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("LED-Seriell-Steuerung gestartet (1=Ein, 0=Aus)");
}

void loop() {
  // Serielle Eingabe prüfen
  if (Serial.available() > 0) {
    char eingabe = Serial.read();

    if (eingabe == '1') {
      ledStatus = HIGH;
      digitalWrite(LED_PIN, ledStatus);
      Serial.println("LED eingeschaltet (Seriell)");
    } else if (eingabe == '0') {
      ledStatus = LOW;
      digitalWrite(LED_PIN, ledStatus);
      Serial.println("LED ausgeschaltet (Seriell)");
    }
  }
}
```

---

## Erklärung

- **Serielle Prüfung**: `Serial.available()` stellt fest, ob Daten anliegen
- **Zeichen lesen**: `Serial.read()` liest genau ein Zeichen
- **LED-Steuerung**: '1' → ein, '0' → aus
- **Rückmeldung**: Status wird mit `Serial.println()` ausgegeben
