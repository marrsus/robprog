void setup() {
  Serial.begin(9600);
  lineSensors.initFiveSensors();
  proxSensors.initThreeSensors();
  proxSensors.setBrightnessLevels(brightnessLevels, levels);
  delay(100);
}

//Copied from challenge 0
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Encoders encoders;
Zumo32U4OLED display;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;

void loop() {
  switch(stage){
    case 0: selectMovement();
    break;
  }
}

void selectMovement(){
  readEncodersMovement();
  if(movementCommand > countJump){
    bip();
    display.clear();
    movementCommand = 0;
    chosenCommand++;
    if(chosenCommand > 6) chosenCommand = 0;
  }
  else if(movementCommand < -countJump){
    bip();
    display.clear();
    movementCommand = 0;
    chosenCommand--;
    if(chosenCommand < 0) chosenCommand = 6;
  }
  lcdPrintMovement();
  if(buttonA.isPressed()){
    bip();
    stage = 1;
    buttonA.waitForRelease();
  }
}

void lcdPrintChosenCommand(String a){
  display.println("Command:");
  display.gotoXY(0, 1);
  display.println(a);
}

void lcdPrintMovement(){
  switch(chosenCommand){
    case 0:
      lcdPrintChosenCommand("Chllng 1");
      break;
    case 1:
      lcdPrintChosenCommand("Chllng 2");
      while(!buttonA.isPressed());
      bip();
      buttonA.waitForRelease();
      setThresholds();
      followLine();
      break;
    case 2:
      lcdPrintChosenCommand("Chllng 3");
      break;
    case 3:
      lcdPrintChosenCommand("Chllng 4");
      break;
    case 4:
      lcdPrintChosenCommand("Chllng 5");
      break;
    case 5:
      lcdPrintChosenCommand("Chllng 6");
      break;
    case 6:
      lcdPrintChosenCommand("Chllng 7");
      break;
  }
}

/*
Encoder functions
*/

void resetEncoders(){
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

void readEncodersMovement(){
  movementCommand = movementCommand + encoders.getCountsRight();
  resetEncoders();
}

void readEncodersParameter(){
  movementParameter = movementParameter + encoders.getCountsLeft();
  resetEncoders();
}

/*
Sound functions
*/

void bip(){
  buzzer.playNote(NOTE_A(4),20,15);
}