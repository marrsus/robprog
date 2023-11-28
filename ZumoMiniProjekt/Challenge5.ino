void challenge5() {
  encoderThreshFirst = get_input(2850,1,1,10000,1,"enc ths1");
  encoderThresh = get_input(3400,1,1,10000,1,"enc ths");
  distancePoleSwitch = get_input(1400,1,1,10000,1,"dist p s");
  k5 = get_input(15,countJump,1,10000,1,"k5");

  //Turn right 180*
  encoders.getCountsAndResetLeft();
  while(true) {
    proxSensors.read();
    revolve(1, proxSensors.countsRightWithRightLeds());
    if(encoders.getCountsLeft() > encoderThreshFirst) break;
  }

  while(true){
    changePole();

    //Revolve around right pole
    encoders.getCountsAndResetRight();
    while(true){
      proxSensors.read();
      revolve(-1, proxSensors.countsLeftWithLeftLeds());
      readLineSensors();
      if(lineSensorValues[0] > 600 || lineSensorValues[2] > 600 || lineSensorValues[4] > 600) encoders.getCountsAndResetRight();
      if(encoders.getCountsRight() > encoderThresh) break;
    }
    changePole();

    //Revolve around left pole
    encoders.getCountsAndResetLeft();
    while(true){
      proxSensors.read();
      revolve(1, proxSensors.countsRightWithRightLeds());
      readLineSensors();
      if(lineSensorValues[0] > 600 || lineSensorValues[2] > 600 || lineSensorValues[4] > 600) encoders.getCountsAndResetLeft();
      if(encoders.getCountsLeft() > encoderThresh) break;
    }
  }
}

void changePole() {
  motors.setSpeeds(baseSpeed5, baseSpeed5);
  encoders.getCountsAndResetRight();
  while(encoders.getCountsRight() < distancePoleSwitch) {}
}

//dir 1 = right  dir -1 = left
//proxSensorCount = counts from sensor pointing towards revolved object

void revolve(int dir, int proxSensorCount) { 
  int leftSpeed = baseSpeed5+dir*proxSensorCount*k5;
  int rightSpeed = baseSpeed5-dir*proxSensorCount*k5;
  motors.setSpeeds(leftSpeed, rightSpeed);
}