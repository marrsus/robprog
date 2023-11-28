//Copied from challenge 0
#include <Wire.h>
#include <Zumo32U4.h>
#include <math.h>

Zumo32U4Encoders encoders;
Zumo32U4OLED display;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonA buttonB;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4IMU imu;


int const countJump = 100;  // number of counts before jumping to next value

//Copied from challenge 1
bool flag1, flag2;
int passes = 0;

int levels1[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int levelCount = 10;

//Copied from challenge 2
#define NUM_SENSORS 5
unsigned int lineSensorValues[NUM_SENSORS];

int threshold[5];

//Copied from challenge 3
uint16_t brightnessLevels[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
uint8_t levels = 20;

uint16_t defaultBrightnessLevels[] = { 4, 15, 32, 55, 85, 120 };
uint8_t defaultLevels = 6;

int baseSpeed3 = 200;
int K3 = 40;

//uint16_t lineSensorValues[3];

//Copied from challenge 4
int movementCommand = 0;
int movementParameter = 0;

int speed = 100;

int travelDist = 0;
float wheelCirc = 12.6;
const float space1 = 4.4589;
bool lineSensorDetected = false;

//float turningAngle = 0; // Declare turningAngle globally

uint32_t turnAngle = 0;
int16_t turnRate;
int16_t gyroOffset;
uint16_t gyroLastUpdate = 0;
int rettning = 0;

//Copied from challenge 5
int baseSpeed5 = 100;
int k5 = 15;

int encoderThreshFirst;
int encoderThresh;
int distancePoleSwitch;

//Copied from challenge 6
int angle = 0;

void setup() {
  lineSensors.initFiveSensors();
  proxSensors.initThreeSensors();
  proxSensors.setBrightnessLevels(brightnessLevels, levels);
  delay(100);
}

void bip() {
  buzzer.playNote(NOTE_A(4), 20, 15);
}

void resetEncoders() {
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

void readLineSensors() {
  lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
}

void printSomething(String line1, String line2) {
  display.clear();
  display.println(line1);
  display.gotoXY(0, 1);
  display.println(line2);
}

int get_input(int start_val, int sensativiti, int stepsize, int max, int min, char* variable_name) {
  int val = start_val;
  int movement = 0;
  while (!buttonA.isPressed()) {
    movement = movement + encoders.getCountsRight();
    resetEncoders();
    if (abs(movement) > sensativiti) {
      bip();
      display.clear();
      display.println(variable_name);
      display.gotoXY(0, 1);
      display.println((char)(val+48));
      movement = 0;

      if (movement > 0) val += stepsize; else val -= stepsize;
      if (val > max) val = max;
      if (val < min) val = min;
    }
  }
  return val;
}

void Align() {
  while (1) {
    motors.setSpeeds(60, 60);

    readLineSensors();
    if (lineSensorValues[3] < threshold[3] && lineSensorValues[1] < threshold[1]) {
      continue;
    }

    if (lineSensorValues[3] > lineSensorValues[1]) {
      motors.setSpeeds(-50, -130);
      flag1 = true;
    } else if (lineSensorValues[3] < lineSensorValues[1]) {
      motors.setSpeeds(-130, -50);
      flag1 = false;
    } else {
      motors.setSpeeds(0, 0);
      return;
    }
    if (flag1 == flag2 && passes < 6) {
      flag2 = flag1;
      passes++;
    }
    delay(100 - (10 * passes));
  }
}

void setThresholds() {
  int temp[10];
  printSomething("On line", "Press A");
  while (!buttonA.isPressed()) {}
  bip();
  readLineSensors();
  for (int i = 0; i < 5; i++) {
    temp[i] = lineSensorValues[i];
  }
  buttonA.waitForRelease();
  printSomething("Off line", "Press A");
  while (!buttonA.isPressed()) {}
  bip();
  readLineSensors();
  for (int i = 9; i > 4; i--) {
    temp[i] = lineSensorValues[i - 5];
  }
  buttonA.waitForRelease();
  for (int i = 0; i < 5; i++) {
    threshold[i] = (temp[i] + temp[i + 5]) / 2;
  }
  printSomething("Press A", "to start");
  while (!buttonA.isPressed()) {}
  buttonA.waitForRelease();
  display.clear();
  display.println("GO!");
}