// Function to execute a specific robot challenge
void challenge4() {
  // Get user input for travel distance, set initial direction to 0
  travelDist = get_input(0, countJump, 1, 100, 0, "TravelDist");
  rettning = 0;

  // Set threshold values for line sensors
  setThresholds();

  // Set up the turn sensor
  turnSensorSetup();
  // Update turn sensor readings
  turnSensorUpdate();

  // Set motor speeds for straight-line motion
  motors.setSpeeds(speed, speed);
  
  // Move forward until any of the three middle line sensors detect the line
  while (lineSensorValues[0] < threshold[0] && lineSensorValues[2] < threshold[2] && lineSensorValues[4] < threshold[4]) {
    readLineSensors();
  }

  // Determine the direction based on which side of the line is detected
  if (lineSensorValues[0] > threshold[0]) {
    rettning = 1;  // Left side of the line detected
  } else if (lineSensorValues[4] > threshold[4]) {
    rettning = 2;  // Right side of the line detected
  }

  // Reset encoder counts
  resetEncoders();

  // Set motor speeds for turning
  motors.setSpeeds(50, 50);
  // Turn until the middle line sensor detects the line
  while (lineSensorValues[2] < threshold[2]) {
    readLineSensors();
  }

  // Reset the turn sensor
  turnSensorReset();

  // Calculate angle (A) based on the distance to a predefined space1
  float A = (atan(getDistance() / space1) * 180) / PI;

  // Execute turn based on the detected direction
  if (rettning == 1) {
    motors.setSpeeds(-speed, speed);
    // Continue turning until the desired angle (A) is reached
    while (getTurnAngleInDegrees() < A) {
      delay(10);
    }
  } else if (rettning == 2) {
    motors.setSpeeds(speed, -speed);
    // Continue turning until the desired angle (A) is reached
    while (getTurnAngleInDegrees() > 360 - A) {
      delay(10);
    }
  }

  // Reset encoder counts
  resetEncoders();
  // Move forward until the specified travel distance is reached
  while (getDistance() < travelDist) {
    motors.setSpeeds(speed, speed);
  }
  
  // Perform a sound signal (e.g., beep) to indicate completion of the challenge
  bip();
  
  // Stop the motors
  motors.setSpeeds(0, 0);
}

// Function to calculate the distance traveled based on encoder counts
float getDistance() {
  // Get left and right encoder counts
  int countsL = encoders.getCountsLeft();
  int countsR = encoders.getCountsRight();

  // Calculate distances traveled by each wheel
  float distanceL = countsL / 900.0 * wheelCirc;
  float distanceR = countsR / 900.0 * wheelCirc;

  // Return the average distance traveled
  return (distanceL + distanceR) / 2;
}
