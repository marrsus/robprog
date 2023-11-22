
void challenge3Setup() {
  /*while(!buttonA.getSingleDebouncedPress()){
    lineSensors.read(lineSensorValues);
    display.clear();
    display.gotoXY(0, 0);
    display.print("cal");
    display.gotoXY(0, 1);
    display.print(lineSensorValues[2]);
    delay(100);
  }
  lineSensors.read(lineSensorValues);
  int surface = lineSensorValues[2];*/
  while(!buttonA.getSingleDebouncedPress()){
    lineSensors.read(lineSensorValues);
    display.clear();
    display.gotoXY(0, 0);
    display.print("tape");
    display.gotoXY(0, 1);
    display.print(lineSensorValues[2]);
    delay(100);
  }
  thresh = lineSensorValues[2]-50;
  display.clear();
  display.gotoXY(0, 0);
  display.print("Press");
  display.gotoXY(0, 1);
  display.print("A");
  while(!buttonA.getSingleDebouncedPress()){}
}

void challenge3Loop() {
  while(true){
    proxSensors.read();
    int L = proxSensors.countsLeftWithLeftLeds();
    int R = proxSensors.countsRightWithRightLeds();
    
    int leftSpeed = baseSpeed3+K3*(L-R);
    int rightSpeed = baseSpeed3-K3*(L-R);

    motors.setSpeeds(leftSpeed, rightSpeed);
    lineSensors.read(lineSensorValues);

    display.clear();
    display.gotoXY(0,0);
    display.print(L);
    //display.print(lineSensorValues[2]);
    display.gotoXY(6,0);
    display.print(R);
    //display.print(thresh);
    display.gotoXY(0, 1);
    display.print(leftSpeed);
    display.gotoXY(5, 1);
    display.print(rightSpeed);

    if (lineSensorValues[2] > thresh) {
      motors.setSpeeds(0,0);
      while(true){}
    }
  }
}
