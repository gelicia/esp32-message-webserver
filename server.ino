#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

WebServer webServer(80);
StaticJsonDocument<500> jsonDocument;

const char* ssid = "";
const char* password = "";

#define RGB_BRIGHTNESS 10

#define LED_PIN 4

const int LEDsH = 8; // edit with your dimensions
const int LEDsW = 32;

Adafruit_NeoPixel pixels(LEDsH*LEDsW, LED_PIN, NEO_GRB + NEO_KHZ800);

String displayString = "hello";
uint32_t displayColor = pixels.Color(0,0,25);

#include "ledUtil.h"
#include "webUtil.h"

void setup() {
  Serial.begin(115200);

  neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS,0,0); // red

  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to %s\n", ssid);

  WiFi.disconnect(true, true);
  WiFi.begin(ssid, password);
  uint8_t wifiAttempts = 0;
  static uint8_t ledState = 0;

  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    Serial.print(".");
    ledState = ledState == 0 ? 1 : 0;
    neopixelWrite(RGB_BUILTIN, ledState == 0 ? RGB_BRIGHTNESS : 0,0,0); // Red blink onboard LED
    delay(1000);
    if(wifiAttempts == 10) {
      WiFi.disconnect(true, true);//Switch off the wifi on making 10 attempts and start again.
      WiFi.begin(ssid, password);
    }
    wifiAttempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    neopixelWrite(RGB_BUILTIN, 0,RGB_BRIGHTNESS,0); // green
    WiFi.setAutoReconnect(true);
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
  } else {
    WiFi.disconnect(true, true);
  }
  delay(100);
  pixels.begin();
  setupWeb();
}

void loop()
{
  handleWeb();
  pixels.clear();
  displayTextOnPanel(displayString, displayColor);
  delay(50);
}
