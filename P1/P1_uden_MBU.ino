#include <Zumo32U4.h>
#include <Wire.h>
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4OLED display;
Zumo32U4IMU imu;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ProximitySensors proxSensors;

#define NUM_SENSORS 5
unsigned int lineSensorValues[NUM_SENSORS];
/*
double ogturnAngle = 0;
double ogtiltAngle = 0;
int16_t oggyroOffsetZ;
int16_t oggyroOffsetY;
uint16_t oggyroLastUpdate = 0;
int32_t ogtotalZ;
int32_t ogtotalY;
*/

double turnAngle = 0;
double tiltAngle = 0;
double pivotAngle = 0;
int16_t gyroOffsetZ;
int16_t gyroOffsetY;
int16_t gyroOffsetX;
uint16_t gyroLastUpdate = 0;
int threshold[5];
bool startFromRoad;
uint16_t brightnessLevels[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
uint8_t levels = 20;
int start;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  proxSensors.initThreeSensors();
  proxSensors.setBrightnessLevels(brightnessLevels, levels);
  startFromRoad = whereWeStart();
  lineSensors.initFiveSensors();
  setThresholds();
  delay(1000);
  gyroSetup();
  //ogturnSensorSetup();
  //while(true){
  //  Serial.print("Mixed_tilt_angle:");
  //  Serial.print(getTiltAngleInDegrees(true));
  //  Serial.print(",Gyroscope:");
  //  Serial.print(oggetTiltAngleInDegrees(true));
  //  Serial.print(",Accelerometer:");
  //  Serial.println(atan2(imu.a.x, imu.a.z) * 180 / M_PI);
  //  Serial.print("Mixed_pivot_angle:");
  //  Serial.print(getPivotAngleInDegrees(true));
  //  Serial.print(",Accelerometer:");
  //  Serial.println(atan2(imu.a.y, imu.a.z) * 180 / M_PI);
  //  motors.setSpeeds(-150,150);
  //  start = millis();
  //  while(start+1000 > millis() ) {
  //    printOnDisplay("",(String)getPivotAngleInDegrees(1));
  //  }
  //  motors.setSpeeds(150,-150);
  //  start = millis();
  //  while(start+1000 > millis() ) {
  //    printOnDisplay("",(String)getPivotAngleInDegrees(1));
  //  }
  //}
}

void loop() {
  // put your main code here, to run repeatedly:
  if(startFromRoad){
    roadToSidewalk();
  } else{
    sidewalkToRoad();
  }
}

void roadToSidewalk(){
  while(lineSensorValues[2] < threshold[2]){
    readLineSensors();
    followOnRight();
    printReadingsToSerial();
    motors.setSpeeds(100,100);
  }
  bip();
  motors.setSpeeds(100,100);
  delay(2000);
  printOnDisplay((String)getTurnAngleInDegrees(),(String)getTiltAngleInDegrees(0));
  turnAndDrive(90,1,100);
  imuReset();
  motors.setSpeeds(150,150);
  while(abs(getTiltAngleInDegrees(1))<18.0){
    printOnDisplay("Sut mig",(String)getTiltAngleInDegrees(0));
  }
  bip();
  while(abs(getTiltAngleInDegrees(1))>1){
    printOnDisplay("Tilt Angle",(String)getTiltAngleInDegrees(0));
  }
  bip();
  delay(300);
  printOnDisplay((String)getTurnAngleInDegrees(),"");
  turnAndDrive(90,0,100);
  delay(2000);
  motors.setSpeeds(0,0);
  while(!buttonA.isPressed()){}
  buttonA.waitForRelease();
}

void sidewalkToRoad(){
  while(abs(getTiltAngleInDegrees(true)) < 18){
    readLineSensors();
    printOnDisplay((String)lineSensorValues[0],(String)threshold[0]);
    if(getPivotAngleInDegrees(true) > 3.5 || getPivotAngleInDegrees(true) < -3.5){
      motors.setSpeeds(-200,200);
      bip();
    }else if(lineSensorValues[0]<threshold[0]){
      motors.setSpeeds(0,150);
    }else if(lineSensorValues[0]>threshold[0]){
      motors.setSpeeds(150,50);
      goodDelay(1000);
    }
  }
  motors.setSpeeds(150,150);
  while(abs(getTiltAngleInDegrees(true)) > 4){
    printOnDisplay("Tilt Angle",(String)getTiltAngleInDegrees(0));
  }
  delay(1000);
  turnAndDrive(90,1,100);
  delay(2000);
  motors.setSpeeds(0,0);
  delay(10000000);
}

void goodDelay(int delayAmount){
  start = millis();
  while(start + delayAmount > millis()){
    printOnDisplay("",(String)getPivotAngleInDegrees(1));
  }
}

bool whereWeStart(){
  printOnDisplay("A = RtoS","B = StoR");
  while(1){
    if(buttonA.isPressed()){
      buttonA.waitForRelease();
      return true;
    }
    if(buttonB.isPressed()){
      buttonB.waitForRelease();
      return false;
    }
  }
}

void followOnRight(){
  proxSensors.read();
  int R = proxSensors.countsRightWithRightLeds();
  int speed = 75;
  int k = 20;
  motors.setSpeeds(speed-(R-12)*k,speed+(R-12)*k);
  int Hspeed;
  int Vspeed;
  printOnDisplay((String)R,"");
}

void turnAndDrive(int angle, bool direction,int speed){
  imuReset();
    if(direction == true){
      while(getTurnAngleInDegrees() > -angle){
        motors.setSpeeds(speed,-speed);
        printOnDisplay((String)getTurnAngleInDegrees(),"");
      }
    } else{
      while(getTurnAngleInDegrees() < angle)
      motors.setSpeeds(-speed,speed);
      printOnDisplay((String)getTurnAngleInDegrees(),"");
    }
  printOnDisplay((String)getTurnAngleInDegrees(),"");
  motors.setSpeeds(150,150);
}

void printReadingsToSerial(){
  char buffer[80];
  sprintf(buffer, "%4d %4d %4d %4d %4d\n",
    lineSensorValues[0],
    lineSensorValues[1],
    lineSensorValues[2],
    lineSensorValues[3],
    lineSensorValues[4]
  );
  Serial.print(buffer);
}

void bip() {
  buzzer.playNote(NOTE_A(4), 20, 15);
}

void setThresholds() {
  int temp[10];
  printOnDisplay("On line", "Press A");
  while (!buttonA.isPressed()) {}
  bip();
  readLineSensors();
  printReadingsToSerial();
  for (int i = 0; i < 5; i++) {
    temp[i] = lineSensorValues[i];
  }
  buttonA.waitForRelease();
  printOnDisplay("Off line", "Press A");
  while (!buttonA.isPressed()) {}
  bip();
  readLineSensors();
  printReadingsToSerial();
  for (int i = 9; i > 4; i--) {
    temp[i] = lineSensorValues[i - 5];
  }
  buttonA.waitForRelease();
  for (int i = 0; i < 5; i++) {
    threshold[i] = (temp[i]*0.5 + temp[i + 5]*0.5);
  }
  printOnDisplay("Press A", "to start");
  while (!buttonA.isPressed()) {}
  buttonA.waitForRelease();
  Serial.println(threshold[2]);
  display.clear();
  display.println("GO!");
}

void readLineSensors() {
  lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
}

void printOnDisplay(String line1, String line2) {
  display.clear();
  display.println(line1);
  display.gotoXY(0, 1);
  display.println(line2);
}

void gyroSetup() {
  Wire.begin();
  imu.init();
  imu.enableDefault();
  imu.configureForTurnSensing();

  display.clear();
  display.print(F("Gyro cal"));

  // Turn on the yellow LED in case the LCD is not available.
  ledYellow(1);

  // Delay to give the user time to remove their finger.
  delay(500);

  // Calibrate the gyro.
  int32_t totalZ = 0;
  int32_t totalY = 0;
  int32_t totalX = 0;
  for (uint16_t i = 0; i < 1024; i++) {
    // Wait for new data to be available, then read it.
    while (!imu.gyroDataReady()) {}
    imu.readGyro();

    // Add the Z axis reading to the total.
    totalZ += imu.g.z;
    totalY += imu.g.y;
    totalX += imu.g.x;
  }
  ledYellow(0);
  gyroOffsetZ = totalZ / 1024;
  gyroOffsetY = totalY / 1024;
  gyroOffsetX = totalX / 1024;

  // Display the angle (in degrees from -180 to 180) until the
  // user presses A.
  display.clear();
  imuReset();
}

double getTurnAngleInDegrees() {
  turnSensorUpdate();
  return turnAngle;
}

double getTiltAngleInDegrees(bool updateSensor) {
  if (updateSensor) turnSensorUpdate();
  return tiltAngle;
}

double getPivotAngleInDegrees(bool updateSensor){
  if (updateSensor) turnSensorUpdate();
  return pivotAngle;
}

void imuReset() {
  gyroLastUpdate = millis();
  turnAngle = 0;
  tiltAngle = 0;
  pivotAngle = 0;
}

// Read the gyro and update the angle.  This should be called as
// frequently as possible while using the gyro to do turns.
void turnSensorUpdate() {
  // Read the measurements from the gyro.
  imu.readGyro();
  imu.readAcc();
  int16_t turnRate = imu.g.z - gyroOffsetZ;
  int16_t tiltRate = -imu.g.y + gyroOffsetY;
  int16_t pivotRate = imu.g.x - gyroOffsetX;

  // Figure out how much time has passed since the last update (dt)
  uint16_t m = millis();
  uint16_t dt = m - gyroLastUpdate;
  gyroLastUpdate = m;

  // Multiply dt by turnRate in order to get an estimation of how
  // much the robot has turned since the last update.
  // (angular change = angular velocity * time)
  double dZ = (double)turnRate*0.07 * (double)dt*0.001;
  double dY = (double)tiltRate*0.07 * (double)dt*0.001;
  double dX = (double)pivotRate*0.07 * (double)dt*0.001;
  // The units of turnrate are gyro digits.
  // The conversion from gyro digits to
  // degrees per second (dps) is determined by the sensitivity of
  // the gyro: 0.07 degrees per second per digit.
  // Also multiply the time by 0.001 to convert from ms to s.
  // turnRate[deg/s] * dt[s] = dZ[deg]

  turnAngle += dZ;

  //For the tilt, also calculate it using accelerometer.
  double accAngleY = (atan2(imu.a.x, imu.a.z) * 180 / M_PI);
  double accAngleX = (atan2(imu.a.y, imu.a.z) * 180 / M_PI);

  //Complimentary filter.
  tiltAngle = (tiltAngle + dY)*0.99 + accAngleY*0.01;
  pivotAngle = (pivotAngle + dX)*0.9999 + accAngleX*0.0001;
  //pivotAngle += dX;
}
