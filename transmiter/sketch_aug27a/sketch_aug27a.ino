#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int xPin = A0;
const int yPin = A1;
const int threshold = 200;

RF24 radio(7, 8);
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

  String direction = getDirection(xValue, yValue);

  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" Y: ");
  Serial.print(yValue);
  Serial.print(" Direction: ");
  Serial.println(direction);

  int data[2] = {xValue, yValue};
  radio.write(&data, sizeof(data));

  delay(100);
}

String getDirection(int xValue, int yValue) {
  if (abs(xValue - 512) < threshold && abs(yValue - 512) < threshold) {
    return "Centered";
  }

  if (xValue < 512 - threshold) {
    return "Left";
  }
  if (xValue > 512 + threshold) {
    return "Right";
  }
  if (yValue < 512 - threshold) {
    return "Forward";
  }
  if (yValue > 512 + threshold) {
    return "Backward";
  }

  return "Centered";
}
