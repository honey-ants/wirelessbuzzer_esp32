/*
> MAX98357 Audio
1 --> LRC (Left/Right Clock)
2 --> BCLK (Board Clock)
42 --> DIN (Data In)
41 --> GAIN (Affects the decibel gain)
40 --> SU (Shutdown, not essential)
*/
#include "Arduino.h"
#include "Audio.h"
#include "SPIFFS.h"

#define I2C_DOUT 42
#define I2C_BCLK 2
#define I2C_LRC 1
Audio audio;

void setup() {
  Serial.begin(115200);
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }
  audio.setPinout(I2C_BCLK, I2C_LRC, I2C_DOUT);
  audio.setVolume(21);

  if (SPIFFS.exists("/buzzer.wav")) {
    Serial.println("File found, playing...");
    audio.connecttoFS(SPIFFS, "/buzzer.wav");
  } else {  
    Serial.println("File not found");
  }
}

void loop() {
  audio.loop();
}

void audio_eof_mp3(const char *info) {
  Serial.println("End of file reached. Restarting playback...");
  // audio.connecttoFS(SPIFFS, "/buzzer.wav"); // Restart playback
}