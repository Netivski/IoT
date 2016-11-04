#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>

#define NUM_LEDS 30
#define DATA_PIN 4
#define CLOCK_PIN 5

CRGB leds[NUM_LEDS];
ESP8266WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(81);

const char* ssid = "ssid";
const char* password = "password";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED: {
      break;
    }
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.print("IP Address: ");
      Serial.printf("Connected from %d.%d.%d.%d url: %s\n", ip[0], ip[1], ip[2], ip[3], payload);
      break;
    }
    case WStype_TEXT: {
      String text = String((char *) &payload[0]);
      if (text == "RED") {
        fill_solid(leds, NUM_LEDS, CRGB::Red); 
        FastLED.show();
      }
      else if (text == "GREEN") {
        fill_solid(leds, NUM_LEDS, CRGB::Green); 
        FastLED.show();
      }
      else if (text == "BLUE") {
        fill_solid(leds, NUM_LEDS, CRGB::Blue); 
        FastLED.show();
      }
      else {
        String col = text;
        Serial.print("Received: " + col + "\n");
        int red = col.substring(0, col.indexOf(",")).toInt();
        int green = col.substring(col.indexOf(",")+1, col.lastIndexOf(",")).toInt();
        int blue = col.substring(col.lastIndexOf(",")+1).toInt();
        Serial.printf("Color is: %d,%d,%d\n",red, green,blue);
        fill_solid(leds, NUM_LEDS, CRGB(green, red, blue));
        FastLED.show();
      }
      break;
    }
  }
}

void setup() {

  delay(3000); // power-up safety delay
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BRG>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  Serial.begin(9600);
  Serial.println("Connecting...");
  WiFiMulti.addAP(ssid, password);

  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
    Serial.println("Attempting to connect...");
  }
  
  Serial.print("Connected to: ");
  Serial.println(WiFi.localIP());
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}
