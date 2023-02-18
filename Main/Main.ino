#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <string>
#include <exception>
#include <vector>
#include <map>

// include YOUR_CREDENTIALS.h instead after filling out your wifi access
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
const char* serverName = "http://192.168.0.103/dump1090/data/aircraft.json";

// The following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.

// Last time the program was updated (in ms)
unsigned long lastTime = 0;
// Set timer to 1 seconds (1000ms)
unsigned long timerDelay = 1000;
// Touch coordinates;
point_t touch_point;
bool pressed = false;

ILI9488 display;
UI ui;
DynamicJsonDocument doc(49152);

WiFiClient client;
HTTPClient http;

JsonArray aircrafts;
std::map<std::string, Aircraft> planes;

mydata_t my;
programdata_t prog;

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
  display.touch_calibrate();
  display.setTextSize(2);

  ui.showWarning();

  display.fillScreen(BLACK);

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
  display.println("It will take 1 second before publishing the first reading.");
  display.println(" ");

  delay(5000);
  ui.init();
}


void loop() {
  //Send an HTTP GET request every second
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {

      //###########################
      // Updating/Erasing aircraft
      //###########################
      for (auto& p : planes) {
        if (round(lastTime / 1000) - p.second.getLastSeen() > 5) {
          p.second.erase();
          planes.erase(p.first);
        } else
          p.second.update();
      }


      //###################
      // Fetching new Data
      //###################
      // Clears the Json document and fetches new data
      doc.clear();
      httpGETRequest(serverName);

      aircrafts = doc["aircraft"];


      //######################
      // Generating next data
      //######################
      // Looping through every aircraft in JSON object
      for (const auto& aircraft : aircrafts) {
        // If location data available extract it
        if (aircraft.containsKey("lat") && aircraft.containsKey("lon")) {
          const char* hex = aircraft["hex"].as<const char*>();
          const char* flight = aircraft["flight"].as<const char*>();
          const char* category = aircraft["category"].as<const char*>();
          const char* squawk = aircraft["squawk"].as<const char*>();
          float lat = aircraft["lat"].as<float>();
          float lon = aircraft["lon"].as<float>();
          float seen_pos = aircraft["seen_pos"].as<float>();
          int altitude = aircraft["altitude"].as<int>();
          signed short vert_rate = aircraft["vert_rate"].as<signed short>();
          short unsigned int speed = aircraft["speed"].as<unsigned short>();
          short unsigned int track = aircraft["track"].as<unsigned short>();

          // create location and vectordata from airplane data
          location_t l = { altitude, lat, lon };
          vector_t v = { speed, track, vert_rate };
          information_t i = { hex, squawk, flight, category };

          // Create new airplane if it does not already exist
          if (planes.find(hex) == planes.end()) {
            Aircraft a(l);
            a.setVector(v);
            a.setInformation(i);
            planes[hex] = a;
          } else {
            // If it exists, update airplane data
            planes[hex].setLocation(l);
            planes[hex].setVector(v);
            planes[hex].setInformation(i);
          }
        }
      }

    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  
  // Touch functionality
  if (display.getTouch(&touch_point.x, &touch_point.y)) {
    if (!pressed) {
      // Check if zoom buttons have been pressed
      uint8_t id = ui.processInput(touch_point);

      if (id == 0) {
        if (prog.currentRange + 10 <= MAX_RANGE)
          ui.setRange(prog.currentRange + 10);
      } else if (id == 1) {
        if (prog.currentRange - 10 >= MIN_RANGE)
          ui.setRange(prog.currentRange - 10);
      }

      // Check if airplane has been touched
      for (auto& p : planes) {
        if (p.second.checkCollision(touch_point)) {
          p.second.setSelected(true);
          p.second.displayInformation();
        }

        // Update all planes after zooming in/out
        p.second.update();
      }
      pressed = true;
    }
  } else {
    pressed = false;
  }
}

void httpGETRequest(const char* serverName) {
  http.useHTTP10(true);
  http.begin(client, serverName);
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

  http.end();
}
