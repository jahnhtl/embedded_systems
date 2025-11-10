# Grundlegende Konsolenbefehle (Windows)

Hier sind einige wichtige Konsolenbefehle, die in der Windows-Eingabeaufforderung verwendet werden:

## 1. Navigation und Orientierung

### 1.1 `cd` oder `pwd` - Aktuelles Verzeichnis anzeigen
Zeigt das aktuelle Arbeitsverzeichnis an.
```sh
cd
```
Mit diesem Befehl siehst du, in welchem Ordner du dich gerade befindest.

### 1.2 `dir` oder `ls` - Verzeichnisinhalt anzeigen
Listet alle Dateien und Ordner im aktuellen Verzeichnis auf.
```sh
dir
```
Zeigt den Inhalt des aktuellen Ordners an.

### 1.3 `cd` - Verzeichnis wechseln
Wechselt das aktuelle Verzeichnis.
```sh
cd ordnername
```
Mit diesem Befehl navigierst du in einen bestimmten Ordner.

### 1.4 `cd ..` - Eine Ebene nach oben
Wechselt eine Ebene nach oben im Verzeichnisbaum.
```sh
cd ..
```
Mit diesem Befehl gelangst du in das übergeordnete Verzeichnis.

## 2. Ordner- und Dateiverwaltung

### 2.1 `mkdir` - Ordner erstellen
Erstellt einen neuen Ordner.
```sh
mkdir neuerordner
```
Damit legst du einen neuen Ordner im aktuellen Verzeichnis an.

### 2.2 `rmdir` - Ordner löschen
Löscht einen Ordner (der Ordner muss leer sein).
```sh
rmdir ordnername
```
Entfernt einen bestehenden, leeren Ordner.

### 2.3 `copy` - Dateien kopieren
Kopiert Dateien von einem Ort zu einem anderen.
```sh
copy dateiname zielordner
copy datei1.txt datei2.txt
```
Kopiert eine Datei in einen anderen Ordner oder erstellt eine Kopie mit neuem Namen.

### 2.4 `move` - Dateien verschieben/umbenennen
Verschiebt Dateien oder benennt sie um.
```sh
move altername neuername
move datei.txt neuerordner\
```
Verschiebt eine Datei oder benennt sie um.

### 2.5 `del` - Dateien löschen
Löscht eine oder mehrere Dateien.
```sh
del dateiname.txt
del *.txt
```
Löscht die angegebene Datei oder alle Dateien mit der angegebenen Endung.

## 3. Text und Ausgabe

### 3.1 `type` - Dateiinhalt anzeigen
Zeigt den Inhalt einer Textdatei an.
```sh
type dateiname.txt
```
Gibt den Inhalt einer Datei in der Konsole aus.

### 3.2 `echo` - Text ausgeben
Gibt Text aus oder erstellt einfache Textdateien.
```sh
echo Hallo Welt
echo "Text" > datei.txt
```
Zeigt Text an oder leitet ihn in eine Datei um.

### 3.3 `cls` - Bildschirm löschen
Löscht den Inhalt der Konsole.
```sh
cls
```
Macht den Bildschirm sauber und übersichtlich.

## 4. Git-Befehle (nach Git-Installation)

### 4.1 `git clone` - Repository klonen
Kopiert ein Git-Repository von einem Server auf deinen Computer.
```sh
git clone https://github.com/benutzername/projektname.git
```
Lädt ein komplettes Projekt von GitHub herunter.

### 4.2 `git add` - Änderungen vormerken
Fügt Dateien zur Versionskontrolle hinzu.
```sh
git add dateiname.txt
git add .
```
Bereitet Dateien für den nächsten Commit vor (. = alle Dateien).

### 4.3 `git commit` - Änderungen speichern
Speichert die vorgemerkten Änderungen mit einer Beschreibung.
```sh
git commit -m "Beschreibung der Änderung"
```
Erstellt einen neuen Versionsstand deines Projekts.

### 4.4 `git push` - Änderungen hochladen
Lädt deine lokalen Commits zum Remote-Repository hoch.
```sh
git push
```
Synchronisiert deine Änderungen mit GitHub oder einem anderen Git-Server.

### 4.5 `git pull` - Änderungen herunterladen
Lädt neue Änderungen vom Remote-Repository herunter.
```sh
git pull
```
Aktualisiert dein lokales Projekt mit den neuesten Änderungen.

---

**Tipps:** 
- Mit `help befehl` erhältst du weitere Informationen zu einem bestimmten Befehl
- Mit der **Tab-Taste** kannst du Datei- und Ordnernamen automatisch vervollständigen
- Mit den **Pfeiltasten** (↑↓) kannst du durch vorherige Befehle navigieren