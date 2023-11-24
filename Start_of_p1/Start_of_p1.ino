#include <Zumo32U4.h>
#include <Wire.h>
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4OLED display;
Zumo32U4IMU imu;
//____
float combinedAngle;
int16_t calibration = 0;  // 1gyroUnit = 0.07dps, same as gyro reading.
uint16_t lastUpdateTime;  //us (micro-seconds)
float mmPerClick = 0.13;

//___
int threshold = 23;
bool sidewalk = true;
bool road = false;
int lineSensorValues[5];


void setup() {
  Wire.begin();

  lineSensors.initFiveSensors();
  lineSensors.calibrate();
  Serial.begin(9600);
  display.clear();


  //init IMU
  if (!imu.init()) {
    while (true) {
      Serial.println(F("Failed to initialize IMU sensors."));
      delay(100);
    }
  }
  imu.enableDefault();
  imu.configureForBalancing();
  calibrateIMU();
}

void loop() {
  display.println("ye");
  while (sidewalk) {
    lineSensors.readCalibrated(lineSensorValues);
    combinedangle();
    display.clear();
    // display.println(lineSensorValues[0]);
    display.println(-1 * combinedAngle);

    if (lineSensorValues[0] > threshold) {
      motors.setSpeeds(150, 50);

    } else if (lineSensorValues[0] > 11 && lineSensorValues[0] < 20) {
      display.clear();
      display.println("ramp");
      motors.setSpeeds(60, 150);

    } else {
      motors.setSpeeds(60, 120);
    }
    if (combinedAngle < -1 * 6.0) {
      road = true;
      sidewalk = false;
    }
  }
  while (road) {
    display.clear();
    display.println(-1 * combinedAngle);
    motors.setSpeeds(60, 120);
        combinedangle();

    if (-1*combinedAngle < -0.2) {
      motors.setSpeeds(0, 0);
      display.println("I DID IT!!! FUCK YEAH!");
      delay(10000);
    }
  }
}
void calibrateIMU() {
  int32_t total = 0;
  for (uint16_t i = 0; i < 2048; i++) {
    // Wait for new data to be available, then read it.
    while (!imu.gyroDataReady()) {}
    imu.readGyro();
    total += imu.g.y;
  }
  calibration = total / 2048;
  lastUpdateTime = micros();
}

void combinedangle() {
  float gyroAngleChange = getGyroAngleChange();
  imu.readAcc();
  float accAngle = (atan2(imu.a.x, imu.a.z) * 180 / M_PI);

  combinedAngle = (combinedAngle + gyroAngleChange) * 0.95 + accAngle * 0.05;

  static byte lastPrintTime = 0;
  byte m = millis();
}

float getGyroAngleChange() {
  //Get time since we last measured:
  uint16_t micro = micros();
  uint16_t dt = micro - lastUpdateTime;
  lastUpdateTime = micro;
  //Read gyro data and subtract calibation value.
  imu.readGyro();
  int16_t gyroReading = imu.g.y - calibration;
  //Calculate change in angle (degrees)
  return ((float)gyroReading * 70 * dt / 1000000000);
}