#include <Zumo32U4.h>
Zumo32U4ProximitySensors Sensor;
Zumo32U4Motors motors;
Zumo32U4OLED display;
Zumo32U4LineSensors lineSensors;

int threshold = 4;
int levels[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int levelCount = 10;
int a, b, lineSensorValues[5];

void setup() {
  // put your setup code here, to run once:
  display.clear();
  Sensor.initThreeSensors();
  Serial.begin(9600);
  //Sensor.setPeriod(500);
  //Sensor.setBrightnessLevels(levels, levelCount);
  lineSensors.initFiveSensors();
  lineSensors.calibrate();
  motors.setSpeeds(60, 60);
  findLine();
  Align();
  //YOYO();
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
    Serial.println(lineSensorValues[2]);
    Serial.println("højre1");
    Serial.println(lineSensorValues[4]);

    a = abs(lineSensorValues[2] - lineSensorValues[4]);
    b = abs(lineSensorValues[4] - lineSensorValues[2]);

    if (a > 2) {
      break;
    } else if (b > 2) {
      break;
    } else if (b==a&&lineSensorValues[4]>3){
      break;
    }
  }
}

void Align() {
  display.clear();
  display.print("Align");
  while (true) {
    lineSensors.readCalibrated(lineSensorValues);
    Serial.println("venstre");
    Serial.println(lineSensorValues[2]);
    Serial.println("højre");
    Serial.println(lineSensorValues[4]);
    if (lineSensorValues[4] > lineSensorValues[2]) {
      Serial.print("YO");
      display.print("YO");

      motors.setSpeeds(-50, -100);
      delay(175);

    } else if (lineSensorValues[4] < lineSensorValues[2]) {
      Serial.print("BO");
      display.print("BO");

      motors.setSpeeds(-100, -50);
      delay(175);

    } else if (lineSensorValues[4] == lineSensorValues[2] && lineSensorValues[4] > 3 && lineSensorValues[2] > 3) {
      break;
    }

    motors.setSpeeds(60, 60);
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
