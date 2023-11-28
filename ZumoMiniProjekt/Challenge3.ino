void challenge3() {
  setThresholds();

  while (true){
    proxSensors.read();
    int L = proxSensors.countsLeftWithLeftLeds();
    int R = proxSensors.countsRightWithRightLeds();
    
    int leftSpeed = baseSpeed3+K3*(L-R);
    int rightSpeed = baseSpeed3-K3*(L-R);

    motors.setSpeeds(leftSpeed, rightSpeed);
    lineSensors.read(lineSensorValues);

    if (lineSensorValues[2] > threshold[2]) {
      motors.setSpeeds(0,0);
      while(true){}
    }
  }
}
