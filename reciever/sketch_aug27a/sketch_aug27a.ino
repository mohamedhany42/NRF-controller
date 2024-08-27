#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);
const byte address[6] = "00001";

#define PWM1_MOTOR1 2
#define DIR1_MOTOR1 3
#define PWM2_MOTOR2 4
#define DIR2_MOTOR2 5
#define PWM1_MOTOR3 6
#define DIR1_MOTOR3 9
#define PWM2_MOTOR4 10
#define DIR2_MOTOR4 A0

void setup() {
  Serial.begin(9600);

  pinMode(DIR1_MOTOR1, OUTPUT);
  pinMode(DIR2_MOTOR2, OUTPUT);
  pinMode(DIR1_MOTOR3, OUTPUT);
  pinMode(DIR2_MOTOR4, OUTPUT);

  pinMode(PWM1_MOTOR1, OUTPUT);
  pinMode(PWM2_MOTOR2, OUTPUT);
  pinMode(PWM1_MOTOR3, OUTPUT);
  pinMode(PWM2_MOTOR4, OUTPUT);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    int data[2];
    radio.read(&data, sizeof(data));

    int xValue = data[0];
    int yValue = data[1];

    Serial.print("Received X: ");
    Serial.print(xValue);
    Serial.print(" Y: ");
    Serial.println(yValue);

    controlMotors(xValue, yValue);
  }
}

void controlMotors(int xValue, int yValue) {
  int threshold = 200;
  int speed = 255;

  if (abs(xValue - 512) < threshold && abs(yValue - 512) < threshold) {
    stopAll();
  } else if (yValue < 512 - threshold) {
    forward(speed);
  } else if (yValue > 512 + threshold) {
    backward(speed);
  } else if (xValue > 512 + threshold) {
    right(speed);
  } else if (xValue < 512 - threshold) {
    left(speed);
  }
}

void forward(int speed) {
  digitalWrite(DIR1_MOTOR1, HIGH);
  digitalWrite(DIR2_MOTOR2, HIGH);
  digitalWrite(DIR1_MOTOR3, HIGH);
  digitalWrite(DIR2_MOTOR4, HIGH);

  analogWrite(PWM1_MOTOR1, speed);
  analogWrite(PWM2_MOTOR2, speed);
  analogWrite(PWM1_MOTOR3, speed);
  analogWrite(PWM2_MOTOR4, speed);
}

void backward(int speed) {
  digitalWrite(DIR1_MOTOR1, LOW);
  digitalWrite(DIR2_MOTOR2, LOW);
  digitalWrite(DIR1_MOTOR3, LOW);
  digitalWrite(DIR2_MOTOR4, LOW);

  analogWrite(PWM1_MOTOR1, speed);
  analogWrite(PWM2_MOTOR2, speed);
  analogWrite(PWM1_MOTOR3, speed);
  analogWrite(PWM2_MOTOR4, speed);
}

void right(int speed) {
  digitalWrite(DIR1_MOTOR1, HIGH);
  digitalWrite(DIR2_MOTOR2, HIGH);
  digitalWrite(DIR1_MOTOR3, LOW);
  digitalWrite(DIR2_MOTOR4, LOW);

  analogWrite(PWM1_MOTOR1, speed);
  analogWrite(PWM2_MOTOR2, speed);
  analogWrite(PWM1_MOTOR3, speed);
  analogWrite(PWM2_MOTOR4, speed);
}

void left(int speed) {
  digitalWrite(DIR1_MOTOR1, LOW);
  digitalWrite(DIR2_MOTOR2, LOW);
  digitalWrite(DIR1_MOTOR3, HIGH);
  digitalWrite(DIR2_MOTOR4, HIGH);

  analogWrite(PWM1_MOTOR1, speed);
  analogWrite(PWM2_MOTOR2, speed);
  analogWrite(PWM1_MOTOR3, speed);
  analogWrite(PWM2_MOTOR4, speed);
}

void stopAll() {
  analogWrite(PWM1_MOTOR1, 0);
  analogWrite(PWM2_MOTOR2, 0);
  analogWrite(PWM1_MOTOR3, 0);
  analogWrite(PWM2_MOTOR4, 0);
}
