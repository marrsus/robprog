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

#define MBUconstant 11930464.71

uint32_t turnAngle = 0;
uint32_t tiltAngle = 0;
int16_t turnRate;
int16_t tiltRate;
int16_t gyroOffsetZ;
int16_t gyroOffsetY;
uint16_t gyroLastUpdate = 0;
int threshold[5];
bool startFromRoad;
uint16_t brightnessLevels[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
uint8_t levels = 20;

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
  while(true){
    printOnDisplay((String)getTurnAngleInDegrees(),(String)getTiltAngleInDegrees(false));
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(startFromRoad==true){
    roadToSidewalk();
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
  gyroReset();
  printOnDisplay((String)getTurnAngleInDegrees(),"");
  turnAndDrive(5,270,1,150);
  delay(1500);
  gyroReset();
  printOnDisplay((String)getTurnAngleInDegrees(),"");
  turnAndDrive(90,355,0,150);
  delay(2000);
  motors.setSpeeds(0,0);
  delay(10000000);
}

void sidewalkToRoad(){

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

void turnAndDrive(int angleMax, int angleMin, bool direction,int speed){
  gyroReset();
  while(getTurnAngleInDegrees() < angleMax || getTurnAngleInDegrees() > angleMin){
    if(direction == true){
      motors.setSpeeds(speed,-speed);
    } else{
      motors.setSpeeds(-speed,speed);
    }
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
    threshold[i] = (temp[i] + temp[i + 5]) / 2;
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
  for (uint16_t i = 0; i < 1024; i++) {
    // Wait for new data to be available, then read it.
    while (!imu.gyroDataReady()) {}
    imu.readGyro();

    // Add the Z axis reading to the total.
    totalZ += imu.g.z;
    totalY += imu.g.y;
  }
  ledYellow(0);
  gyroOffsetZ = totalZ / 1024;
  gyroOffsetY = totalY / 1024;

  // Display the angle (in degrees from -180 to 180) until the
  // user presses A.
  display.clear();
  gyroReset();
}

uint32_t getTurnAngleInDegrees() {
  turnSensorUpdate();
  // do some math and pointer magic to turn angle in seconds to angle in degree
  return (((uint32_t)turnAngle >> 16) * 360) >> 16;
}

double getTiltAngleInDegrees(bool updateSensor) {
  if (updateSensor) turnSensorUpdate();
  return (((uint32_t)tiltAngle >> 16) * 360) >> 16;
}

void gyroReset() {
  gyroLastUpdate = micros();
  turnAngle = 0;
  tiltAngle = 0;
}

// Read the gyro and update the angle.  This should be called as
// frequently as possible while using the gyro to do turns.
void turnSensorUpdate() {
  // Read the measurements from the gyro.
  imu.readGyro();
  imu.readAcc();
  turnRate = imu.g.z - gyroOffsetZ;
  tiltRate = -imu.g.y + gyroOffsetY;

  // Figure out how much time has passed since the last update (dt)
  uint16_t m = micros();
  uint16_t dt = m - gyroLastUpdate;
  gyroLastUpdate = m;

  // Multiply dt by turnRate in order to get an estimation of how
  // much the robot has turned since the last update.
  // (angular change = angular velocity * time)
  int32_t dZ = (int32_t)turnRate * dt;
  int32_t dY = (int32_t)tiltRate * dt;

  // The units of dZ are gyro digits times microseconds.  We need
  // to convert those to the units of turnAngle, where 2^29 units
  // represents 45 degrees.  The conversion from gyro digits to
  // degrees per second (dps) is determined by the sensitivity of
  // the gyro: 0.07 degrees per second per digit.
  //
  // (0.07 dps/digit) * (1/1000000 s/us) * (2^29/45 unit/degree)
  // = 14680064/17578125 unit/(digit*us)
  turnAngle += (int64_t)dZ * 14680064 / 17578125;
  uint32_t tiltAngleChange = (int64_t)dY * 14680064 / 17578125;


  double accAngleInDegrees = (atan2(imu.a.x, imu.a.z) * 180 / M_PI);
  uint32_t accAngleInMBU = (uint32_t)(accAngleInDegrees * MBUconstant);
  
  
  tiltAngle = (tiltAngle + tiltAngleChange)*0.9 + accAngleInMBU*0.1;
}