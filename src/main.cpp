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
#include "Timer.h"
#include "EasingFloat.h"

#define UPDATES_PER_SECOND 100

#define NUM_LEDS 30
#define DATA_PIN D6

#define DHT_UPDATE_RATE 300
#define DHT_PIN D1
#define DHT_TYPE DHT22

#define RANGE_MIN (0.0)
#define RANGE_MAX 30.0
#define RANGE_ADJUSTMENT (-10.0)

#define HUE_MIN 0
#define HUE_MAX 255

LEDRing leds = LEDRing(NUM_LEDS);
DHT dht = DHT(DHT_PIN, DHT_TYPE);

CHSV tempColor = fromHSV(0, 0, 100);
Timer timer = Timer(DHT_UPDATE_RATE);

uint8_t colorIndex = 0;

EasingFloat hueValue = EasingFloat();
EasingFloat satValue = EasingFloat();

double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void showRainbow(uint8_t colorIndex) {
    uint8_t brightness = 255;

    for (int i = 0; i < NUM_LEDS; i++) {
        leds.leds[i] = ColorFromPalette(RainbowColors_p, colorIndex, brightness, LINEARBLEND);
        colorIndex += 3;
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println("Climate Tree");

    dht.begin();

    leds.setup<DATA_PIN>();

    satValue.set(255);
}

void loop() {
    if (timer.elapsed()) {
        float h = dht.readHumidity();
        float t = dht.readTemperature();

        if (isnan(h) || isnan(t)) {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        float hic = dht.computeHeatIndex(t, h, false);
        // set color
        auto hue = min(HUE_MAX, max(HUE_MIN, mapf(hic, RANGE_MIN, RANGE_MAX, HUE_MIN, HUE_MAX)));
        hueValue.set(static_cast<float>(hue + RANGE_ADJUSTMENT));

        Serial.print("Color: ");
        Serial.print(hue);
        Serial.print(" Humidity: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.print(" *C ");
        Serial.print("Heat index: ");
        Serial.print(hic);
        Serial.print(" *C \n");
    }

    // write leds
    // overwrite with rainbow
    if (hueValue.getValue() > 254.0) {
        showRainbow(colorIndex);
    } else {
        leds.all(tempColor);
    }

    // update
    hueValue.update();
    satValue.update();

    tempColor.hue = static_cast<uint8_t>(round(hueValue.getValue()));
    tempColor.sat = static_cast<uint8_t>(satValue.getValue());

    colorIndex = static_cast<uint8_t>((colorIndex + 1) % 255);

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}