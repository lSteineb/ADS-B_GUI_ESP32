#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include "credentials.h"
#include "ADSB_ILI9488.h"
#include "ADSB_Aircraft.h"
#include "ADSB_UI.h"
#include <string>
#include <exception>
#include <vector>

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;

// IP address of raspberry pi with dump1090 image
const char* serverName = "http://192.168.178.70/dump1090/data/aircraft.json";

// The following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

Display display;
DynamicJsonDocument doc(16384);

WiFiClient client;
HTTPClient http;

std::vector<Aircraft> planes;

void setup() {
  display.init();
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
  //Send an HTTP GET request every 5 seconds
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      httpGETRequest(serverName);

      double lat;
      double lon;
      JsonArray aircrafts = doc["aircraft"];

      // Looping through every aircraft in JSON object
      for (auto aircraft : aircrafts) {
        // Get Aircraft ICAO Hex Address
        try {
          int icao_hex = aircraft["hex"].as<unsigned int>();
        } catch (...) {
          Serial.println("Hex Parse failed");
        }


        // ########################################################################################
        // # If location data exists extract it
        // ########################################################################################
        if (aircraft.containsKey("seen_pos") && (aircraft["seen_pos"].as<unsigned int>() < 10)) {
          // Get Altitude, 0 if aircraft in ground
          ;
            /*
            // Put location data into queue
            outQ.put( ['L',
                      icao_hex,
                      aircraft['alt'],
                      aircraft['lat'],
                      aircraft['lon'],
                      aircraft['seen_pos']
                      ]                      
                    );

            */
        }

        // ########################################################################################
        // # If vector data available extract it
        // ########################################################################################
        if (aircraft.containsKey("vert_rate") && aircraft.containsKey("track")) {
          ;
          /*
            outQ.put( ['V',
                       icao_hex,
                       aircraft['gs'],
                       aircraft['track'],
                       0,
                       aircraft['seen']
                      ]                      
                     )
            */
        }

        lat = double(aircraft["lat"]);
        lon = double(aircraft["lon"]);

        if (!isnan(lat) && !isnan(lon)) {
          float x, y;
          std::pair<float, float> tmp = getXY(lat, lon);
          x = tmp.first;
          y = tmp.second;

          display.drawRhomb(x, y, 6, TFT_GREEN);
        }
      }
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

void httpGETRequest(const char* serverName) {
  http.useHTTP10(true);
  http.begin(client, serverName);
  // Send HTTP GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    DeserializationError error = deserializeJson(doc, http.getStream());
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}
