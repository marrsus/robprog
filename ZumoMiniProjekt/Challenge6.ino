void challenge6() {
  angle = get_input(0,countJump,5,180,-180,"Angle");

  setThresholds();
  Align();
  turnSensorSetup();

  while(getTurnAngleInDegrees() < angle || getTurnAngleInDegrees() > 355){
    motors.setSpeeds(0,300);
  }

  while(lineSensorValues[0] < 900 && lineSensorValues[2] < 600 && lineSensorValues[4] < 900){
    readLineSensors();
    motors.setSpeeds(300,300);
  }
  motors.setSpeeds(0,0);
}