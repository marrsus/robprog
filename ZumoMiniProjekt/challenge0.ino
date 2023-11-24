void loop() {
  selectChallenge();
}

void selectChallenge(){
  readEncodersMovement();
  if(movementCommand0 > countJump){
    bip();
    display.clear();
    movementCommand0 = 0;
    chosenChallenge++;
    if(chosenChallenge > 6) chosenChallenge = 0;
  }
  else if(movementCommand0 < -countJump){
    bip();
    display.clear();
    movementCommand0 = 0;
    chosenChallenge--;
    if(chosenChallenge < 0) chosenChallenge = 6;
  }
  Challenges();
}

void lcdPrintChosenChallenge(String a){
  display.println("Command:");
  display.gotoXY(0, 1);
  display.println(a);
}

void Challenges(){
  switch(chosenChallenge){
    case 0:
      lcdPrintChosenChallenge("Chllng 1");
      if(buttonA.isPressed()){
        bip();
        buttonA.waitForRelease();
        challenge1();
      }
      break;
    case 1:
      lcdPrintChosenChallenge("Chllng 2");
      if(buttonA.isPressed()){
        bip();
        buttonA.waitForRelease();
        setThresholds();
        followLine();
      }
      break;
    case 2:
      lcdPrintChosenChallenge("Chllng 3");
      if(buttonA.isPressed()){
        bip();
        buttonA.waitForRelease();
        challenge3Setup();
        challenge3Loop();
      }
      break;
    case 3:
      lcdPrintChosenChallenge("Chllng 4");
      if(buttonA.isPressed()){
        bip();
        buttonA.waitForRelease();
        challenge4Loop();
      }
      break;
    case 4:
      lcdPrintChosenChallenge("Chllng 5");
      if(buttonA.isPressed()){
        bip();
        buttonA.waitForRelease();
        challenge5Setup();
        challenge5Loop();
      }
      break;
    case 5:
      lcdPrintChosenChallenge("Chllng 6");
      if(buttonA.isPressed()){
        bip();
        buttonA.waitForRelease();
        challenge6Loop();
      }
      break;
    case 6:
      lcdPrintChosenChallenge("Chllng 7");
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
  movementCommand0 = movementCommand0 + encoders.getCountsRight();
  resetEncoders();
}

/*
Sound functions
*/

void bip(){
  buzzer.playNote(NOTE_A(4),20,15);
}