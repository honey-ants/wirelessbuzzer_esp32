#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA); // Set WiFi to station mode
  Serial.println("MAC Address: "+ WiFi.macAddress()); // Print the MAC address
}

void loop() {
  // Nothing to do here
}