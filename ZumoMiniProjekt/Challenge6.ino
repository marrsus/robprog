void challenge6Loop() {
  setThresholds();
  chooseAngle();
  //while(true){
    //switch(goTime){
      //case 0:
      motors.setSpeeds(70, 70);
      Align();
      //goTime++;
      //break;
      //case 1:
      turnSensorSetup();
      turn();
      //break;
      //case 2:
      driveTillStop();
      //break;
    //}
  //}
}

void jump(){
  movement = movement + encoders.getCountsRight();
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
  while(lineSensorValues[0] < 900 && lineSensorValues[2] < 600 && lineSensorValues[4] < 900){
    readLineSensors();
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
    motors.setSpeeds(0,300);
  }
  stop();
  goTime++;
}