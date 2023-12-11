/*
void ogturnSensorSetup() {
  Wire.begin();
  imu.init();
  imu.enableDefault();
  imu.configureForTurnSensing();

  display.clear();
  display.print(F("Gyro cal"));

  // Turn on the yellow LED in case the LCD is not available.
  ledYellow(1);

  // Delay to give the user time to remove their finger.
  delay(500);

  // Calibrate the gyro.
  ogtotalZ = 0;
  ogtotalY = 0;
  for (uint16_t i = 0; i < 1024; i++) {
    // Wait for new data to be available, then read it.
    while (!imu.gyroDataReady()) {}
    imu.readGyro();

    // Add the Z axis reading to the total.
    ogtotalZ += imu.g.z;
    ogtotalY += imu.g.y;
  }
  ledYellow(0);
  oggyroOffsetZ = ogtotalZ / 1024;
  oggyroOffsetY = ogtotalY / 1024;

  // Display the angle (in degrees from -180 to 180) until the
  // user presses A.
  display.clear();
  ogturnSensorReset();
}

double oggetTurnAngleInDegrees() {
  ogturnSensorUpdate();
  // do some math and pointer magic to turn angle in seconds to angle in degree
  return (((double)ogturnAngle >> 16) * 360) >> 16;
}


double oggetTiltAngleInDegrees(bool updateSensor) {
  if (updateSensor) ogturnSensorUpdate();
  return ogtiltAngle*-1;
}

double oggetTiltAngleInDegrees() {
  ogturnSensorUpdate();
  // do some math and pointer magic to turn angle in seconds to angle in degree
  return (((double)ogtiltAngle >> 16) * 360) >> 16;
}

void ogturnSensorReset() {
  oggyroLastUpdate = millis();
  ogturnAngle = 0;
  ogtiltAngle = 0;
}

// Read the gyro and update the angle.  This should be called as
// frequently as possible while using the gyro to do turns.
void ogturnSensorUpdate() {
  // Read the measurements from the gyro.
  imu.readGyro();
  int16_t ogturnRateZ = imu.g.z - oggyroOffsetZ;
  int16_t ogturnRateY = imu.g.y - oggyroOffsetY;

  // Figure out how much time has passed since the last update (dt)
  uint16_t m = millis();
  uint16_t dt = m - oggyroLastUpdate;
  oggyroLastUpdate = m;

  // Multiply dt by turnRate in order to get an estimation of how
  // much the robot has turned since the last update.
  // (angular change = angular velocity * time)
  double dZ = (double)ogturnRateZ * (double)dt;
  double dY = (double)ogturnRateY*0.07 * (double)dt*0.001;

  // The units of d are gyro digits times microseconds.  We need
  // to convert those to the units of turnAngle, where 2^29 units
  // represents 45 degrees.  The conversion from gyro digits to
  // degrees per second (dps) is determined by the sensitivity of
  // the gyro: 0.07 degrees per second per digit.
  //
  // (0.07 dps/digit) * (1/1000000 s/us) * (2^29/45 unit/degree)
  // = 14680064/17578125 unit/(digit*us)
  ogturnAngle += dZ * 14680064 / 17578125;
  ogtiltAngle += dY;
}
*/