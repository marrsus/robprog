#include <Wire.h>
#include <Zumo32U4.h>
#include <math.h>

Zumo32U4Motors motors;
Zumo32U4Encoders encoders;
Zumo32U4LineSensors lineSensors;
Zumo32U4OLED oled;
Zumo32U4IMU imu;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;

int movementCommand = 0;
int movementParameter = 0;

int speed = 100;
int threshold = 500;

int const countJump = 100;
int stage = 0;
int travelDist = 0;
float wheelCirc = 12.6;
int chosenCommand = 0;
const float space1 = 4.4589;
bool lineSensorDetected = false;

#define NUM_SENSORS 3
uint16_t lineSensorValues[3];

//float turningAngle = 0; // Declare turningAngle globally

uint32_t turnAngle = 0;
int16_t turnRate;
int16_t gyroOffset;
uint16_t gyroLastUpdate = 0;
int rettning=0;

void setup() {
  Serial.begin(9600);
  delay(100);
  lineSensors.initThreeSensors();
  turnSensorSetup();
  turnSensorUpdate();
}

void loop() {
  switch (stage) {
    case 0:
      readLineSensors();
      selectDistance();
      break;
    case 1:
      
      while (lineSensorValues[0] < threshold && lineSensorValues[1] < threshold && lineSensorValues[2] < threshold) {
      motors.setSpeeds(speed, speed);
      readLineSensors();
      }
      stop();
      if (lineSensorValues[0] > threshold){
        rettning=1;
      }
      else if (lineSensorValues[2] > threshold){
        rettning=2;
      }
      resetEncoders();
      
      delay(10);
      motors.setSpeeds(50, 50);
      stage++;
      break;
      
    case 2:
      while (lineSensorValues[1] < threshold){
        readLineSensors();
      }
      stop();
      stage++;
      break;
    case 3:
      turning();
      break;
    case 4:
      forward();
      break;
  }
}


void selectDistance() {
  readEncodersParameter();
  if (movementParameter > 10) {
    travelDist++;
    movementParameter = 0;
    if (travelDist > 100) {
      travelDist = 0;
    }
  } else if (movementParameter < -10) {
    travelDist--;
    movementParameter = 0;
    if (travelDist < 0) {
      travelDist = 100;
    }
  }
  OLEDSelectMovement();
  if (buttonA.isPressed()) {
    bip();
    stage = 1;
    buttonA.waitForRelease();
  }
}

float turningA(){
  float A=(atan(getDistance() / space1) * 180) / PI;
  Serial.println(A);
  return A;
  
}




void turning() {
  turnSensorReset();
  readLineSensors();

  if (rettning==1) {

    motors.setSpeeds(-speed, speed);
    delay(10);
    while (getTurnAngleInDegrees() < turningA()) {
      delay(10);
    }
  }
  else if (rettning==2){
    motors.setSpeeds(speed, -speed);
    delay(10);
    while (getTurnAngleInDegrees() > 360-turningA()) {
      delay(10);
    }
  }
  stop();
  OLEDSelectMovement();
  delay(100);
  stage++;
}




void forward() {
  resetEncoders();
 // countdown();
  OLEDSelectMovement();
  while (getDistance() < travelDist) {
    motors.setSpeeds(speed, speed);
  }
  stop();
  bip();
  stage = 0;
  travelDist =0;
  rettning=0;
}

void stop() {
  motors.setSpeeds(0, 0);
}

void bip() {
  buzzer.playNote(NOTE_A(4), 20, 15);
  delay(30);
}



void OLEDSelectMovement() {
  oled.clear();
  switch (stage) {
    case 0:
      printOLED("TravelDist", String(travelDist));
      break;
    case 2:
      oled.clear();
      printOLED("sensor(1)", String(lineSensorValues[1]));
      break;
    case 3:
      oled.clear();
      printOLED("Angle", String(turningA()));
      break;
   case 4:
      oled.clear();
      printOLED("Angle", String(getDistance()));
      break;
  }
}


void countdown() {
  delay(800);
  printOLED("Coun", "Down");
  buzzer.playNote(NOTE_E(5), 150, 15); // Mario tone
  delay(400);
  printOLED("Cou", "Dow");
  buzzer.playNote(NOTE_D(5), 150, 15); // Mario tone
  delay(400);
  printOLED("Co", "Do");
  buzzer.playNote(NOTE_C(5), 150, 15); // Mario tone
  delay(400);
  printOLED("C", "D");
  buzzer.playNote(NOTE_G(4), 150, 15); // Mario tone
  delay(400);
  printOLED(" !STEP!", " !ASIDE!");
  buzzer.playNote(NOTE_G(4), 800, 15); // Mario tone
}


void resetEncoders() {
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

void readEncodersParameter() {
  movementParameter = movementParameter + encoders.getCountsLeft();
  resetEncoders();
}

float getDistance() {
  int countsL = encoders.getCountsLeft();
  int countsR = encoders.getCountsRight();

  float distanceL = countsL / 900.0 * wheelCirc;
  float distanceR = countsR / 900.0 * wheelCirc;

  return (distanceL + distanceR) / 2;
}


void printOLED(String s0, String s1) {
  oled.clear();
  oled.print(s0);
  oled.gotoXY(0, 1);
  oled.print(s1);
}

uint32_t getTurnAngleInDegrees(){
  turnSensorUpdate();
  // do some math and pointer magic to turn angle in seconds to angle in degree
  return (((uint32_t)turnAngle >> 16) * 360) >> 16;
}

void turnSensorReset()
{
  gyroLastUpdate = micros();
  turnAngle = 0;
}

// Read the gyro and update the angle.  This should be called as
// frequently as possible while using the gyro to do turns.
void turnSensorUpdate()
{
  // Read the measurements from the gyro.
  imu.readGyro();
  turnRate = imu.g.z - gyroOffset;

  // Figure out how much time has passed since the last update (dt)
  uint16_t m = micros();
  uint16_t dt = m - gyroLastUpdate;
  gyroLastUpdate = m;

  // Multiply dt by turnRate in order to get an estimation of how
  // much the robot has turned since the last update.
  // (angular change = angular velocity * time)
  int32_t d = (int32_t)turnRate * dt;

  // The units of d are gyro digits times microseconds.  We need
  // to convert those to the units of turnAngle, where 2^29 units
  // represents 45 degrees.  The conversion from gyro digits to
  // degrees per second (dps) is determined by the sensitivity of
  // the gyro: 0.07 degrees per second per digit.
  //
  // (0.07 dps/digit) * (1/1000000 s/us) * (2^29/45 unit/degree)
  // = 14680064/17578125 unit/(digit*us)
  turnAngle += (int64_t)d * 14680064 / 17578125;
}

void turnSensorSetup()
{
  Wire.begin();
  imu.init();
  imu.enableDefault();
  imu.configureForTurnSensing();

  oled.clear();
  oled.print(F("Gyro cal"));

  // Turn on the yellow LED in case the LCD is not available.
  ledYellow(1);

  // Delay to give the user time to remove their finger.
  delay(500);

  // Calibrate the gyro.
  int32_t total = 0;
  for (uint16_t i = 0; i < 1024; i++)
  {
    // Wait for new data to be available, then read it.
    while(!imu.gyroDataReady()) {}
    imu.readGyro();

    // Add the Z axis reading to the total.
    total += imu.g.z;
  }
  ledYellow(0);
  gyroOffset = total / 1024;

  // Display the angle (in degrees from -180 to 180) until the
  // user presses A.
  oled.clear();
  turnSensorReset();
}


void printReadingsToSerial()
{
  char buffer[80];
  sprintf(buffer, "%4d %4d %4d\n",
    lineSensorValues[0],
    lineSensorValues[1],
    lineSensorValues[2]
  );
  Serial.print(buffer);
}

// the uncalibrated line sensor reading are between 0 (very bright) and 2000 (very dark)
void readLineSensors(){
  lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
  printReadingsToSerial();
}

