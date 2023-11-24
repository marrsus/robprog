void challenge1() {
  //proxSensors.initFrontSensor();
  proxSensors.setPeriod(500);
  proxSensors.setBrightnessLevels(levels1, levelCount);
  setThresholds();
  chal1();
}

void chal1(){
  motors.setSpeeds(70, 70);
  Align();
  stopafter(2);
}

void Align() {
  while (1) {

    readLineSensors();
    if (lineSensorValues[3] < threshold[3] && lineSensorValues[1] < threshold[1]) {
      continue;
    }

    if (lineSensorValues[3] > lineSensorValues[1]) {
      if (flag1 == true) {if (passes < 6){passes++;}} else {flag2 = false;}
      flag2 = true;
      motors.setSpeeds(-50, -130);
      delay(100 - (10*passes));

    } else if (lineSensorValues[3] < lineSensorValues[1]) {
      if (flag2 == true) {if (passes < 6){passes++;}} else {flag1 = false;}
      flag1 = true;
      motors.setSpeeds(-130, -50);
      delay(100 - (10*passes));
    } else{
      motors.setSpeeds(0,0);
      return;
    }
    motors.setSpeeds(60,60);
  }
}

void stopafter(int threshold1) {
  while (true) {
    motors.setSpeeds(90, 90);
    proxSensors.read();
    int read = proxSensors.countsFrontWithLeftLeds();
    if (read < threshold1 && read != 0) {
      motors.setSpeeds(0,0);
      return;
    }
  }
}
