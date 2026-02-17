# Erstes Programm – „Hello World“

Zum Programmieren von AVR Mikrocontrollern genügt bei einem Arduino ein Texteditor und eine Toolchain (die das C-Programm übersetzt in ein HEX-File konvertiert und dann auf den Microcontroller ladet - siehe Anhang). Komfortabler ist Visual Studio Code mit dem Plugin "PlatformIO": eine leistungsfähige IDE, in der man C oder Assembler schreiben und Programme simulieren und debuggen (Breakpoints, Variablenüberwachung, Live-Debugging via GDB).

Auch die Arduino‑IDE kann für „normales“ C genutzt werden: Der folgende Code lässt sich direkt kompilieren und auf den Mikrocontroller laden – ganz ohne `setup()`/`loop()` und Arduino‑Bibliotheken.

Noch ein Hinweis - die eigentliche main Funktion ist normalerweise bei Arduino-Programmen "versteckt" und befindet sich bei PlatformIO hier: `C:\Users\<username>\.platformio\packages\framework-arduino-avr\cores\arduino\main.cpp` oder bei Nutzung der Arduino IDE hier: `C:\Users\<username>\AppData\Local\Arduino15\packages\arduino\hardware\avr\<version>\cores\arduino`.

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
