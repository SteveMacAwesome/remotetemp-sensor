#include <Arduino.h>
#include <TM1637Display.h>

#include <pinout.h>
#include <temperature.h>
#include <neopixels.h>
#include <radio.h>

// Set up 7-segment driver
TM1637Display display(LED_CLK_PIN, LED_DAT_PIN);

// Global variables
unsigned long prevMillis = 0;
float temperature = 0.0;

void setup() {
  Serial.begin(9600);

  setupTemperature();
  setupRadio();

  display.setBrightness(7);
}

void loop() {
  if (millis() > prevMillis + 10000) {
    prevMillis = millis();
    updateTemperature();
    display.showNumberDecEx(temperature * 100, 64);
  }

  updateRadio();
}
