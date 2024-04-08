#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <Wire.h>
#include <Math.h>

#define NUMPIXELS 43
#define DATAPIN    5
#define CLOCKPIN   6
#define NUM_LIT_LEDS 5 // Number of LEDs in the moving group

// Parameters to control the moving speed and direction of the rainbow
unsigned long rainbowDelay = 100; // Delay in milliseconds between each frame
int direction = 1; // Direction of the rainbow movement: 1 for forward, -1 for backward
float offset_pitch;

Adafruit_LSM6DS3TRC lsm6ds3trc; // Create LSM6DS3 object
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

void setup(void) {
  Wire.begin(1, 10); // Initialize I2C
  Serial.begin(115200); // Begin Serial communication
  
  if (!lsm6ds3trc.begin_I2C()) { // Initialize LSM6DS3
    Serial.println("Failed to find LSM6DS3TR-C chip");
    while (1) {
      delay(10); // Loop forever if the sensor is not found
    }
  }

  strip.begin(); // Initialize DotStar LED strip
  strip.show(); // Turn off all LEDs
}

void Wheel(byte WheelPos, byte *r, byte *g, byte *b) {
  if(WheelPos < 85) {
   *r = WheelPos * 3;
   *g = 255 - WheelPos * 3;
   *b = 0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   *r = 255 - WheelPos * 3;
   *g = 0;
   *b = WheelPos * 3;
  } else {
   WheelPos -= 170;
   *r = 0;
   *g = WheelPos * 3;
   *b = 255 - WheelPos * 3;
  }
}

void rainbow(unsigned long wait) {
  static long firstPixelHue = 0;
  for(int i=0; i<strip.numPixels(); i++) {
    int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
    byte r, g, b;
    Wheel(((pixelHue / 256) & 255), &r, &g, &b);
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();   
  
  // Adjust the direction of movement
  if (direction == 1) {
      firstPixelHue += 256; // Move the rainbow forward
  } else {
      firstPixelHue -= 256; // Move the rainbow backward
  }
  
  delay(wait);
}

void loop() {
  sensors_event_t accel, gyro, temp;
  lsm6ds3trc.getEvent(&accel, &gyro, &temp); // Get the latest sensor event

  // Calculate pitch using the accelerometer data
  float pitch = atan2(accel.acceleration.y, accel.acceleration.z) * 180 / M_PI;

  // Determine direction based on pitch
  direction = pitch > 0 ? -1 : 1;
  
  // Map the absolute value of pitch for delay time
  // Normalize pitch to range 0 (vertical) to 90 (horizontal)
  float normalizedPitch = fabs(pitch);
  float pitchOffset = fabs(90 - normalizedPitch);

  //Serial.print("pitchOffset: ");
  //Serial.println(pitchOffset);
  
  // Map normalized pitch to delay: faster (1ms) at 90°, slower (100ms) as it approaches 0° or 180°
  unsigned long delayTime = map(pitchOffset, 0, 110, 1, 200);

  //Serial.print("DelayTime: ");
  //Serial.println(delayTime);

  rainbow(delayTime); // Generate the rainbow effect with dynamic speed and direction


  // Optional: Print the pitch to the Serial monitor for debugging
  //Serial.print("NormalizedPitch: ");
  //Serial.println(normalizedPitch);

  //delay(100); // Short delay before the next loop iteration
}
