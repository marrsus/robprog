#include <Zumo32U4.h>
Zumo32U4ProximitySensors Sensor;
Zumo32U4Motors motors;
Zumo32U4OLED display;
Zumo32U4LineSensors lineSensors;

int threshold = 4;
int levels[10] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };
int levelCount = 10;

void setup() {
  // put your setup code here, to run once:
  Sensor.initThreeSensors();
  Serial.begin(9600);
  Sensor.setPeriod(500);
  Sensor.setBrightnessLevels(levels, levelCount);
  lineSensors.initThreeSensors();
  lineSensors.calibrate();
  motors.setSpeeds(70, 70);
  findLine();
  spinspin();
  gogo();
  dodo();
}

void loop() {
  display.clear();
  display.print("loop");
  motors.setSpeeds(0, 0);
}

void dodo() {
  display.clear();
  display.print("dodo");
  while (1) {
    Sensor.read();
    if (Sensor.countsFrontWithLeftLeds() > threshold) {
      return;
    } else if (Sensor.countsLeftWithLeftLeds() > Sensor.countsRightWithRightLeds()) {
      motors.setSpeeds(85, 70);
    } else if (Sensor.countsLeftWithLeftLeds() < Sensor.countsRightWithRightLeds()) {
      motors.setSpeeds(70, 85);
    } else {
      motors.setSpeeds(70, 70);
    }
  }
}

void findLine() {
  display.clear();
  display.print("findLine");
  int a, b, lineSensorValues[5];

  while (1) {
    Serial.println(a);
    Serial.println("yo");
    Serial.println(b);

    lineSensors.readCalibrated(lineSensorValues);
    a = abs(lineSensorValues[0] - lineSensorValues[1]);
    if (a > 1) {
      return;
    }
    b = abs(lineSensorValues[2] - lineSensorValues[1]);
    if (b > 1) {
      return;
    }
  }
}

void spinspin() {
  display.clear();
  display.print("SpinSpin");
  while (true) {
    motors.setSpeeds(70, -70);
    Sensor.read();

    Serial.println(Sensor.countsLeftWithLeftLeds());
    Serial.println(Sensor.countsRightWithRightLeds());
    Serial.println(Sensor.countsFrontWithRightLeds());

    if (Sensor.countsLeftWithLeftLeds() > 4 || Sensor.countsRightWithRightLeds() > 4) {
      return;
    }
  }
}
void gogo() {
  display.clear();
  display.print("gogo");
  while (true) {
    Sensor.read();

    if (Sensor.countsLeftWithLeftLeds() > 2 || Sensor.countsRightWithRightLeds() > 2) {
      return;
    } else if (Sensor.countsLeftWithLeftLeds() > 2) {
      motors.setSpeeds(60, 70);
    } else if (Sensor.countsRightWithRightLeds() > 2) {
      motors.setSpeeds(70, 60);
    } else {
      //cry and die
    }
  }
}