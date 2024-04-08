#include <TinyGPSPlus.h>
#include <Adafruit_DotStar.h>

#define NUMPIXELS 70 // Adjust to the number of LEDs in your strip
#define DATAPIN 5
#define CLOCKPIN 6

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 4, 6);

  strip.begin(); // Initialize the DotStar LED strip
  strip.show();  // Turn off all pixels initially

  Serial.println("LED SPEED");
  delay(10000);
  
}

void loop() {
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    if (gps.encode(c)) {
      if (gps.speed.isUpdated()) {
        Serial.print("Speed (km/h): ");
        Serial.println(gps.speed.kmph());
        updateLEDs(gps.speed.kmph());
        //updateLEDs(10);
      }
    }
  }
}

void updateLEDs(double speed) {
  int delayTime = 118.8 / speed; // Converts speed (given in kilometers per hour, km/h) to delay (in milliseconds) for a known distance of 33 millimeters (mm).
  //Serial.println(delayTime);
  //delayTime = delayTime - elapsedTime;

  // Create a moving rainbow pattern
  static int rainbowOffset = 0;
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, Wheel((i + rainbowOffset) & 255));
  }
  strip.show();
  rainbowOffset++;

  delay(delayTime); // Delay based on speed
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
