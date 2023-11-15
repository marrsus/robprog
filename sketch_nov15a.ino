#include <Zumo32U4.h>
Zumo32U4Motors motors;
Zumo32U4LCD display;
Zumo32U4LineSensors lineSensors;

int a, b, lineSensorValues[5];
bool flag1, flag2;
int passes = 0;

void setup() {
  flag1 = false;
  flag2 = false;
  Serial.begin(9600);
  lineSensors.initFiveSensors();
  lineSensors.calibrate();
  motors.setSpeeds(70, 70);
  Align();
}

void loop() {
}

void Align() {
  display.clear();
  display.print("Align");
  while (1) {

    lineSensors.readCalibrated(lineSensorValues);
    Serial.println("venstre1");
    Serial.println(lineSensorValues[1]);
    Serial.println("højre1");
    Serial.println(lineSensorValues[3]);

    if (lineSensorValues[3] < 2 && lineSensorValues[1] < 2) {
      continue;
    }

    if (lineSensorValues[3] > lineSensorValues[1]) {
      if (flag1 == true) {passes++;} else {flag2 = false;}
      flag2 = true;
      Serial.print("YO");
      motors.setSpeeds(-50, -130);
      delay(100 - (10*passes));

    } else if (lineSensorValues[3] < lineSensorValues[1]) {
      if (flag2 == true) {passes++;} else {flag1 = false;}
      flag1 = true;
      Serial.print("BO");
      motors.setSpeeds(-130, -50);
      delay(100 - (10*passes));
    } else{
      motors.setSpeeds(0,0);
      return;
    }
    motors.setSpeeds(60,60);
  }
}
