# Merkblatt – Guter Programmierstil in C

Dieses Merkblatt bietet eine kompakte Grundlage für das Schreiben von solidem Code.

---

## 1. Grundlagen

### 1.1 Konsistenz und Formatierung

**Richtlinie:** Einheitliches Layout verwenden (Einrückungen, Zeilenlängen, Leerzeichen, Kommentare). Ein konsistenter Stil erleichtert das Lesen und Warten erheblich.

**Gutes Beispiel**

```c
// K&R-Klammerstil, 4 Leerzeichen Einzug
int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}
```

**Schlechtes Beispiel**

```c
int max( int a,int b)
{
if(a>b){return a;} return b; }

int max(int a, int b)
{
if(a>b)
    {
return a;
}
    return b;
}
```

---

### 1.2 Konsistenter Stil: Allman vs. K&R Klammersetzung, snake_case vs. camelCase

**Richtlinie:** Stil (Klammer- und Namenskonvention) im gesamten Projekt einheitlich wählen und durchhalten.

**Allman-Style mit snake_case (einheitlich):**
```c
int berechne_summe(int erste_zahl, int zweite_zahl)
{
    return erste_zahl + zweite_zahl;
}
```

**K&R-Style mit camelCase (einheitlich):**
```c
int berechneSumme(int ersteZahl, int zweiteZahl) {
    return ersteZahl + zweiteZahl;
}
```

**Schlechtes Beispiel (Mischung):**
```c
int berechneSumme(int erste_zahl, int zweiteZahl)
{
    return erste_zahl + zweiteZahl;
}

int diff(int zahl_1, int zahl2) {
    return zahl_1 - zahl2;
}
```

### 1.3 Sprechende Namen

**Richtlinie:** Aussagekräftige, konsistente Namen für Variablen, Funktionen und Typen vergeben. Und ebenfalls für die Codedatei selbst soll einen sinnvollen Namen erhalten.

**Gutes Beispiel**

```c
int berechne_durchschnitt_temperatur(const int *werte, int anzahl);
int anzahl_sensoren = 4;
```

**Schlechtes Beispiel**

```c
int f(const int *a, int n);
int x = 4; // Was ist x?
```

**Hinweis:** Englische Variablennamen und Kommentare sind zu bevorzugen - aber auch hier unbedingt einheitlich (nur deutsch oder nur englisch)!

---

### 1.4 Kommentare

**Richtlinie:** Das *Warum* und *Was* dokumentieren, nicht das Offensichtliche. Kommentare aktuell halten.

**Gutes Beispiel**

```c
// Nutzt gleitenden Durchschnitt, um Ausreißer zu glätten
float filter_mittel(float neuer_wert) {
    static float mittel = 0.0f;
    mittel = 0.8f * mittel + 0.2f * neuer_wert; // Gewichtung 80/20
    return mittel;
}
```

**Schlechtes Beispiel**

```c
i = i + 1; // i um 1 erhöhen (überflüssig)
```

---

### 1.5 Kommentare – Fileheader

**Richtlinie:** Ein Fileheader steht am Anfang jeder Datei und beschreibt deren Zweck, Autor, Datum und weitere relevante Informationen.

**Gutes Beispiel**

```c
/*
 * File:           modulname.c
 * Autor:          Max Mustermann
 * Datum:          2024-06-01
 * Version:        1.0
 * Lizenz:         MIT
 * 
 * Beschreibung:   Implementierung der Sensorlogik - liest Sensordaten aus und berechnet den Mittelwert.
 */
```

**Schlechtes Beispiel**

```c
// sensor.c
#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
}
```

**Hinweis:** Der Fileheader sollte immer aktuell gehalten werden und mindestens Zweck, Autor und Datum enthalten. Zusätzliche Angaben wie Version, Lizenz oder Änderungsverlauf sind hilfreich, aber optional.

---

### 1.6 Funktionen / Module

**Richtlinie:** Probleme in kleine, fokussierte Funktionen/Module zerlegen. Jede Funktion erfüllt eine klar umrissene Aufgabe.

**Gutes Beispiel**

```c
bool lese_datei(const char *pfad, char *puffer, int groesse);
int zaehle_zeilen(const char *puffer);
```

**Schlechtes Beispiel**

```c
// Macht "alles": öffnen, lesen, parsen, drucken, schließen
int process(const char *f) { /* 200+ Zeilen */ }
```

---

### 1.7 Vermeidung von magischen Zahlen

**Richtlinie:** Hart kodierte Zahlen durch Konstanten, Enums oder Makros mit Bedeutung ersetzen.

**Gutes Beispiel**

```c
#define MAX_PUFFER 256 // defines immer in Großbuchstaben damit sind sie immer klar als solche erkennbar

char puffer[MAX_PUFFER];
```

```c
// alternativ Konstante verwenden, hat den Vorteil das der Datentyp definiert ist
const int max_puffer = 256;

char puffer[max_puffer];
```

**Schlechtes Beispiel**

```c
char puffer[256]; // Warum 256? Was wenn man es hier anpasst? Wo noch?
```

---

### 1.8 Fehlerbehandlung

**Richtlinie:** Rückgabewerte prüfen, Fehler kontrolliert behandeln und nützliche Diagnosen ausgeben.

**Gutes Beispiel**

```c
FILE *fp = fopen("daten.txt", "r");
if (fp == NULL) {
    perror("fopen");
    return EXIT_FAILURE;
}
// ...
fclose(fp);
```

**Schlechtes Beispiel**

```c
FILE *fp = fopen("daten.txt", "r");
// Annahme: wird schon klappen
// ... Null-Pointer-Dereferenz droht
```

---

### 1.9 Compiler-Warnungen

**Richtlinie:** Mit "strengen" Warnungen ("-Wall") kompilieren und diese sofort beheben.

**Gutes Beispiel**

```c
// Kompilieren (z. B. mit GCC/Clang): -Wall -Wextra -Werror
int summe(const int *a, int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) {
        s += a[i];
    }
    return s;
}
```

**Schlechtes Beispiel**

```c
int summe(int *a, int n) {
    int s = 0, i; // i uninitialisiert – erzeugt Warnung
    for (; i < n; i++) s += a[i];
    return s;
}
```

### 1.10 Globale Variablen vs. Parameterübergabe (Pointer)

**Richtlinie:** Globale Variablen vermeiden, da sie die Wartbarkeit und Testbarkeit erschweren. Stattdessen lokale Variablen oder Übergabe per Parameter (Pointer) bevorzugen.

**Gutes Beispiel**

```c
int berechne_summe(const int *werte, int anzahl); // Werte werden übergeben
```

**Schlechtes Beispiel**

```c
int werte[10]; // global definiert
int berechne_summe(void) { /* nutzt globale Variable */ }
```

### 1.11 Globale vs. lokale statische Variablen

**Richtlinie:** Lokale statische Variablen verwenden, wenn ein Wert über Funktionsaufrufe hinweg erhalten bleiben soll, aber keine globale Sichtbarkeit benötigt wird. Globale Variablen nur, wenn zwingend erforderlich.

**Gutes Beispiel**

```c
// Zähler bleibt zwischen Funktionsaufrufen erhalten, ist aber nur hier sichtbar
void zaehler_beispiel(void) {
    static int aufrufe = 0;
    aufrufe++;
    printf("Funktion wurde %d-mal aufgerufen\n", aufrufe);
}
```

**Schlechtes Beispiel**

```c
// Globale Variable – kann überall verändert werden, erschwert Fehlersuche
int aufrufe = 0;

void zaehler_beispiel(void) {
    aufrufe++;
    printf("Funktion wurde %d-mal aufgerufen\n", aufrufe);
}
```

---

**Gutes Beispiel**

```c
// Statische lokale Variable – verhält sich wie eine globale ist jedoch nur in der Funktion selbst sichtbar
int blinking(int time_period) {
    static unsigned long previous_millis = 0;
    // ...
    if (millis() - previous_millis > time_period/2) {
        // ...
    }
}
```

**Schlechtes Beispiel**

```c
// Globale Variable – überall sichtbar, fehleranfällig
unsigned long previous_millis = 0;

int blinking(int time_period) {
    // ...
    if (millis() - previous_millis > time_period/2) {
        // ...
    }
}
```

---

## 2. Fortgeschrittene Themen

### 2.1 Effizienz vs. Lesbarkeit

**Richtlinie:** Optimierung erst durchführen, wenn diese tatsächlich benötigt werden.

**Gutes Beispiel**

```c
// Klar und gut zu optimieren - gut lesbar
int summe(const int *a, int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) {
        s += a[i];
    }
    return s;
}
```

**Schlechtes Beispiel**

```c
// Undurchsichtige Mikro-Optimierung
int summe(const int *a, int n) {
    int s = 0; const int *end = a + n;
    while (a != end) s += *a++;
    return s; // Gewinn minimal, Lesbarkeit leidet
}
```

---

### 2.2 Coding-Guidelines

**Richtlinie:** Projektspezifische Konventionen (Benennung, Klammerstil, Dateistruktur) befolgen. Formatierung automatisieren.

**Hinweis:** Der Code (Variablennamen, Funktionen, Kommentare) sollte immer auf Englisch verfasst werden. Englisch ist die Standardsprache in der Softwareentwicklung und erleichtert Zusammenarbeit, Wartung und Verständnis – auch international.

**Gutes Beispiel**

```c
// .clang-format in Repo; uniform snake_case
static int read_sensor_value(void);
```

**Schlechtes Beispiel**

```c
// Mischung aus Styles im selben Projekt
static int ReadSensorValue(); // CamelCase hier
static int lese_sensorwert();  // snake_case dort (und deutsch)
```

---

### 2.3 Speicherfreigabe

**Richtlinie:** `free()` verwenden und Ressourcen in allen Pfaden freigeben.

**Gutes Beispiel**

```c
char *buf = malloc(1024);
if (!buf) return -1;
if (lade(buf) != 0) {
    free(buf);
    return -1;
}
// ...
free(buf);
```

**Schlechtes Beispiel**

```c
char *buf = malloc(1024);
if (lade(buf) != 0) {
    return -1; // Leak!
}
```

---

### 2.4 Portabilität

**Richtlinie:** Standardkonforme C-Features und feste Breiten aus `<stdint.h>` nutzen. Nicht standardisierte Funktionen vermeiden.

**Hintergrund:** _int_ auf dem Arduino sind 16-bit, _int_ auf dem ESP32 sind 32-bit.

**Gutes Beispiel**

```c
#include <stdint.h>
#include <inttypes.h>

uint64_t z = 0;
printf("Wert: %" PRIu64 "\n", z);
```

**Schlechtes Beispiel**

```c
long long z = 0;          // Annahme über Größe
printf("%llu\n", z);      // Undefiniertes Verhalten auf manchen Plattformen
// itoa(z, str, 10);      // Nicht standardisierte Funktion
```

---

### 2.5 Testen

**Richtlinie:** Kleine, automatisierbare Tests schreiben (z. B. mit `assert`). Randfälle abdecken.

**Gutes Beispiel**

```c
#include <assert.h>

int max(int a, int b);

int main(void) {
    assert(max(2, 3) == 3);
    assert(max(-1, -5) == -1);
    return 0;
}
```

**Schlechtes Beispiel**

```c
int main(void) {
    // "Sieht richtig aus" – rein visuelle Kontrolle
    printf("%d\n", max(2, 3));
}
```

---

### 2.6 Projektorganisation

**Richtlinie:** Projekt mit klaren Verzeichnissen und Header/Source-Trennung strukturieren. Reproduzierbare Builds sicherstellen.

**Gutes Beispiel**

```
projekt/
├─ include/
│  └─ mathx.h
├─ src/
│  └─ mathx.c
└─ Makefile
```

```c
// include/mathx.h
int max(int a, int b);
```

```c
// src/mathx.c
#include "mathx.h"
int max(int a, int b) {
    return (a > b) ? a : b;
}
```

**Schlechtes Beispiel**

```c
// alles_in_einer_datei.c – Header, Implementierung, Test gemischt
int max(int a, int b) { /* ... */ }
// hunderte Zeilen unsortiert
```

---

### 2.7 Versionierung

**Richtlinie:** Git für Nachvollziehbarkeit, Branches und Reviews nutzen. Kleine, klare Commits mit aussagekräftigen Messages erstellen.

**Gutes Beispiel**

```bash
git init
git add src/ include/
git commit -m "feat: fügt max()-Funktion mit Tests hinzu"
git pull
```

**Schlechtes Beispiel**

```
Projekt_final_NEU_endgueltig_v3.zip
Projekt_final_NEU_endgueltig_v3_final.zip
```

---

### 2.8 Sicherheit

**Richtlinie:** Pufferüberläufe vermeiden, Eingaben und Grenzen prüfen, sichere Varianten bevorzugen.

**Gutes Beispiel**

```c
#define MAX 64
char name[MAX];
if (fgets(name, MAX, stdin) == NULL) {
    fprintf(stderr, "Eingabe fehlgeschlagen\n");
}
```

**Schlechtes Beispiel**

```c
char name[32];
gets(name); // Unsicher, kann Pufferüberlauf verursachen
```

---
