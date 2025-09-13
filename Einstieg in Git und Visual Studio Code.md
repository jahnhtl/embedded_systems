# Einstieg in Git und Visual Studio Code

## Was ist Git?

Git ist ein Programm, mit dem du Änderungen an Dateien speichern und verwalten kannst. Es hilft dir, den Überblick über verschiedene Versionen deiner Projekte zu behalten und mit anderen zusammenzuarbeiten.

### Wichtige Begriffe

- **Repository (Repo):** Ein Ordner, in dem Git alle Dateien und Änderungen speichert.
- **Commit:** Ein gespeicherter Stand deiner Dateien.
- **Clone:** Eine Kopie eines Repositories von GitHub auf deinen Computer.
- **Push:** Deine Änderungen von deinem Computer zu GitHub hochladen.
- **Pull:** Neue Änderungen von GitHub auf deinen Computer holen.
- **Branch:** Ein "Nebenarm" deines Projekts, um neue Funktionen zu testen.

---

## Schritt 1: GitHub Account erstellen

1. Gehe auf [github.com](https://github.com).
2. Klicke auf "Sign up" und folge den Anweisungen.
3. Bestätige deine E-Mail-Adresse.

---

## Schritt 2: Ein Repository clonen und ausprobieren

Wir nutzen das Beispielprojekt [Arduino-simon-game](https://github.com/wokwi/arduino-simon-game/tree/main).

1. Öffne Visual Studio Code.
2. Öffne das Terminal (`Strg + ö` oder `Strg + Shift + P` und "Terminal öffnen").
3. Gib ein:
    ```bash
    git clone https://github.com/wokwi/arduino-simon-game.git
    ```
4. Öffne den Ordner in VS Code.

### Wokwi Simulator-Code direkt in Visual Studio Code ausführen

Mit der Wokwi-Erweiterung für Visual Studio Code kannst du Arduino-Projekte direkt im Editor simulieren.

1. Installiere die [Wokwi VS Code Extension](https://marketplace.visualstudio.com/items?itemName=Wokwi.wokwi-vscode).
2. Öffne das geklonte Projekt in VS Code.
3. Klicke auf das Wokwi-Symbol in der Seitenleiste.
4. Wähle "Simulate Project" aus.
5. Der Simulator startet und zeigt dein Projekt direkt im Editor an.

So kannst du deinen Code testen, ohne ein echtes Arduino-Board zu benötigen.
---

## Schritt 3: Eigenes Repository "Blinking LED" erstellen

### Möglichkeit 1: Auf GitHub erstellen

1. Gehe auf GitHub und klicke auf "New Repository".
2. Gib als Namen `Blinking_LED` ein.
3. Klicke auf "Create repository".

### Möglichkeit 2: In VS Code clonen

1. Kopiere die Repository-URL von GitHub.
2. In VS Code:
    ```bash
    git clone https://github.com/dein-benutzername/Blinking_LED.git
    ```

### Neues PlatformIO-Projekt erstellen

1. Öffne VS Code und installiere die PlatformIO-Erweiterung.
2. Klicke auf "PlatformIO" > "New Project".
3. Wähle das geklonte Repository als Speicherort.

### Änderungen committen und pushen

1. Im Terminal:
    ```bash
    git add .
    git commit -m "Erstes Blinking LED Projekt"
    git push
    ```

---

## Schritt 4: Beispiel "Blinking_LED_millis" mit Kommandozeile

1. Erstelle ein neues Repository auf GitHub: `Blinking_LED_millis`.
2. Klone es in VS Code:
    ```bash
    git clone https://github.com/dein-benutzername/Blinking_LED_millis.git
    ```
3. Erstelle die Projektdateien mit dem Terminal:
    ```bash
    cd Blinking_LED_millis
    mkdir src
    echo "// Blinking LED mit millis()" > src/main.cpp
    ```
4. Änderungen speichern und hochladen:
    ```bash
    git add .
    git commit -m "Projekt erstellt"
    git push
    ```

---

Viel Erfolg beim Ausprobieren!