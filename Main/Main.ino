#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <string>
#include <exception>
#include <vector>
#include <map>

#include "Credentials.h"
#include "ILI9488.h"
#include "Aircraft.h"
#include "UI.h"


//########################################################################################
//# Globals
//########################################################################################
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;

// IP address of raspberry pi with dump1090 image
//const char* serverName = "http://192.168.178.70/dump1090/data/aircraft.json";
const char* serverName = "http://192.168.43.15/dump1090/data/aircraft.json";

// The following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 1000;

Display display;
DynamicJsonDocument doc(49152);

WiFiClient client;
HTTPClient http;

JsonArray aircrafts;
std::map<std::string, Aircraft> planes;

mydata_t my;

#pragma region Jsondata
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
#pragma endregion Jsondata

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


      //###################
      // Updating aircraft
      //###################
      Serial.println(planes.size());
      Serial.println("-----------\n");
      int now = round(millis() / 1000);
      for (auto& p : planes) {
        if (now - p.second.getLastSeen() > 10) {
          Serial.println("TIMEOUT");
          p.second.erase();
          planes.erase(p.first);
        } else {
          p.second.update();
        }
      }

      
      //##################
      // Fetching Data
      //##################
      // Clears the Json document and fetches new data
      doc.clear();
      httpGETRequest(serverName);

      aircrafts = doc["aircraft"];

      
      //######################
      // Generating next data
      //######################
      // Looping through every aircraft in JSON object
      for (auto aircraft : aircrafts) {
        // If location data available extract it
        if (aircraft.containsKey("lat") && aircraft.containsKey("lon")) {

          std::string hex = aircraft["hex"].as<std::string>();
          std::string flight = aircraft["flight"].as<std::string>();
          float lat = aircraft["lat"].as<float>();
          float lon = aircraft["lon"].as<float>();
          float seen_pos = aircraft["seen_pos"].as<float>();
          int altitude = aircraft["altitude"].as<int>();
          int squawk = aircraft["squawk"].as<int>();
          int vert_rate = aircraft["vert_rate"].as<int>();
          unsigned int speed = aircraft["speed"].as<unsigned int>();
          unsigned int track = aircraft["track"].as<unsigned int>();

          // create location and vectordata from airplane data
          location_t l = { altitude, lat, lon };
          vector_t v = { speed, track, vert_rate };

          // Check if the icao_hex already is inside the map
          if (planes.find(hex) == planes.end()) {
            // If not: Create a new airplane object and add it to 
            Aircraft a(l);
            a.setVector(v);
            // Adds a new airplane with the current icao_hex to the map
            planes[hex] = a;
          } else {
            // If it exists, update location and vectordata
            planes[hex].setLocation(l);
            planes[hex].setVector(v);
          }
        }
      }




    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}


bool printable(float& x, float& y) {
  return (pow((x - TFT_X_CENTER), 2) + pow((y - TFT_Y_CENTER), 2) <= pow(TFT_Y_CENTER - AIRCRAFT_SIZE, 2));
}

bool printableLbl(float& x, float& y) {
  return (pow((x + 15 - TFT_X_CENTER), 2) + pow((y + 15 - TFT_Y_CENTER), 2) <= pow(TFT_Y_CENTER - (AIRCRAFT_SIZE + 15), 2));
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
