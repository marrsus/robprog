void challenge2(){
  setThresholds();
  while(true){
    readLineSensors();
    if(lineSensorValues[0]>threshold[0]){
      motors.setSpeeds(-600,600); // turn sharp left 
    } else if(lineSensorValues[4]>threshold[4]){
      motors.setSpeeds(600,-600); // turn sharp right 
    } else if(lineSensorValues[2]>threshold[2] || (lineSensorValues[1]>threshold[1] && lineSensorValues[2]>threshold[2])){
      motors.setSpeeds(300,300); // forward
    } else if(lineSensorValues[1]>threshold[1]) {
      motors.setSpeeds(0,600); // turn left
    } else if(lineSensorValues[3]>threshold[3]){
      motors.setSpeeds(600,0); // trun right
    }
  }
}