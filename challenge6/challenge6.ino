#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Encoders encoders;
Zumo32U4LCD display;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4IMU imu;
Zumo32U4LineSensors lineSensors;

#define NUM_SENSORS 3
unsigned int lineSensorValues[NUM_SENSORS];

int speed = 200;
int angle = 0;
int goTime = 0;
int movement = 0;
int countJump = 5;
uint32_t turnAngle = 0;
int16_t turnRate;
int16_t gyroOffset;
uint16_t gyroLastUpdate = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lineSensors.initThreeSensors();
  chooseAngle();
  //delay(1000);
  turnSensorSetup();
}

void printReadingsToSerial(){
  char buffer[80];
  sprintf(buffer, "%4d %4d %4d\n",
    lineSensorValues[0],
    lineSensorValues[1],
    lineSensorValues[2]
  );
  Serial.print(buffer);
}

void readLineSensors(){
  lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
  printReadingsToSerial();
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(goTime){
    case 0:
    driveTillStop();
    break;
    case 1:
    turn();
    break;
    case 2:
    driveTillStop();
    break;
  }
}

void jump(){
  movement = movement + encoders.getCountsRight()/10;
  resetEncoders();
}

void chooseAngle(){
  while(!buttonA.isPressed()){
    jump();
    if(movement > countJump){
      bip();
      movement = 0;
      angle = angle + 5;
      if(angle > 360) angle = 360;
    }
    else if(movement < -countJump){
      bip();
      movement = 0;
      angle = angle - 5;
      if(angle < 0 ) angle = 0;
    }
    resetEncoders();
    display.clear();
    display.print("Angle:");
    display.gotoXY(0,1);
    display.print(angle);
    delay(10);
  }
}

void driveTillStop(){
  readLineSensors();
  while(lineSensorValues[0] < 900 && lineSensorValues[1] < 600 && lineSensorValues[2] < 900){
    readLineSensors();
    printReadingsToSerial();
    forward();
  }
  stop();
  goTime++;
}

void turn(){
  while(getTurnAngleInDegrees() < angle || getTurnAngleInDegrees() > 355){
    display.clear();
    display.print("T: ");
    display.print(angle);
    display.gotoXY(0,1);
    display.print("A: ");
    display.print(getTurnAngleInDegrees());
    motors.setSpeeds(0,speed);
    printReadingsToSerial();
  }
  stop();
  goTime++;
}

void forward(){
  motors.setSpeeds(speed,speed);
}

void stop(){
  motors.setSpeeds(0,0);
}

void resetEncoders(){
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

/* 
Gyro setup and convenience functions '
*/
void turnSensorSetup(){
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
  display.clear();
  turnSensorReset();
}

void turnSensorReset(){
  gyroLastUpdate = micros();
  turnAngle = 0;
}

void turnSensorUpdate(){
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

uint32_t getTurnAngleInDegrees(){
  turnSensorUpdate();
  // do some math and pointer magic to turn angle in seconds to angle in degree
  return (((uint32_t)turnAngle >> 16) * 360) >> 16;
}

void bip(){
  buzzer.playNote(NOTE_A(4),20,15);}