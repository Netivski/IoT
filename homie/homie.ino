#include <Homie.h>
#include "DHT.h"

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

const int LEDPIN = 5;
const int DHTPIN = 0;
float tempInCelsius;
float humidity;
DHT dht(DHTPIN, DHTTYPE);

HomieNode lightNode("light", "switch");
HomieNode tempNode("temp", "temperature");

bool lightOnHandler(String value) {

  Serial.println(value);
  
  if (value == "true") {
    digitalWrite(LEDPIN, HIGH);
    Homie.setNodeProperty(lightNode, "on", "true"); // Update the state of the light
    Serial.println("Light is on");
  } else if (value == "false") {
    digitalWrite(LEDPIN, LOW);
    Homie.setNodeProperty(lightNode, "on", "false");
    Serial.println("Light is off");
  } else {
    return false;
  }
  return true;
}

bool readTemp() {
  humidity = dht.readHumidity();
  tempInCelsius = dht.readTemperature();
  if (isnan(humidity) || isnan(tempInCelsius)) {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }
  return true;
}

void setup() {
  Serial.begin(9600);
      
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  lightNode.subscribe("on", lightOnHandler);
  Homie.registerNode(lightNode);

  dht.begin();
  Homie.registerNode(tempNode);  

  Homie.setup();
}

void loop() {
  delay(100);
  if (readTemp()) {
    Homie.setNodeProperty(tempNode, "temperature", String(tempInCelsius), true);
    Homie.setNodeProperty(tempNode, "humidity", String(humidity), true);
  }
  Homie.loop();
}

