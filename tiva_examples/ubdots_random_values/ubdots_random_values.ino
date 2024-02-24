/*
#include <UbiBuilder.h>
#include <UbiConstants.h>
#include <Ubidots.h>
#include <UbiHttp.h>
#include <UbiProtocol.h>
#include <UbiProtocolHandler.h>
#include <UbiTcp.h>
#include <UbiTypes.h>
#include <UbiUdp.h>
#include <UbiUtils.h>
*/

// This example sends data to multiple variables to
// Ubidots through HTTP protocol.

/****************************************
* Include Libraries
****************************************/

#include <Ubidots.h>

/****************************************
* Define Instances and Constants
****************************************/
const char* UBIDOTS_TOKEN = "BBUS-qz5mUew5QAprHbKXDdH8fTr5xYcXfh"; // Put here your Ubidots TOKEN
const char* WIFI_SSID = "WR60"; // Put here your Wi-Fi SSID
const char* WIFI_PASS = "981212100"; // Put here your Wi-Fi password

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

/****************************************
* Auxiliar Functions
****************************************/

// Put here your auxiliar functions

/****************************************
* Main Functions
****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  // client.setDeviceLabel("tm4c1294xl");
  ubidots.setDebug(true);  // Uncomment this line for printing debug messages
}

void loop() {
  float value1 = random(100);
  // float value2 = random(0, 9) * 100; 
  // float value3 = random(0, 9) * 1000;
  ubidots.add("Variable_Name_One", value1); // Change for your variable name
  // ubidots.add("Variable_Name_Two", value2);
  // ubidots.add("Variable_Name_Three", value3);
  bool bufferSent = false;
  bufferSent = ubidots.send(); // Will send data to a device label that matches the device Id
  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  delay(5000);
}
