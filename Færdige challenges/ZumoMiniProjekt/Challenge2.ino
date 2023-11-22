void readLineSensors(){
  lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
}

void forward(){
  motors.setSpeeds(300,300);
}

void turnLeft(){
  motors.setSpeeds(0,600);
}

void turnSharpLeft(){
  motors.setSpeeds(-600,600);
}

void turnRight(){
  motors.setSpeeds(600,0);
}

void turnSharpRight(){
  motors.setSpeeds(600,-600);
}

void printSomething(String line1, String line2){
  display.clear();
  display.println(line1);
  display.gotoXY(0,1);
  display.println(line2);
}

void setThresholds(){
  int temp[10];
  printSomething("Off line", "Press A");
  while(!buttonA.isPressed()){}
  bip();
  readLineSensors();
  for(int i = 0; i<5; i++){
    temp[i] = lineSensorValues[i];
  }
  buttonA.waitForRelease();
  printSomething("On line", "Press A");
  while(!buttonA.isPressed()){}
  bip();
  readLineSensors();
  for(int i = 9; i>4; i--){
    temp[i] = lineSensorValues[i-5];
  }
  buttonA.waitForRelease();
  for(int i = 0; i<5; i++){
    threshold[i] = (temp[i]+temp[i+5])/2;
  }
  printSomething("Press A", "to start");
  while(!buttonA.isPressed()){}
  buttonA.waitForRelease();
  display.clear();
  display.println("GO!");
}

void followLine(){
  while(true){
    readLineSensors();
    if(lineSensorValues[0]>threshold[0]){
      turnSharpLeft();
    } else if(lineSensorValues[4]>threshold[4]){
      turnSharpRight();
    } else if((lineSensorValues[2]>threshold[2] || (lineSensorValues[1]>threshold[1] && lineSensorValues[2]>threshold[2]) || (lineSensorValues[2]>threshold[2] && lineSensorValues[3]>threshold[3])) && lineSensorValues[0]<threshold[0] && lineSensorValues[4]<threshold[4]){
      forward();
    } else if(lineSensorValues[1]>threshold[1] && lineSensorValues[2]<threshold[2]){
      turnLeft();
    } else if(lineSensorValues[2]<threshold[2] && lineSensorValues[3]>threshold[3]){
      turnRight();
    }
  }
}