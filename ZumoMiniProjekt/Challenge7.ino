void challenge7() {
  setThresholds();
  motors.setSpeeds(70, 70);
  Align();
  motors.setSpeeds(70, 70);
  delay(1000);
  Stop();
  motors.setSpeeds(0, 0);
}

void Stop() {
  flow = 0;
  turnRate = 0;
  delay(100);
  countsLeft = encoders.getCountsAndResetLeft();
  countsRight = encoders.getCountsAndResetRight();
  while (1) {
    readLineSensors();
    if (lineSensorValues[3] > threshold[3] || lineSensorValues[1] > threshold[1]) {
      motors.setSpeeds(0, 0);
      display.println("yep");
      break;
    } else if (countsLeft < countsRight) {
      motors.setSpeeds(90, 60);
    } else if (countsRight < countsLeft) {
      motors.setSpeeds(60, 90);
    } else {
      motors.setSpeeds(60, 60);
    }
    countsRight = encoders.getCountsAndResetRight();
    countsLeft = encoders.getCountsAndResetLeft();
  }
}