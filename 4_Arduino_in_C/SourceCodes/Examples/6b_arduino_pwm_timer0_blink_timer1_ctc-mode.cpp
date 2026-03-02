/**
 * @file 6b_arduino_pwm_timer0_blink_timer1_ctc-mode.cpp
 * @brief Arduino-basiertes Embedded-Systems-Projekt mit dualen Timer-Modi
 * 
 * @details
 * Dieses Programm demonstriert die gleichzeitige Nutzung zweier Timer auf einem
 * AVR-Mikrocontroller (Arduino) mit unterschiedlichen Betriebsmodi:
 * 
 * **Funktionsübersicht:**
 * - **Timer0 (PWM-Modus)**: Erzeugt ein Pulsweiten-Moduliertes Signal mit 50% 
 *   Duty Cycle auf Pin PD6. Dies ist typischerweise für die Steuerung von 
 *   Leuchtkraft oder Motorgeschwindigkeit geeignet.
 * 
 * - **Timer1 (CTC-Modus mit Polling)**: Erzeugt zeitgenaue Interrupt-ähnliche 
 *   Events durch zyklisches Abfragen (Polling) des Compare Match Flags. 
 *   Damit wird eine LED auf Pin PB5 mit einer Blinkfrequenz von 1 Hz 
 *   (0,5s an/aus) gesteuert.
 * 
 * **Praktische Anwendung:**
 * Dieses Beispiel zeigt, wie man auf einem einzelnen Mikrocontroller 
 * mehrere zeitabhängige Prozesse parallel verwaltet:
 * - Analoge Signalsteuerung (PWM)
 * - Digitale Pulserzeugung (Timer-basiertes Polling)
 * - Ressourcenschonende Implementierung ohne Interrupts
 * 
 * @hardware
 * - Arduino/ATmega328P Mikrocontroller (16 MHz Quarzoszillator)
 * - LED an Pin PB5 (Arduino Pin 13)
 * - PWM-Ausgang an Pin PD6 (Arduino Pin 6)
 * 
 * @author Embedded Systems Course
 * @version 1.0
 */

#include <avr/io.h>

// Funktion zur Initialisierung von Timer0 für PWM
void init_timer0_pwm() {
    // OC0A Pin als Ausgang setzen (PD6)
    DDRD |= (1 << PD6);
    
    // Timer0: Fast PWM Mode (WGM01=1, WGM00=1)
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    
    // Non-inverting mode (COM0A1=1)
    TCCR0A |= (1 << COM0A1);
    
    // Prescaler 64 (CS01=1, CS00=1)
    TCCR0B |= (1 << CS01) | (1 << CS00);
    
    // PWM Duty Cycle 50% setzen (50% von 255 = 127)
    OCR0A = 127;
}

// Funktion zur Initialisierung von Timer1 für Polling
void init_timer1_polling() {
    // Timer1: CTC Mode (WGM12=1)
    TCCR1B |= (1 << WGM12);
    
    // Prescaler 1024 (CS12=1, CS10=1)
    TCCR1B |= (1 << CS12) | (1 << CS10);
    
    // Compare Match Wert für 0.5s (1Hz Toggle)
    // Bei 16MHz CPU: 16000000 / 1024 / 2 = 7812.5
    OCR1A = 7812;
    
    // Timer1 Counter auf 0 setzen
    TCNT1 = 0;
}

int main(void) {
    // LED Pin als Ausgang konfigurieren
    DDRB |= (1 << PB5);
    
    // Timer initialisieren
    init_timer0_pwm();
    init_timer1_polling();
    
    // Variable für LED-Zustand
    uint8_t led_state = 0;
    
    while(1) {
        // Polling: Timer1 Compare Match Flag überprüfen
        if (TIFR1 & (1 << OCF1A)) {
            // Flag zurücksetzen (durch Schreiben einer 1)
            TIFR1 |= (1 << OCF1A);
            
            // LED togglen
            if (led_state == 0) {
                PORTB |= (1 << PB5);    // LED einschalten
                led_state = 1;
            } else {
                PORTB &= ~(1 << PB5);   // LED ausschalten
                led_state = 0;
            }
        }
    }
    
    return 0;
}

/* Erklärung:

**Timer0 (PWM):**
- Fast PWM Mode mit 8-Bit Auflösung (0-255)
- Non-inverting Mode: HIGH bei Zählerstand < OCR0A
- Prescaler 64: PWM-Frequenz = 16MHz / (64 * 256) ≈ 976 Hz
- OCR0A = 127 entspricht 50% Duty Cycle
- PWM-Signal erscheint an Pin PD6 (Arduino Pin 6)

**Timer1 (LED-Blinken):**
- CTC Mode: Timer zählt bis OCR1A und springt dann auf 0
- Prescaler 1024 für langsame Zählfrequenz
- OCR1A = 7812: Bei 16MHz/1024 = 15625 Hz → 7812 Ticks = 0.5s
- Polling des OCF1A Flags in der Hauptschleife
- LED togglet alle 0.5s → Blinkfrequenz 1Hz

**Alternative Toggle-Implementierung (kürzer):**
```cpp
// Statt if-else:
PORTB ^= (1 << PB5);  // XOR togglet das Bit
```

**Berechnungen:**
- PWM-Frequenz: f_PWM = f_CPU / (Prescaler * 256) = 16MHz / (64 * 256) = 976 Hz
- Timer1 Periode: T = OCR1A / (f_CPU / Prescaler) = 7812 / (16MHz / 1024) = 0.5s
- Blinkfrequenz: f_blink = 1 / (2 * T) = 1 / 1s = 1 Hz

*/