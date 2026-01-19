# Datentypen

## Integer

Beim Programmieren von Mikrocontrollern sollte man den kleinstmöglichen Datentyp verwenden. In C hängt die Größe von `int` vom System ab (16 oder 32 Bit). Für bessere Portabilität des Codes von einem zum anderen Microcontroller ist es besser fixe Bitanzahl (Bytes) zu verwenden:

```c
typedef signed char             int8_t;
typedef unsigned char          uint8_t;
typedef signed int              int16_t;
typedef unsigned int           uint16_t;
typedef signed long int         int32_t;
typedef unsigned long int      uint32_t;
typedef signed long long int    int64_t;
typedef unsigned long long int uint64_t;
```

Hinweis: Konstanten mit `const` sparen RAM, da `const`‑Daten im Flash liegen, normale Variablen im RAM.

## Gleitkomma‑Operationen

AVR‑Controller besitzen keine FPU (floating point unit); Gleitkommaberechnungen sind rechenintensiv und sollten vermieden werden. Strategien:

- Skalieren statt konvertieren (z. B. Millivolt statt Volt verwenden)
- Häufig reicht die Arbeit mit Rohwerten (z. B. ADC‑Counts 0–1023)
- Divisionen durch Schiebebefehle ersetzen: Mittelwerte über eine Anzahl von Messungen bilden, die eine Zweierpotenz ist (2, 4, 8, 16, …); anschließend rechts schieben.

Beispiel: Mittelwert über 8 ADC‑Messungen (Division durch 8 via Rechts‑Shift `>> 3`):

```c
uint8_t i = 0;
uint16_t average, sum = 0;
uint16_t ADC_val = 0;
for (i = 0; i < 8; i++) {
    // Konversion starten ...
    // 10‑Bit‑ADC‑Wert (High + Low) in 16‑Bit‑Variable ablegen
    ADC_val = (uint16_t)(ADCL + (ADCH << 8));
    sum += ADC_val;
}
average = sum >> 3; // 3 Bit rechts: Division durch 8
```
