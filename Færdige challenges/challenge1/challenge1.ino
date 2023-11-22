#include <Zumo32U4.h>
Zumo32U4Motors motors;
Zumo32U4ProximitySensors Sensor;
Zumo32U4LineSensors lineSensors;

int a, b, lineSensorValues[5];
bool flag1, flag2;
int passes = 0;

int levels[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int levelCount = 10;

void setup() {
  Sensor.initFrontSensor();
  Sensor.setPeriod(500);
  Sensor.setBrightnessLevels(levels, levelCount);
  lineSensors.initFiveSensors();
  lineSensors.calibrate();
  chal1();
}

void loop() {
}
void chal1(){
  motors.setSpeeds(70, 70);
  Align();
  stopafter(2);
}

void Align() {
  while (1) {

    lineSensors.readCalibrated(lineSensorValues);
    if (lineSensorValues[3] < 2 && lineSensorValues[1] < 2) {
      continue;
    }

    if (lineSensorValues[3] > lineSensorValues[1]) {
      if (flag1 == true) {passes++;} else {flag2 = false;}
      flag2 = true;
      motors.setSpeeds(-50, -130);
      delay(100 - (10*passes));

    } else if (lineSensorValues[3] < lineSensorValues[1]) {
      if (flag2 == true) {passes++;} else {flag1 = false;}
      flag1 = true;
      motors.setSpeeds(-130, -50);
      delay(100 - (10*passes));
    } else{
      motors.setSpeeds(0,0);
      return;
    }
    motors.setSpeeds(60,60);
  }
}

void stopafter(int threshold) {
  while (true) {
    motors.setSpeeds(90, 90);
    Sensor.read();
    if (Sensor.countsFrontWithLeftLeds() < threshold) {
      motors.setSpeeds(0,0);
      return;
    }
  }
}
