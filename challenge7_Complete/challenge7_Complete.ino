#include <Wire.h>
#include <Zumo32U4.h>
#include <Zumo32U4Encoders.h>
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4OLED display;
Zumo32U4Encoders encoders;


int a, b, lineSensorValues[5];
bool flag1, flag2;
int passes = 0;
int threshold;
int c = 0, f = 0;
int16_t turnRate;
int16_t flow = 0;
int16_t gyroOffset;
int16_t turnAngle = 0;
uint16_t gyroLastUpdate = 0;
int32_t turnDegrees;
int16_t countsLeft, countsRight;



void setup() {
  lineSensors.initFiveSensors();
  lineSensors.calibrate();
  Serial.begin(9600);
  display.clear();
  threshold = 2;
  flag1 = false;
  flag2 = false;
}

void loop() {
  while (c == 0) {
    motors.setSpeeds(70, 70);
    Align();
    motors.setSpeeds(70, 70);
    delay(1000);
    Stop();
    motors.setSpeeds(0, 0);
    c++;
  }
}

void Align() {
  while (true) {

    lineSensors.readCalibrated(lineSensorValues);
    if (lineSensorValues[3] < 2 && lineSensorValues[1] < 2) {
      continue;
    }

    if (lineSensorValues[3] > lineSensorValues[1]) {
      if (flag1 == true) {
        passes++;
      } else {
        flag2 = false;
      }
      flag2 = true;
      motors.setSpeeds(-50, -130);
      delay(100 - (10 * passes));

    } else if (lineSensorValues[3] < lineSensorValues[1]) {
      if (flag2 == true) {
        passes++;
      } else {
        flag1 = false;
      }
      flag1 = true;
      motors.setSpeeds(-130, -50);
      delay(100 - (10 * passes));
    } else {
      motors.setSpeeds(60, 60);
      return;
    }
    motors.setSpeeds(60, 60);
  }
}
void Stop() {
  flow = 0;
  turnRate = 0;
  delay(100);
  encoders.getCountsAndResetRight();
  encoders.getCountsAndResetLeft();
  countsLeft = encoders.getCountsAndResetLeft();
  countsRight = encoders.getCountsAndResetRight();
  while (f == 0) {
    lineSensors.readCalibrated(lineSensorValues);
    if (lineSensorValues[3] > threshold || lineSensorValues[1] > threshold) {
      motors.setSpeeds(0, 0);
      display.println("yep");
      f++;
      return;
    } else if (countsLeft < countsRight) {
      motors.setSpeeds(90, 60);
    } else if (countsRight < countsLeft) {
      motors.setSpeeds(60, 90);
    } else {
      motors.setSpeeds(60, 60);
    }
    encoders.getCountsAndResetRight();
    encoders.getCountsAndResetLeft();
  }
}
