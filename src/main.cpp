/*
Speaker Test
19 (Blue) --> SU (Shutdown, not essential)
20 (Purple) --> GAIN (Affects the decibel gain)
21 (Grey) --> DIN (Data In)
47 (White) --> BCLK (Board Clock)
48 (Black) --> LRC (Left/Right Clock)
*/
#include "Arduino.h"
#include "Audio.h"
#include "WiFi.h"

#define I2C_DOUT 21
#define I2C_BCLK 47
#define I2C_LRC 48
Audio audio;
String ssid = "early worm gets the worm";
String password = "biarcepet";

void setup() {
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  delay(1500);

  Serial.begin(115200);
  audio.setPinout(I2C_BCLK, I2C_LRC, I2C_DOUT);
  audio.setVolume(50);
  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Pink-Panther.wav");
  // audio.connecttoFS(SPIFFS, "/buzzer.wav");  
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Looped.");
  audio.loop();
  delay(2000);
}