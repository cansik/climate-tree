#include <Arduino.h>

// NodeMCU Digital Pins
#define D0  16
#define D1  5  // I2C Bus SCL (clock)
#define D2  4  // I2C Bus SDA (data)
#define D3  0
#define D4  2  // Same as "LED_BUILTIN", but inverted logic
#define D5  14 // SPI Bus SCK (clock)
#define D6  12 // SPI Bus MISO
#define D7  13 // SPI Bus MOSI
#define D8  15 // SPI Bus SS (CS)
#define D9  3  // RX0 (Serial console)
#define D10 1  // TX0 (Serial console)

#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <DHT.h>
#include <FastLED.h>
#include "LEDRing.h"
#include "MicroUtils.h"

#define UPDATES_PER_SECOND 100

#define NUM_LEDS 30
#define DATA_PIN D6

#define DHT_PIN D1
#define DHT_TYPE DHT22

LEDRing leds = LEDRing(NUM_LEDS);
DHT dht = DHT(DHT_PIN, DHT_TYPE);

CHSV tempColor = fromHSV(255, 100, 100);

void setup() {
    Serial.begin(9600);
    Serial.println("Climate Tree");

    dht.begin();

    leds.setup<DATA_PIN>();
}

void loop() {
    delay(2000);

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");

    // set color
    

    leds.all(tempColor);

    // write leds
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}