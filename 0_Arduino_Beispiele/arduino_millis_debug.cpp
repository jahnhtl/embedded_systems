#include <Arduino.h>
#include "avr8-stub.h"
#include "app_api.h"

const int LED_PIN = 13;
const unsigned long BLINK_INTERVAL = 500; // milliseconds

unsigned long previous_millis = 0;
bool ledState = LOW;

void setup() {
  debug_init(); // Initialize debugging
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  unsigned long elapsedMillis = currentMillis - previous_millis;

  if (elapsedMillis >= BLINK_INTERVAL) {
    previous_millis = currentMillis;

    ledState = !ledState;
    
    digitalWrite(LED_PIN, ledState);
  }
}