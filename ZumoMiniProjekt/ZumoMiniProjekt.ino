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

// vars to count the accumulated counts of the encoders
int movementCommand0 = 0;   // used to select the command

int const countJump = 100;  // number of counts before jumping to next command in stage 0     

// control selection 1              0 Forward
//                                  1 Backward
//                                  2 turn
int chosenChallenge = 0;

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
uint16_t brightnessLevels[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
uint8_t levels = 20;

uint16_t defaultBrightnessLevels[] = { 4, 15, 32, 55, 85, 120 };
uint8_t defaultLevels = 6;

int baseSpeed3 = 200;
int K3 = 40;
int thresh;

//uint16_t lineSensorValues[3];

//Copied from challenge 4
int movementCommand = 0;
int movementParameter = 0;

int speed = 100;
int threshold4 = 500;

int stage = 0;
int travelDist = 0;
float wheelCirc = 12.6;
int chosenCommand = 0;
const float space1 = 4.4589;
bool lineSensorDetected = false;

//float turningAngle = 0; // Declare turningAngle globally

uint32_t turnAngle = 0;
int16_t turnRate;
int16_t gyroOffset;
uint16_t gyroLastUpdate = 0;
int rettning=0;

//Copied from challenge 5
int baseSpeed5 = 100;
int k5 = 15;

int encoderThreshFirst;
int encoderThresh;
int distancePoleSwitch;

//Copied from challenge 6
int angle = 0;
int goTime = 0;
int movement = 0;

void setup() {
  lineSensors.initFiveSensors();
  proxSensors.initThreeSensors();
  proxSensors.setBrightnessLevels(brightnessLevels, levels);
  delay(100);
}