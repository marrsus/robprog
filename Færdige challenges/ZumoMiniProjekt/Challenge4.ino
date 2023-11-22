void challenge4Loop() {
  turnSensorSetup();
  turnSensorUpdate();
  while(true){
    switch (stage) {
      case 0:
        readLineSensors();
        selectDistance();
        break;
      case 1:
        
        while (lineSensorValues[0] < threshold4 && lineSensorValues[2] < threshold4 && lineSensorValues[4] < threshold4) {
        motors.setSpeeds(speed, speed);
        readLineSensors();
        }
        stop();
        if (lineSensorValues[0] > threshold4){
          rettning=1;
        }
        else if (lineSensorValues[4] > threshold4){
          rettning=2;
        }
        resetEncoders();
        
        delay(10);
        motors.setSpeeds(50, 50);
        stage++;
        break;
        
      case 2:
        while (lineSensorValues[2] < threshold4){
          readLineSensors();
        }
        stop();
        stage++;
        break;
      case 3:
        turning();
        break;
      case 4:
        goForward();
        break;
    }
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

void goForward() {
  resetEncoders();
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

void OLEDSelectMovement() {
  display.clear();
  switch (stage) {
    case 0:
      printSomething("TravelDist", String(travelDist));
      break;
    case 2:
      display.clear();
      printSomething("sensor(1)", String(lineSensorValues[2]));
      break;
    case 3:
      display.clear();
      printSomething("Angle", String(turningA()));
      break;
   case 4:
      display.clear();
      printSomething("Angle", String(getDistance()));
      break;
  }
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

uint32_t getTurnAngleInDegrees(){
  turnSensorUpdate();
  // do some math and pointer magic to turn angle in seconds to angle in degree
  return (((uint32_t)turnAngle >> 16) * 360) >> 16;
}

void turnSensorReset(){
  gyroLastUpdate = micros();
  turnAngle = 0;
}

// Read the gyro and update the angle.  This should be called as
// frequently as possible while using the gyro to do turns.
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