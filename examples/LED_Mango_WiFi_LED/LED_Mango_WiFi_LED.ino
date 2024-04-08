#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "Milanet9000";
const char* password = "oliviapup";

// Neopixel setup
#define PIN         7  // IO7 used for data
#define NUMPIXELS   30 // Number of pixels in the strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print the local IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize the strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Serve HTML web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body>"
                  "<h1>ESP32 Neopixel Color Control</h1>"
                  "<p>Select Color: <input type='color' onchange='updateColor(this.value)' id='colorPicker'></p>"
                  "<script>"
                  "function updateColor(color) {"
                  "  var xhr = new XMLHttpRequest();"
                  "  xhr.open('GET', '/setColor?color=' + color.replace('#', ''), true);"
                  "  xhr.send();"
                  "}"
                  "</script>"
                  "</body></html>";
    request->send(200, "text/html", html);
  });

  // Handle Color Change Request
  server.on("/setColor", HTTP_GET, [](AsyncWebServerRequest *request){
    String color = request->getParam("color")->value();
    setColor(strip.Color(
      strtol(color.substring(1, 3).c_str(), NULL, 16),
      strtol(color.substring(3, 5).c_str(), NULL, 16),
      strtol(color.substring(5, 7).c_str(), NULL, 16)
    ));
    request->send(200, "text/plain", "Color changed to " + color);
  });

  // Start server
  server.begin();
}

void loop() {
  // Nothing needed here
}

void setColor(uint32_t color) {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
