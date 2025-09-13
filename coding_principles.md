# Merkblatt – Guter Programmierstil

**Zielgruppe:** HTL – Zweig *Technische Informatik* · **Sprache & Beispiele:** C

Dieses Merkblatt bietet eine kompakte Grundlage für guten Code in Ausbildung und Praxis. Halten Sie die folgenden Leitlinien konsequent ein. Zu jedem Punkt finden Sie ein gutes und ein schlechtes Beispiel.

---

## 1 Basics

### 1.1 Konsistenz und Formatierung
**Richtlinie:** Verwenden Sie ein einheitliches Layout (Einrückungen, Klammern, Zeilenlängen, Leerzeichen, Kommentare). Ein konsistenter Stil erleichtert das Lesen und Warten erheblich.

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
```

---

### 1.2 Sprechende Namen
**Richtlinie:** Vergeben Sie aussagekräftige, konsistente Namen für Variablen, Funktionen und Typen.

**Gutes Beispiel**
```c
int berechne_durchschnitt_temperatur(const int *werte, size_t anzahl);
int anzahl_sensoren = 4;
```

**Schlechtes Beispiel**
```c
int f(const int *a, int n);
int x = 4; // Was ist x?
```

---

### 1.3 Kommentare
**Richtlinie:** Kommentieren Sie das *Warum* und *Was*, nicht das Offensichtliche. Halten Sie Kommentare aktuell.

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
int i = i + 1; // i um 1 erhöhen (überflüssig)
```

---

### 1.4 Funktionen / Module
**Richtlinie:** Zerlegen Sie Probleme in kleine, fokussierte Funktionen/Module. Eine Funktion soll eine klar umrissene Aufgabe erfüllen.

**Gutes Beispiel**
```c
bool lese_datei(const char *pfad, char *puffer, size_t groesse);
size_t zaehle_zeilen(const char *puffer);
```

**Schlechtes Beispiel**
```c
// Macht "alles": öffnen, lesen, parsen, drucken, schließen
int process(const char *f) { /* 200+ Zeilen */ }
```

---

### 1.5 Vermeidung von magischen Zahlen
**Richtlinie:** Ersetzen Sie hart kodierte Zahlen durch Konstanten, Enums oder Makros mit Bedeutung.

**Gutes Beispiel**
```c
#define 
enum { MAX_PUFFER = 256 };
char puffer[MAX_PUFFER];
```

**Schlechtes Beispiel**
```c
char puffer[256]; // Warum 256? Was wenn man es hier anpasst? Wo noch?
```

---

### 1.6 Fehlerbehandlung
**Richtlinie:** Prüfen Sie Rückgabewerte, behandeln Sie Fehler kontrolliert und geben Sie nützliche Diagnosen aus.

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

### 1.7 Compiler-Warnungen
**Richtlinie:** Kompilieren Sie mit strengen Warnungen und beheben Sie diese sofort.

**Gutes Beispiel**
```c
// Kompilieren (z. B. mit GCC/Clang): -Wall -Wextra -Werror
int summe(const int *a, size_t n) {
    int s = 0;
    for (size_t i = 0; i < n; ++i) {
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

---

## 2 Advanced

### 2.1 Effizienz
**Richtlinie:** Optimieren Sie erst nach Messung (Profiling). Bevorzugen Sie klare O(n)-Lösungen statt fragiler Mikro-Optimierungen.

**Gutes Beispiel**
```c
// Klar und gut zu optimieren
int summe(const int *a, size_t n) {
    int s = 0;
    for (size_t i = 0; i < n; ++i) {
        s += a[i];
    }
    return s;
}
```

**Schlechtes Beispiel**
```c
// Undurchsichtige Mikro-Optimierung
int summe(const int *a, size_t n) {
    int s = 0; const int *end = a + n;
    while (a != end) s += *a++;
    return s; // Gewinn minimal, Lesbarkeit leidet
}
```

---

### 2.2 Coding-Guidelines
**Richtlinie:** Befolgen Sie die projektspezifischen Konventionen (Benennung, Klammerstil, Dateistruktur). Automatisieren Sie Formatierung.

**Gutes Beispiel**
```c
// .clang-format im Repo; einheitlich snake_case
static int lese_sensorwert(void);
```

**Schlechtes Beispiel**
```c
// Mischung aus Styles im selben Projekt
static int ReadSensorValue(); // CamelCase hier
static int lese_sensorwert();  // snake_case dort
```

---

### 2.3 Speicherfreigabe
**Richtlinie:** Verwenden Sie `free()` und geben Sie Ressourcen in allen Pfaden frei (RAII-ähnliche Muster, "goto cleanup" u. ä.).

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

### 2.4 Portabilität
**Richtlinie:** Nutzen Sie standardkonforme C‑Features und feste Breiten aus `<stdint.h>`. Vermeiden Sie nicht standardisierte Funktionen.

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
printf("%llu\n", z);     // Undefiniertes Verhalten auf manchen Plattformen
// itoa(z, str, 10);      // Nicht standardisiert
```

---

### 2.5 Testen
**Richtlinie:** Schreiben Sie kleine, automatisierbare Tests (z. B. mit `assert`). Testen Sie Randfälle.

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

### 2.6 Projektorganisation
**Richtlinie:** Strukturieren Sie das Projekt mit klaren Verzeichnissen und Header/Source‑Trennung. Bauen Sie reproduzierbar.

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

### 2.7 Versionierung
**Richtlinie:** Nutzen Sie Git für Nachvollziehbarkeit, Branches und Reviews. Committen Sie klein und mit klaren Messages.

**Gutes Beispiel**
```bash
git init
git add src/ include/
git commit -m "feat: fügt max()-Funktion mit Tests hinzu"
git switch -c fix/off-by-one
```

**Schlechtes Beispiel**
```
Projekt_final_NEU_endgueltig_v3.zip
Projekt_final_NEU_endgueltig_v3_final.zip
```

---

### 2.8 Sicherheit
**Richtlinie:** Vermeiden Sie Pufferüberläufe, prüfen Sie Eingaben und Grenzen, und bevorzugen Sie sichere Varianten.

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

