#include <Adafruit_NeoPixel.h>

#define PIN 7
#define NUM_LEDS 63

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int pos = 0; // Start position of the LED
int direction = 1; // Direction of movement: 1 for forward, -1 for backward

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  strip.clear(); // Turn off all pixels
  strip.setPixelColor(pos, strip.Color(150, 0, 0)); // Set the current pixel to red
  strip.show();

  delay(30); // Speed of movement

  pos += direction; // Move the position for next loop

  // Change direction if we hit the end
  if (pos >= (NUM_LEDS - 1) || pos <= 0) {
    direction *= -1;
  }
}
