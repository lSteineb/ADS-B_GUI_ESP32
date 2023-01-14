#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Arduino.h>
#include "credentials.h"
#include "ADSB_ILI9488.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;

// iP address of raspberry pi with dump1090 image
const char* serverName = "http://192.168.178.70/dump1090/data/aircraft.json";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 60000;

String readings;
float readingsArr[3];
Display d;

void setup() {
  d.drawWarning();

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  d.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    d.print(".");
  }
  d.println(" ");
  d.print("Connected to WiFi network with IP Address: ");
  d.println(WiFi.localIP());
  d.println(" ");
  d.println("Timer set to 60s, it will take 60 seconds before publishing the first reading.");
  d.println(" ");
}

void loop() {
  //Send an HTTP POST request every 5 seconds
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {

      Serial.println("");
      Serial.println("##############################################################################################");
      Serial.println("#                                          NEXT MESSAGE                                      #");
      Serial.println("##############################################################################################");
      Serial.println("");

      readings = httpGETRequest(serverName);
      Serial.println(readings);
      JSONVar myObject = JSON.parse(readings);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      Serial.print("JSON object = ");
      Serial.println(myObject);

      // myObject.keys() can be used to get an array of all the keys in the object
      JSONVar keys = myObject.keys();

      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject[keys[i]];
        Serial.print(keys[i]);
        Serial.print(" = ");
        Serial.println(value);
        readingsArr[i] = double(value);
      }
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  // Send HTTP GET request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}