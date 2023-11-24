#include <Zumo32U4.h>
#include <Wire.h>

Zumo32U4LCD display;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer;
Zumo32U4Encoders encoders;
Zumo32U4LineSensors lineSensors;

uint16_t lineSensorValues[3];

uint16_t brightnessLevels[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
uint8_t levels = 20;

uint16_t defaultBrightnessLevels[] = { 4, 15, 32, 55, 85, 120 };
uint8_t defaultLevels = 6;

int baseSpeed = 100;
int k = 15;
int thresh;
int encoderThreshFirst;
int encoderThresh;
int distancePoleSwitch;

void setup() {
  Serial.begin(9600);
  proxSensors.initThreeSensors();
  lineSensors.initThreeSensors();
  proxSensors.setBrightnessLevels(brightnessLevels, levels);

  while(!buttonA.getSingleDebouncedPress()){
    encoderThreshFirst = 2850 + encoders.getCountsLeft();
    encoderThresh = 3400 + encoders.getCountsRight();
    display.clear();
    display.gotoXY(0,0);
    display.print(encoderThreshFirst);
    display.gotoXY(0,1);
    display.print(encoderThresh);
    delay(50);
  }
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  while(!buttonA.getSingleDebouncedPress()){
    distancePoleSwitch = 1400 + encoders.getCountsLeft();
    k = 15 + encoders.getCountsRight()/50;
    display.clear();
    display.gotoXY(0,0);
    display.print(distancePoleSwitch);
    display.gotoXY(0,1);
    display.print(k);
    delay(50);
  }
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  while(!buttonA.getSingleDebouncedPress()){
    proxSensors.read();
    int L = proxSensors.countsLeftWithLeftLeds();
    int R = proxSensors.countsRightWithRightLeds();
    display.clear();
    display.gotoXY(0,0);
    display.print(L);
    display.gotoXY(0,1);
    display.print(encoders.getCountsLeft());
    display.gotoXY(6,1);
    display.print(R);
    display.gotoXY(4,0);
    display.print(encoders.getCountsRight());
  }

  //Turn right 180*
  encoders.getCountsAndResetLeft();
  while(true) {
    proxSensors.read();
    int L = proxSensors.countsLeftWithLeftLeds();
    int R = proxSensors.countsRightWithRightLeds();
    revolve(1, R);
    display.clear();
    display.gotoXY(0,0);
    display.print(L);
    display.gotoXY(0,1);
    display.print(encoders.getCountsLeft());
    if(encoders.getCountsLeft() > encoderThreshFirst) break;
  }
}

void loop() {
  changePole();

  //Revolve around right pole
  encoders.getCountsAndResetRight();
  while(true){
    proxSensors.read();
    int L = proxSensors.countsLeftWithLeftLeds();
    int R = proxSensors.countsRightWithRightLeds();
    revolve(-1, L);
    lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
    if(lineSensorValues[0] > 600 || lineSensorValues[1] > 600 || lineSensorValues[2] > 600) encoders.getCountsAndResetRight();
    if(encoders.getCountsRight() > encoderThresh) break;
    display.clear();
    display.gotoXY(0,0);
    display.print(L);
    display.gotoXY(0,1);
    display.print(encoders.getCountsRight());
  }
  changePole();

  //Revolve around left pole
  encoders.getCountsAndResetLeft();
  while(true){
    proxSensors.read();
    int L = proxSensors.countsLeftWithLeftLeds();
    int R = proxSensors.countsRightWithRightLeds();
    revolve(1, R);
    lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
    if(lineSensorValues[0] > 600 || lineSensorValues[1] > 600 || lineSensorValues[2] > 600) encoders.getCountsAndResetLeft();
    if(encoders.getCountsLeft() > encoderThresh) break;
    display.clear();
    display.gotoXY(0,0);
    display.print(R);
    display.gotoXY(0,1);
    display.print(encoders.getCountsLeft());
  }
}
void changeToRightPole() {
  motors.setSpeeds(baseSpeed, baseSpeed*2);
  while(true) {
    proxSensors.read();
    int L = proxSensors.countsLeftWithLeftLeds();
    int R = proxSensors.countsRightWithRightLeds();
    display.gotoXY(0,0);
    display.print(L);
    if(L >= 11 || (R == 0 && L == 0)) break;
  }
  delay(800);
}
void changePole() {
  motors.setSpeeds(baseSpeed, baseSpeed);
  encoders.getCountsAndResetRight();
  while(encoders.getCountsRight() < distancePoleSwitch) {}
}
//dir 1 = right  dir -1 = left
//proxSensorCount = counts from sensor pointing towards revolved object
void revolve(int dir, int proxSensorCount) { 
  int leftSpeed = baseSpeed+dir*proxSensorCount*k;
  int rightSpeed = baseSpeed-dir*proxSensorCount*k;
  motors.setSpeeds(leftSpeed, rightSpeed);
}