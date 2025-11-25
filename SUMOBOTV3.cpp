#include <IRremote.hpp>
#include <AFMotor.h>
#define IR_RECV_PIN A0
#define POWER_BTN 0x57A8FF00  
#define SUMO_BTN  0x22DDFF00
#define CALIB_BTN 0xBE41FF00 

AF_DCMotor motorLeft(3);   
AF_DCMotor motorRight(4);  
IRrecv irrecv(IR_RECV_PIN);

const int trigPin = 10;
const int echoPin = 9;
const int lineSensor[5] = {A1, A2, A3, A4, A5}; 

bool powerOn = false;
bool sumoMode = false;
int speed = 255;


int blackThreshold[5] = {500, 500, 500, 500, 500}; 
bool isCalibrated = false;
int calibrationStep = 0; 
int blackValues[5];
int whiteValues[5];

void setup() {

  Serial.begin(9600);
  irrecv.begin(IR_RECV_PIN, true, LED_BUILTIN);
  motorLeft.setSpeed(speed);
  motorRight.setSpeed(speed);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  for (int i = 0; i < 5; i++) {
    pinMode(lineSensor[i], INPUT);
  }
  stopMotors();
  Serial.println("SUMOBOT IS READY");
  Serial.println("=== CALIBRATION REQUIRED ===");
  Serial.println("STEP 1: Place robot on BLACK LINE, then press CALIB");
}

void stopMotors() {
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}

void forward() {
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
}

void backward() {
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
}

void leftTurn() {
  motorLeft.run(BACKWARD);
  motorRight.run(FORWARD);
}

void rightTurn() {
  motorLeft.run(FORWARD);
  motorRight.run(BACKWARD);
}

long getDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000);
  if (duration == 0) return -1;

  long distanceCM = duration * 0.034 / 2;
  return distanceCM;
}

void calibrateLineSensors() {

  if (calibrationStep == 0) {
    Serial.println("Reading BLACK LINE values...");
    for (int i = 0; i < 5; i++) {
      blackValues[i] = analogRead(lineSensor[i]);
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.print(" Black: ");
      Serial.println(blackValues[i]);
    }
    calibrationStep = 1;
    Serial.println("");
    Serial.println("STEP 2: Place robot on WHITE surface, then press CALIB");
  }
  else if (calibrationStep == 1) {
    Serial.println("Reading WHITE surface values...");
    for (int i = 0; i < 5; i++) {
      whiteValues[i] = analogRead(lineSensor[i]);
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.print(" White: ");
      Serial.println(whiteValues[i]);
    }
    Serial.println("");
    Serial.println("Calculating thresholds...");
    for (int i = 0; i < 5; i++) {
      blackThreshold[i] = (blackValues[i] + whiteValues[i]) / 2;
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.print(" Threshold: ");
      Serial.println(blackThreshold[i]);
    }
    isCalibrated = true;
    calibrationStep = 0; 
    Serial.println("");
    Serial.println("=== CALIBRATION COMPLETE ===");
    Serial.println("Press POWER to turn on, then SUMO to start");
  }
}

bool detectBlackLine() {

  for (int i = 0; i < 5; i++) {
    int sensorValue = analogRead(lineSensor[i]);
    if (sensorValue < blackThreshold[i]) {
      Serial.print("BLACK LINE detected on sensor ");
      Serial.println(i + 1);
      return true;
    }
  }
  return false;
}

void sumobotMode() {
  
  if (detectBlackLine()) {
    Serial.println("Edge detected! Backing up...");
    motorLeft.setSpeed(255);
    motorRight.setSpeed(255);
    backward();
    delay(500);
    stopMotors();
    delay(100);
    return;
  }
  long dist = getDistance();
  if (dist > 0 && dist < 40) {
    // Enemy detected - attack!
    Serial.print("Enemy detected at ");
    Serial.print(dist);
    Serial.println("cm - ATTACKING!");
    motorLeft.setSpeed(255);
    motorRight.setSpeed(255);
    forward();
  } else {
    Serial.println("Searching for enemy...");
    motorLeft.setSpeed(200);
    motorRight.setSpeed(200);
    rightTurn(); 
  }
}

void loop() {
  IRData* data = irrecv.read();
  if (data != nullptr) {
    unsigned long code = data->decodedRawData;
    Serial.print("IR Code: 0x");
    Serial.println(code, HEX);

    if (code == POWER_BTN) {
      powerOn = !powerOn;
      if (!powerOn) {
        sumoMode = false;
      }
      stopMotors();
      Serial.println(powerOn ? "POWER ON" : "POWER OFF");
    } 
    else if (code == CALIB_BTN) {
      stopMotors();
      calibrateLineSensors();
    }
    else if (powerOn && code == SUMO_BTN) {
      sumoMode = !sumoMode;
      if (!sumoMode) {
        stopMotors();
      }
      Serial.println(sumoMode ? "SUMO MODE ACTIVATED" : "SUMO MODE DEACTIVATED");
    }
    irrecv.resume();
  }
  if (powerOn && sumoMode) {
    sumobotMode();
  } else {
    stopMotors();
  }
}