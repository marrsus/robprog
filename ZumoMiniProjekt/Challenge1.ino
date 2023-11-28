void challenge1() {
  proxSensors.setPeriod(500);
  proxSensors.setBrightnessLevels(levels1, levelCount);
  setThresholds();

  int threshold1 = get_input(1,100,1,10,1,"dist");
  Align();
  
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

