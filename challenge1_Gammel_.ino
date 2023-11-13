#include <Zumo32U4.h>
Zumo32U4ProximitySensors Sensor;
Zumo32U4Motors motors;
Zumo32U4LCD display;
Zumo32U4LineSensors lineSensors;

int threshold = 6;
int levels[10] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };
int levelCount = 10;
int a, b, lineSensorValues[5];

void setup() {
  // put your setup code here, to run once:
  Sensor.initThreeSensors();
  Serial.begin(9600);
  //Sensor.setPeriod(500);
  //Sensor.setBrightnessLevels(levels, levelCount);
  lineSensors.initFiveSensors();
  lineSensors.calibrate();
  motors.setSpeeds(70, 70);
  findLine();
  Align();
  YOYO();
}

void loop() {
  display.clear();
  display.print("loop");
  motors.setSpeeds(0, 0);
}

void findLine() {
  display.clear();
  display.print("findLine");

  while (1) {

    lineSensors.readCalibrated(lineSensorValues);
    Serial.println("venstre1");
    Serial.println(lineSensorValues[1]);
    Serial.println("højre1");
    Serial.println(lineSensorValues[3]);

    a = abs(lineSensorValues[1] - lineSensorValues[3]);
    b = abs(lineSensorValues[3] - lineSensorValues[1]);

    if (a > 2) {
      break;
    } else if (b > 2) {
      break;
    } else if (b==a&&lineSensorValues[3]>3){
      break;
    }
  }
}

void Align() {
  display.clear();
  display.print("Align");
  while (true) {
   // delay(10);
    lineSensors.readCalibrated(lineSensorValues);

    if (lineSensorValues[3] > lineSensorValues[1]) {
      Serial.print("YO");
        display.print("YO");

      motors.setSpeeds(-50, -150);
      delay(325);

    } else if (lineSensorValues[3] < lineSensorValues[1]) {
      Serial.print("BO");
        display.print("BO");

      motors.setSpeeds(-150, -50);
      delay(325);
    } else{
      return;
    }
    motors.setSpeeds(60,60);
    findLine();

    
  }
}


void YOYO() {
  display.clear();
  display.print("YOYO");
  while (true) {
    motors.setSpeeds(90, 90);
    Serial.println(Sensor.countsFrontWithRightLeds());

    Sensor.read();
    if (Sensor.countsFrontWithLeftLeds() < threshold) {
      return;
    }
  }
}
/*
void spinspin() {
  display.clear();
  display.print("SpinSpin");
  while (true) {
    motors.setSpeeds(90, -90);
    Sensor.read();
    Serial.println(Sensor.countsLeftWithLeftLeds());
    Serial.println(Sensor.countsRightWithRightLeds());
    Serial.println(Sensor.countsFrontWithRightLeds());

    if (Sensor.countsLeftWithRightLeds() > 4 || Sensor.countsRightWithLeftLeds() > 4||Sensor.countsLeftWithLeftLeds() > 4 ||Sensor.countsRightWithRightLeds() > 4) {
      return;
    }
  }
}
void gogo() {
  display.clear();
  display.print("gogo");
  while (true) {
    Sensor.read();
    Serial.println(Sensor.countsLeftWithLeftLeds());
    Serial.println(Sensor.countsRightWithRightLeds());
    Serial.println(Sensor.countsFrontWithRightLeds());
    if (Sensor.countsLeftWithLeftLeds() > 4 && Sensor.countsRightWithRightLeds() > 4) {
      return;
    } else if (Sensor.countsLeftWithLeftLeds() > 4) {
      motors.setSpeeds(60, 70);
    } else if (Sensor.countsRightWithRightLeds() > 4) {
      motors.setSpeeds(70, 60);
    } else {
      //cry and die
    }
  }
}
*/
/*
void dodo() {
  
  display.clear();
  display.print("dodo");
  while (1) {
        Serial.println(Sensor.countsFrontWithRightLeds());
    Sensor.read();
    if (Sensor.countsFrontWithLeftLeds()>threshold) {
      return;
    } else if (Sensor.countsLeftWithLeftLeds() > Sensor.countsRightWithRightLeds()) {
      motors.setSpeeds(150, 50);
    } else if (Sensor.countsLeftWithLeftLeds() < Sensor.countsRightWithRightLeds()) {
      motors.setSpeeds(50, 150);
    } else {
      motors.setSpeeds(60, 60);
    }
  }
}
*/
