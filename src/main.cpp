#include <WiFi.h>
#include <WebServer.h>
#include <esp_now.h>

// WiFi Setup
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

void handleRoot () {
  server.send(200, "text/html", index_html);
}

void handleUnlockPress () {
  Serial.println("Unlock!");
  server.send(200, "text/plain", "Button press received");
}

void handleResetPress() {
  Serial.println("Reset!");
  server.send(200, "text/plain", "Button press received");
}

void setupWiFi() {
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.localIP());
}

void onDataReceive () {

}

void setupESPNOW() {
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(onDataReceive));
}

void setupServer() {
  server.on("/", handleRoot);
  server.on("/unlock-press", handleUnlockPress);
  server.on("/reset-press", handleResetPress);
  server.begin();
  Serial.println("Web server started.");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  setupWiFi();
  setupESPNOW();
  setupServer();
}

void loop() {
  server.handleClient();
}