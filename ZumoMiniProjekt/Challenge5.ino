void challenge5Setup() {
  while(!buttonA.getSingleDebouncedPress()){
    encoderThreshFirst = 2850 + encoders.getCountsLeft();
    encoderThresh = 3400 + encoders.getCountsRight();
    display.clear();
    display.gotoXY(0,0);
    display.print(encoderThreshFirst);
    display.gotoXY(0,1);
    display.print(encoderThresh);
    delay(50);
  }
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  while(!buttonA.getSingleDebouncedPress()){
    distancePoleSwitch = 1400 + encoders.getCountsLeft();
    k5 = 15 + encoders.getCountsRight()/50;
    display.clear();
    display.gotoXY(0,0);
    display.print(distancePoleSwitch);
    display.gotoXY(0,1);
    display.print(k5);
    delay(50);
  }
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  while(!buttonA.getSingleDebouncedPress()){
    proxSensors.read();
    int L = proxSensors.countsLeftWithLeftLeds();
    int R = proxSensors.countsRightWithRightLeds();
    display.clear();
    display.gotoXY(0,0);
    display.print(L);
    display.gotoXY(0,1);
    display.print(encoders.getCountsLeft());
    display.gotoXY(6,1);
    display.print(R);
    display.gotoXY(4,0);
    display.print(encoders.getCountsRight());
  }

  //Turn right 180*
  encoders.getCountsAndResetLeft();
  while(true) {
    proxSensors.read();
    int L = proxSensors.countsLeftWithLeftLeds();
    int R = proxSensors.countsRightWithRightLeds();
    revolve(1, R);
    display.clear();
    display.gotoXY(0,0);
    display.print(L);
    display.gotoXY(0,1);
    display.print(encoders.getCountsLeft());
    if(encoders.getCountsLeft() > encoderThreshFirst) break;
  }
}

void challenge5Loop() {
  while(true){
    changePole();

    //Revolve around right pole
    encoders.getCountsAndResetRight();
    while(true){
      proxSensors.read();
      int L = proxSensors.countsLeftWithLeftLeds();
      int R = proxSensors.countsRightWithRightLeds();
      revolve(-1, L);
      lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
      if(lineSensorValues[0] > 600 || lineSensorValues[2] > 600 || lineSensorValues[4] > 600) encoders.getCountsAndResetRight();
      if(encoders.getCountsRight() > encoderThresh) break;
      display.clear();
      display.gotoXY(0,0);
      display.print(L);
      display.gotoXY(0,1);
      display.print(encoders.getCountsRight());
    }
    changePole();

    //Revolve around left pole
    encoders.getCountsAndResetLeft();
    while(true){
      proxSensors.read();
      int L = proxSensors.countsLeftWithLeftLeds();
      int R = proxSensors.countsRightWithRightLeds();
      revolve(1, R);
      lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
      if(lineSensorValues[0] > 600 || lineSensorValues[2] > 600 || lineSensorValues[4] > 600) encoders.getCountsAndResetLeft();
      if(encoders.getCountsLeft() > encoderThresh) break;
      display.clear();
      display.gotoXY(0,0);
      display.print(R);
      display.gotoXY(0,1);
      display.print(encoders.getCountsLeft());
    }
  }
}

void changeToRightPole() {
  motors.setSpeeds(baseSpeed5, baseSpeed5*2);
  while(true) {
    proxSensors.read();
    int L = proxSensors.countsLeftWithLeftLeds();
    int R = proxSensors.countsRightWithRightLeds();
    display.gotoXY(0,0);
    display.print(L);
    if(L >= 11 || (R == 0 && L == 0)) break;
  }
  delay(800);
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