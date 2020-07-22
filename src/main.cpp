#include <Arduino.h>
#include <FastLED.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <SPI.h>
#include <NRFLite.h>

// Headers for my own stuff
#include <temperature.h>

#define RADIO_ID 1;             // Our radio's id, 0 = hub
#define DESTINATION_RADIO_ID 0; // Id of the radio we will transmit to.
#define PIN_RADIO_CE 9;
#define PIN_RADIO_CSN 10;

#define ONE_WIRE_BUS 8

#define NUM_LEDS 8
#define LED_PIN 3

enum RadioPacketType {
    Heartbeat,
    BeginGetData,
    EndGetData,
    ReceiverData
};

struct RadioPacket {
    RadioPacketType PacketType;
    uint8_t FromRadioId;
    uint32_t OnTimeMillis;
};

// Set up Dallas Temperature probe
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempAddress;

// Set up LED strip
CRGB leds[NUM_LEDS];

// Set up NRF24 radio
NRFLite radio;
RadioPacket radioData;

// Global variables
int step = 0;
uint8_t hue = 0;
float temperature = 0.0;

// Forward-declare functions
void setupLeds();
void updateLeds();

void setup() {
  // Just hang out for a bit
  delay(2000);

  // Serial for debugging
  Serial.begin(9600);

  setupTemperature();
  setupLeds();
}

void loop() {
  if (step % 100 == 0) {
    updateTemperature();
  }

  updateLeds();

  step++;
  delay(20);
}

void setupLeds() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
}

void updateLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setHue(hue + ((255 / 8) * i));
  }

  FastLED.show();

  hue++;
}
