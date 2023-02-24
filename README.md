# ADS-B_GUI_ESP32

This project was made in the MCT module of the applied mathematics and computer science degree of the FH Aachen.
![IMG_20230217_152207](https://user-images.githubusercontent.com/74535078/221164047-9e5d6b45-73e5-43f1-a50c-2cb5044f7efe.jpg)

It uses an ESP32 as its core to display aircraft traffic information on a tft display(ILI9488), which it gets from a 1090 MHz antenna that is conected to a raspberry pi with a [fr24 image](https://www.flightradar24.com/build-your-own).
Each modern aircraft has to have a so called ADS-B reciever/transmitter equipped which permanently sends out aircraft information. The following table shows the available information that can be received and displayed with the current setup:

| JSON argument  | description |
| :---: | :--- |
| now | the time this file was generated, in seconds since Jan 1 1970 00:00:00 GMT (the Unix epoch)  |
| --------------- | -------------------------------------------------------------------------- |
| messages | the total number of Mode S messages processed since dump1090 started  |
| --------------- | --------------------------------------------------------------------------- |
| aircraft | an array of JSON objects, one per known aircraft. Each aircraft has the following keys. Keys will be omitted if data is not available |
| --------------- | --------------------------------------------------------------------------- |
| hex       | the 24-bit ICAO identifier of the aircraft, as 6 hex digits |
| squak     | the 4-digit squawk code (octal representation)  |
| flight    | the flight name / callsign  |
| lat       | latitude of the aircrafts position in decimal degrees |
| lon       | longitude of the aircrafts position in decimal degrees |
| nucp      | the NUCp (navigational uncertainty category) reported for the position |
| seen_pos  | how long ago (in seconds before "now") the position was last updated |
| altitude  | the aircraft altitude in feet, or "ground" / "0" if it is reporting it is on the ground |
| vert_rate | vertical rate in feet/minute |
| track     | true track over ground in degrees (0-359) |
| speed     | reported speed in kt. This is usually speed over ground, but might be IAS(indicated air speed) |
| category  | emitter category to identify particular aircraft or vehicle classes (values A0 - D7)  |
| mlat      | list of fields derived from MLAT data (not aplicable here)  |
| tisb      | list of fields derived from TIS-B data (not aplicable here) |
| messages  | total number of Mode S messages received from this aircraft |
| seen      | how long ago (in seconds before "now") a message was last received from this aircraft |
| rssi      | recent average RSSI (signal power), in dbFS; this will always be negative |
