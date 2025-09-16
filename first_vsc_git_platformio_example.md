# Einstieg in Git und Visual Studio Code

**Voraussetzung:** Installiere Visual Studio Code, Git und einige hilfreiche Plugins wie hier beschrieben -> [Installationsanleitung](visual_studio_code_install.md).

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


## Schritt 1: Ein Repository clonen und compilieren (builden)

Wir nutzen das Beispielprojekt [Arduino-simon-game](https://github.com/wokwi/arduino-simon-game/tree/main).

1. Öffne Visual Studio Code.
2. Öffne das Terminal (`Strg + ö` oder `Strg + Shift + P` und "Terminal öffnen").
3. Wechsle nun in das Verzeichnis in das das Projekt kopiert werden soll. Dazu tipp ein (für das Schul-setup):
    ```bash
    Z:              # wechlse auf das Laufwerk Z
    cd 5AAELI       # change directory - wechselt in das Verzeichnis 5AAELI
                    # (ersetze 5AAELI mit dem passenden vorhandenen Ordner)
    mkdir ESY       # erstellt einen neuen Ordner ESY
    cd ESY          # wechselt in den Ordner ESY

    # das Terminal zeigt nun an Z:\5AAELI\ESY
    ```
4. Alternativ erstelle über den Windows Explorer den gewünschten Ordner - klicke oben in die Adressezeile und kopiere den gesamten Pfad (*STRG + C*) und tippe:
    ```bash
    cd Z:\5AAELI\ESY  # wiederum change directory und den Pfad einfügen mit STRG+V

    # das Terminal zeigt nun wiederuman Z:\5AAELI\ESY
    ```
5. Gib ein um das Repository von Github lokal zu kopieren (= clonen)
    ```bash
    git clone https://github.com/wokwi/arduino-simon-game.git
    ```
6. Öffne den Ordner in VS Code. Klicke dazu auf **Menü -> File -> OpenFolder**
7. Um das **Projekt zu kompilieren** klicke in der Statuszeile auf das Häkchen-Symbol („Build“) oder wähle im PlatformIO-Menü Build aus. Alternativ kannst du Ctrl + Alt + B drücken.

**Hinweis:** Wenn mehrere Ordner geöffnet werden soll wählen Sie **Add Folder to Workspace**.

### Wokwi Simulator-Code direkt in Visual Studio Code ausführen

Mit der Wokwi-Erweiterung für Visual Studio Code kannst du Arduino-Projekte direkt im Editor simulieren.

1. Installiere die [Wokwi VS Code Extension](https://marketplace.visualstudio.com/items?itemName=Wokwi.wokwi-vscode).
2. Öffne das geklonte Projekt in VS Code.
3. Kompiliere das Projekt
4. Drücke **F1** oder **Ctrl+Shift+P**, um die Befehlspalette zu öffnen.
5. Tippe **Wokwi** und wähle **Wokwi: Start Simulator** aus
5. Der Simulator startet und zeigt dein Projekt direkt im Editor an.

So kannst du deinen Code testen, ohne ein echtes Arduino-Board zu benötigen.
---

## Schritt 2: Eigenes Repository "Blinking LED" auf GitHub erstellen

### Repo auf GitHub erstellen

1. Gehe auf [github.com](github.com), logge dich mit deinem Benutzernamen ein
2. Klicke in der linken Seitenleiste auf "New" oberhalb deiner Repositories.
3. Gib als Namen `Blinking_LED` ein.
4. Wähle aus, ob dein Repository **public** (öffentlich) oder **private** (privat) sein soll.

    - **Public:** Jeder kann dein Repository sehen und darauf zugreifen. Ideal, wenn du deinen Code teilen oder anderen zeigen möchtest.
    - **Private:** Nur du und ausgewählte Personen können das Repository sehen. Gut geeignet für Projekte, die vertraulich bleiben sollen.

    Die Einstellung kannst du später jederzeit ändern. Beachte: Bei privaten Repositories können andere deinen Code nicht ohne Einladung sehen oder nutzen.

5. Aktiviere die Option **Add a README file**, um eine erste Beschreibung deines Projekts zu erstellen. Die README-Datei hilft anderen, dein Projekt zu verstehen (z.B. Zweck, Installation, Nutzung). Die Datei wird im **Markdown-Format (.md)** geschrieben und kann jederzeit bearbeitet und erweitert werden.

6. Wähle **Add .gitignore** und suche nach dem passenden Template (z.B. Arduino, C++, PlatformIO). Die .gitignore-Datei sorgt dafür, dass bestimmte Dateien (z.B. Build-Ordner, temporäre Dateien) nicht ins Repository gelangen.

7. Mit **Add a license** kannst du eine Lizenz für dein Projekt auswählen (z.B. MIT, GPL, Apache). Die Lizenz regelt, wie andere deinen Code nutzen dürfen. [Choose a license](https://choosealicense.com) hilft dir die richtige Lizenz zu wählen. 
8. Geschafft - klicke auf "Create repository".


### In VS Code clonen via terminal - Option 1

1. Kopiere die Repository-URL von GitHub.
2. In VS Code:
    ```bash
    git clone https://github.com/dein-benutzername/Blinking_LED.git
    ```

### In VS Code clonen via Befehlspalette - Option 2

1. Klicke oben links auf **"Ansicht"** > **"Befehlspalette..."** (oder drücke `F1` bzw. `Ctrl+Shift+P`).
2. Gib **"Git: Clone"** ein und wähle den Befehl **"Git: Clone"** aus.
3. Füge die Repository-URL ein (z.B. `https://github.com/dein-benutzername/Blinking_LED.git`) und bestätige mit Enter.
4. Wähle einen Ordner aus, in den das Projekt gespeichert werden soll.
5. VS Code fragt, ob du das geklonte Projekt direkt öffnen möchtest – bestätige mit **"Öffnen"**.

Dadurch wird das Git-Repository direkt über die VS Code-Oberfläche geklont, ohne das Terminal zu benutzen.


### Neues PlatformIO-Projekt erstellen

1. Öffne VS Code und installiere die PlatformIO-Erweiterung.
2. Klicke auf "PlatformIO" > "New Project".
3. Wähle das geklonte Repository als Speicherort.

### Code schreiben und testen

1. Öffne im geklonten Repository den Ordner `src` und erstelle eine Datei namens `main.cpp`.
2. Schreibe den notwendigen Code um die LED auf Pin13 blinken zu lassen.
3. Speichere die Datei.

#### Option 1: Projekt mit PlatformIO bauen und hochladen

1. Öffne die PlatformIO-Seitenleiste in VS Code.
2. Klicke auf **"Build"** (Häkchen-Symbol), um das Projekt zu kompilieren.
3. Schließe dein Arduino-Board per USB an.
4. Klicke auf **"Upload"** (Pfeil-Symbol), um den Code auf das Board zu übertragen.
5. Überprüfe die Ausgabe im Terminal, ob der Upload erfolgreich war.

Damit kannst du deinen Code direkt auf das echte Board spielen und testen.

#### Option 2: Testen mit Wokwi

1. Drücke `F1` oder `Ctrl+Shift+P` um die Befehlspalette zu öffnen.
2. Gib **"Wokwi: Start Simulation"** ein und wähle den entsprechenden Befehl aus.
3. Beobachte im Simulator, wie die LED blinkt.

So kannst du deinen Code direkt testen, ohne ein echtes Arduino-Board zu benötigen.



### Änderungen committen und pushen

#### Option 1: Via terminal

1. Achte darauf, dass du im Terminal im richtigen Verzeichnis bist:
2. Führe folgende Befehle aus.
    ```bash
    # Änderungen zum Commit vormerken
    git add .
    # erstelle von allen Dateien die geändert wurden eine neue Version (lokal)
    git commit -m "Erstes Blinking LED Projekt"
    # Änderungen ins Remote-Repository hochladen
    git push
    ```

**Tipp:**  
Falls du noch kein Remote-Repository verbunden hast, musst du es zuerst mit  
`git remote add origin <URL>`  
verbinden.

#### Option 2: Über die grafische Oberfläche von VS Code

1. Öffne die Seitenleiste **Quellcodeverwaltung** (Symbol mit drei Linien und Punkten oder `Ctrl+Shift+G`).
2. Du siehst alle geänderten Dateien unter "Änderungen".
3. Gib eine Commit-Nachricht ins Feld oben ein (z.B. "Erstes Blinking LED Projekt").
4. Klicke auf das Häkchen-Symbol, um zu committen.
5. Klicke auf die drei Punkte (`...`) oben und wähle **"Push"** aus, um die Änderungen zu GitHub hochzuladen.

Damit kannst du Änderungen einfach und übersichtlich ohne Terminal verwalten.

---

## Schritt 3: Beispiel "Blinking_LED_millis" mit PlatformIO lokal in VS Code erstellen und auf GitHub hochladen

1. Öffne Visual Studio Code und installiere die PlatformIO-Erweiterung.
2. Klicke in der PlatformIO-Seitenleiste auf **"New Project"**.
3. Gib als Projektnamen `Blinking_LED_millis` ein, wähle dein Board (z.B. Arduino Uno) und als Speicherort einen neuen Ordner.
4. Nach dem Erstellen öffnet sich das Projekt automatisch in VS Code.
5. Öffne den Ordner `src` und erstelle oder bearbeite die Datei `main.cpp`. Schreibe dort deinen Code für das LED-Blinken mit `millis()`.

### Neues Git-Repository erstellen
1. Öffne das Terminal in VS Code und initialisiere ein neues Git-Repository:
    ```bash
    # neues Repo initialisieren
    git init
    git add .
    git commit -m "Erstes Blinking_LED_millis Projekt mit PlatformIO"
    ```
2. Erstelle ein neues Repository auf GitHub mit dem Namen `Blinking_LED_millis`.
3. Verbinde dein lokales Repository mit GitHub:
    ```bash
    git remote add origin https://github.com/dein-benutzername/Blinking_LED_millis.git
    git push -u origin master
    ```
4. Dein PlatformIO-Projekt ist jetzt auf GitHub verfügbar.

---

Viel Erfolg beim Ausprobieren!