#include <Zumo32U4.h>
#include <Wire.h>

Zumo32U4LCD display;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer;
Zumo32U4LineSensors lineSensors;

uint16_t brightnessLevels[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
uint8_t levels = 20;

uint16_t defaultBrightnessLevels[] = { 4, 15, 32, 55, 85, 120 };
uint8_t defaultLevels = 6;

int baseSpeed = 200;
int K = 40;
int thresh;

uint16_t lineSensorValues[3];

void setup() {
  Serial.begin(9600);
  lineSensors.initThreeSensors();
  proxSensors.initThreeSensors();
  proxSensors.setBrightnessLevels(brightnessLevels, levels);
  while(!buttonA.getSingleDebouncedPress()){
    lineSensors.read(lineSensorValues);
    display.clear();
    display.gotoXY(0, 0);
    display.print("tape");
    display.gotoXY(0, 1);
    display.print(lineSensorValues[1]);
    delay(100);
  }
  thresh = lineSensorValues[1]-50;
  display.clear();
  display.gotoXY(0, 0);
  display.print("Press");
  display.gotoXY(0, 1);
  display.print("A");
  while(!buttonA.getSingleDebouncedPress()){}
}

void loop() {
  proxSensors.read();
  int L = proxSensors.countsLeftWithLeftLeds();
  int R = proxSensors.countsRightWithRightLeds();
  
  int leftSpeed = baseSpeed+K*(L-R);
  int rightSpeed = baseSpeed-K*(L-R);

  motors.setSpeeds(leftSpeed, rightSpeed);
  lineSensors.read(lineSensorValues);

  display.clear();
  display.gotoXY(0,0);
  display.print(L);
  display.gotoXY(6,0);
  display.print(R);
  display.gotoXY(0, 1);
  display.print(leftSpeed);
  display.gotoXY(5, 1);
  display.print(rightSpeed);

  if (lineSensorValues[1] > thresh) {
    motors.setSpeeds(0,0);
    while(true){}
  }
 }
