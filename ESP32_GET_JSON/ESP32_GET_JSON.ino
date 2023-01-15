#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Arduino.h>
#include "credentials.h"
#include "ADSB_ILI9488.h"
#include "ADSB_UI.h"


const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;

// iP address of raspberry pi with dump1090 image
const char* serverName = "http://192.168.178.70/dump1090/data/aircraft.json";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

String readings;
float readingsArr[3];
Display display;

void setup() {
  display.init();
  display.showWarning();
  display.setTextSize(2);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  display.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
  }
  display.println(" \n");
  display.print("Connected to WiFi network with IP Address: ");
  display.println(WiFi.localIP());
  display.println(" ");
  display.println("It will take 5 seconds before publishing the first reading.");
  display.println(" ");

  delay(3000);
  drawBaseUI(display);
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
      //Serial.println(readings);
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

      JSONVar planes = myObject[keys[2]];
      double lat;
      double lon;
      
      for (int i = 0; i < 10; ++i) {
        lat = double(planes[i]["lat"]);
        lon = double(planes[i]["lon"]);


        if (!isnan(lat) && !isnan(lon)) {
          float x, y;
          std::pair<float, float> tmp = getXY(lat, lon);
          x = tmp.first;
          y = tmp.second;

          display.drawCircle(x, y, 6, TFT_GREEN);
        }
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