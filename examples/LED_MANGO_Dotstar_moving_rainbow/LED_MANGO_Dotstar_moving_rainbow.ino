#include <FastLED.h>

#define NUM_LEDS 60 // Change this to reflect the number of LEDs in your strip
#define DATA_PIN 5
#define CLOCK_PIN 6
#define COLOR_ORDER BGR
#define CHIPSET DOTSTAR

CRGB leds[NUM_LEDS];

void setup() {
  // Initialize the LED strip
  FastLED.addLeds<CHIPSET, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

void loop() {
  // Fill the dots one after the other with a color
  rainbowCycle(20);
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on the wheel
    for (i = 0; i < NUM_LEDS; i++) {
      leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
    }
    FastLED.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
