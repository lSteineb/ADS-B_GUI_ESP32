#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <string>
#include <exception>
#include <vector>
#include "credentials.h"
#include "ILI9488.h"
#include "Aircraft.h"
#include "UI.h"


//########################################################################################
//# Globals
//########################################################################################
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
DynamicJsonDocument doc(24576);

WiFiClient client;
HTTPClient http;

std::vector<Aircraft> planes;

mydata_t my;

/* 
########################################################################################
# JSON arguments explained
########################################################################################
#################################################################################################################################################
-------------------------------------------------------------------------------------------------------------------------------------------------
now       : the time this file was generated, in seconds since Jan 1 1970 00:00:00 GMT (the Unix epoch)
-------------------------------------------------------------------------------------------------------------------------------------------------
messages  : the total number of Mode S messages processed since dump1090 started
-------------------------------------------------------------------------------------------------------------------------------------------------
aircraft  : an array of JSON objects, one per known aircraft. Each aircraft has the following keys. Keys will be omitted if data is not available
-------------------------------------------------------------------------------------------------------------------------------------------------
hex       : the 24-bit ICAO identifier of the aircraft, as 6 hex digits
squak     : the 4-digit squawk code (octal representation)
flight    : the flight name / callsign
lat       : latitude of the aircrafts position in decimal degrees
lon       : longitude of the aircrafts position in decimal degrees
nucp      : the NUCp (navigational uncertainty category) reported for the position
seen_pos  : how long ago (in seconds before "now") the position was last updated
altitude  : the aircraft altitude in feet, or "ground" / "0" if it is reporting it is on the ground
vert_rate : vertical rate in feet/minute
track     : true track over ground in degrees (0-359)
speed     : reported speed in kt. This is usually speed over ground, but might be IAS(indicated air speed)
category  : emitter category to identify particular aircraft or vehicle classes (values A0 - D7)
mlat      : list of fields derived from MLAT data (not aplicable here)
tisb      : list of fields derived from TIS-B data (not aplicable here)
messages  : total number of Mode S messages received from this aircraft
seen      : how long ago (in seconds before "now") a message was last received from this aircraft
rssi      : recent average RSSI (signal power), in dbFS; this will always be negative
-------------------------------------------------------------------------------------------------------------------------------------------------
#################################################################################################################################################
*/


//########################################################################################
//# Functions
//########################################################################################
void setup() {
  display.init();
  display.setTextSize(2);

  Serial.begin(115200);
  my.location = myloc;

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
        // # If a new Airplane has been captured
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


        // ########################################################################################
        // # If location data available extract it
        // ########################################################################################
        if (aircraft.containsKey("lat") && aircraft.containsKey("lon")) {
          location_t l = { int(aircraft["altitude"]), float(aircraft["lat"]), float(aircraft["lon"]) };
          //Aircraft currentAircraft(l);
          //planes.push_back(currentAircraft);
          /*
          if (!isnan(lat) && !isnan(lon)) {
            float x, y;
            std::pair<float, float> tmp = getXY(lat, lon);
            x = tmp.first;
            y = tmp.second;
            double posDistance = round(dF * (TFT_WIDTH/2) / 25);
            if(posDistance < TFT_DRAWABLE)
            display.drawCircle(x, y, 6, TFT_GREEN);
          }
          */
        }
      }


    } else {
      Serial.println("WiFi Disconnected");
    }
    doc.clear();
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
      Serial.print("Deserialization failed: ");
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
