// MASTER BUZZER (YELLOW)

/*
ESP32 Connections
> MAX98357 Audio
1 --> LRC (Left/Right Clock)
2 --> BCLK (Board Clock)
42 --> DIN (Data In)
41 --> GAIN (Affects the decibel gain)
40 --> SU (Shutdown, not essential)

> Big Dome Button
4 --> Switch
10 --> LED
*/

// Libraries
#include <Arduino.h>
#include <Audio.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <WebServer.h>
#include <esp_now.h>

typedef struct struct_message {
  char x;
} struct_message;
struct_message outData;
struct_message inData;

esp_now_peer_info_t peerInfo;

//MAC Addresses of Receivers, self is commented out
uint8_t blue[] = {0xE4, 0xB0, 0x63, 0xB9, 0xDB, 0x98}; // Blue
uint8_t green[] = {0xE4, 0xB0, 0x63, 0xB9, 0xDA, 0x5C}; //  Green
// uint8_t yellow[] = {0xE4, 0xB0, 0x63, 0xB3, 0xF5, 0xDC}; //  Yellow
uint8_t red[] = {0xE4, 0xB0, 0x63, 0xB3, 0xFA, 0x24}; // Red
uint8_t white1[] = {0xE4, 0xB0, 0x63, 0xB3, 0xA2, 0xBC}; // White1
uint8_t white2[] = {0xE4, 0xB0, 0x63, 0xB9, 0xDB, 0x88}; // White2

// Button Setup
const int buttonPin = 4;
const int ledPin = 10;
int prevButtonState;
int currentButtonState;
bool buttonLocked = false;
bool someonePressed = false;
bool audioIsPlaying = false;

// Audio Setup
#define I2C_DOUT 42
#define I2C_BCLK 2
#define I2C_LRC 1
Audio audio;

// Web Server Setup
const char* ssid = "BuzzerReset";
const char* password = "iceicebaby";
WebServer server(80);
const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
  <html>

    <head>
      <title>
        Buzzer Reset
      </title>

      <meta name="viewport" content="width=device-width, initial-scale=1">

      <style>
        .button {
          width: 75%;
          padding: 15px 32px;
        }
        .reset {
          background-color: #98f59c;
        }
      </style>

      <script>
        function resetPress() {
          fetch('/reset-press')
        }
        function unlockPress() {
          fetch('/unlock-press')
        }
      </script>
    </head>

    <body>
      <br>
      <br>
      <p>
        <button class="button" onclick="unlockPress()">
          UNLOCK
        </button>
      </p>
      <br>
      <br>
      <p>
        <button class="button reset" onclick="resetPress()">
          RESET
        </button>
      </p>
    </body>

  </html>
)rawliteral";

void onDataReceive (const uint8_t * mac, const uint8_t *incomingData, int len) {
  Serial.println("Message received!");
  Serial.println(inData.x);
  memcpy(&inData, incomingData, sizeof(inData));

  char msg = inData.x;
  if (msg == 'l') { // Locked when someone else presses first
    someonePressed = true;
    buttonLocked = true;
    digitalWrite(ledPin, LOW);
  } else if (msg == 'r') { // Reset, buttons inactive until unlocked
    someonePressed = false;
    buttonLocked = true;
    digitalWrite(ledPin, LOW);
  } else if (msg == 'u') { //  Unlock, all buttons active
    buttonLocked = false;
    someonePressed = false;
    digitalWrite(ledPin, HIGH);
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void startESPNOW() {
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(esp_now_recv_cb_t(onDataReceive));
}

void registerPeers() {
  //Add peers
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // Register receivers, comment out self
  memcpy(peerInfo.peer_addr, blue, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add Blue");
    return;
  }
  memcpy(peerInfo.peer_addr, green, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add Green");
    return;
  }
  // memcpy(peerInfo.peer_addr, yellow, 6);
  // if (esp_now_add_peer(&peerInfo) != ESP_OK){
  //   Serial.println("Failed to add Yellow");
  //   return;
  // }
  memcpy(peerInfo.peer_addr, red, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add Red");
    return;
  }
  memcpy(peerInfo.peer_addr, white2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add White2");
    return;
    memcpy(peerInfo.peer_addr, white1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add White1");
    return;
  }
  }
}

void sendMessage(char message) {
  outData.x = message;
  Serial.println(outData.x);
  esp_err_t result = esp_now_send(0, (uint8_t*) &outData, sizeof(outData));
  if (result == ESP_OK) {
    Serial.println("Message sent successfully");
  } else {
    Serial.println("Error sending message");
  }
}

void handleRoot () {
  server.send(200, "text/html", index_html);
}

void handleUnlockPress () {
  Serial.println("Unlock!");
  sendMessage('u');
  buttonLocked = false;
  someonePressed = false;
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/plain", "Button press received");
}

void handleResetPress() {
  Serial.println("Reset!");
  sendMessage('r');
  someonePressed = false;
  buttonLocked = true;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "Button press received");
}

void startWiFi() {
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.localIP());
}

void startServer() {
  server.on("/", handleRoot);
  server.on("/unlock-press", handleUnlockPress);
  server.on("/reset-press", handleResetPress);
  server.begin();
  Serial.println("Web server started.");
}

void startAudio() {
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }
  audio.setPinout(I2C_BCLK, I2C_LRC, I2C_DOUT);
  audio.setVolume(21);
}

void playAudio() {
  if (!audioIsPlaying) {
    audio.connecttoFS(SPIFFS, "/buzzer.wav"); // Sound
    audioIsPlaying = true;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);

  WiFi.mode(WIFI_AP_STA);
  startWiFi();
  startESPNOW();
  registerPeers();
  startServer();
  startAudio();
}

void loop() {
  audio.loop();
  server.handleClient();
  
  currentButtonState = digitalRead(buttonPin);

  if (!buttonLocked and !someonePressed) {
    if (prevButtonState == LOW && currentButtonState == HIGH) { // First one to press
      digitalWrite(ledPin, HIGH); // Lights on
      playAudio(); // Buzzer noise
      sendMessage('l');
    }
  }

  prevButtonState = digitalRead(buttonPin);
}

// Resets audio file
void audio_eof_mp3(const char *info){
  audioIsPlaying = false;
}