# Erstes Programm – „Hello World“

Zum Programmieren von Microchip (ehemals AVR) Mikrocontrollern genügt bei einem Arduino  ein Texteditor und eine Toolchain (siehe Anhang). Komfortabler ist Atmel Studio: eine leistungsfähige IDE, in der man C oder Assembler schreiben und Programme simulieren kann (Register‑ und Speichereinblicke inklusive).

Auch die Arduino‑IDE kann für „normales“ C genutzt werden: Der folgende Code lässt sich direkt kompilieren und auf den Mikrocontroller laden – ganz ohne `setup()`/`loop()` und Arduino‑Bibliotheken.

Das „Hello World“ der Mikrocontroller‑Programmierung ist eine blinkende LED. Hier die einfache Version mit Kommentaren:

```c
/*
 * File:     blinking_led_delay.c
 * Created:  25.09.2017
 * Author:   Philipp Jahn
 *
 * Blinking LED with Delay
 * Problem mit Delay -> Prozessor ist waehrend des Wartens blockiert!
 */ 

// benoetigt für Delay‑Funktion (abhaengig von Taktfrequenz)
#define F_CPU 16000000UL    // UL kennzeichnet eine unsigned long Konstante

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  DDRB |= (1 << PB5);       // PB5 als Ausgang
  PORTB |= (1 << PB5);      // LED an

  while (1)
  {
    PORTB ^= (1 << PB5);    // LED ein/aus toggeln
    _delay_ms(500);         // CPU 500 ms blockiert
  }

  return 0;                 // wird nie erreicht (Endlosschleife)
}
```
