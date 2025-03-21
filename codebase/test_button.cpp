#include <Arduino.h>

const int buttonPin = 4;
const int ledPin = 10;
int buttonState;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Started");
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    Serial.println(buttonState);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    Serial.println(buttonState);
  }
  delay(100);
}