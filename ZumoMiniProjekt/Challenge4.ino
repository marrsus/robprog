void challenge4() {
  travelDist = get_input(0,countJump,1,100,0,"TravelDist");
  rettning = 0;

  setThresholds();
  turnSensorSetup();
  turnSensorUpdate();
  

  motors.setSpeeds(speed, speed);
  while (lineSensorValues[0] < threshold[0] && lineSensorValues[2] < threshold[2] && lineSensorValues[4] < threshold[4]) {
    readLineSensors();
  }

  if (lineSensorValues[0] > threshold[0]) {
    rettning = 1;
  } else if (lineSensorValues[4] > threshold[4]) {
    rettning = 2;
  }

  resetEncoders();

  motors.setSpeeds(50, 50);
  while (lineSensorValues[2] < threshold[2]) {
    readLineSensors();
  }

  turnSensorReset();

  float A = (atan(getDistance() / space1) * 180) / PI;

  if (rettning == 1) {
    motors.setSpeeds(-speed, speed);
    while (getTurnAngleInDegrees() < A) {
      delay(10);
    }
  } else if (rettning == 2) {
    motors.setSpeeds(speed, -speed);
    while (getTurnAngleInDegrees() > 360 - A) {
      delay(10);
    }
  }

  resetEncoders();
  while (getDistance() < travelDist) {
    motors.setSpeeds(speed, speed);
  }
  bip();
  motors.setSpeeds(0, 0);
}

float getDistance() {
  int countsL = encoders.getCountsLeft();
  int countsR = encoders.getCountsRight();

  float distanceL = countsL / 900.0 * wheelCirc;
  float distanceR = countsR / 900.0 * wheelCirc;

  return (distanceL + distanceR) / 2;
}

