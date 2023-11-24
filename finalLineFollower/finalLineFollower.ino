#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer;
Zumo32U4LCD display;

#define NUM_SENSORS 5
unsigned int lineSensorValues[NUM_SENSORS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lineSensors.initFiveSensors();
  setThresholds();
}

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

int threshold[5];

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
  for(int i=0; i<10; i++){
    Serial.print(i);
    Serial.print(":");
    Serial.print(temp[i]);
    Serial.print(" ");
  }
  printThresholds();
  while(!buttonA.isPressed()){}
  buttonA.waitForRelease();
  display.clear();
  display.println("GO!");
}

void printThresholds(){
  for(int i=0;i<5;i++){
    Serial.print(threshold[i]);
    Serial.print(" ");
  }
}

void followLine(){
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

void printReadingsToSerial(){
  char buffer[80];
  sprintf(buffer, "%4d %4d %4d %4d %4d\n",
    lineSensorValues[0],
    lineSensorValues[1],
    lineSensorValues[2],
    lineSensorValues[3],
    lineSensorValues[4]
  );
  Serial.print(buffer);
}

void loop() {
  // put your main code here, to run repeatedly:
  //forward();
  readLineSensors();
  printReadingsToSerial();
  followLine();
}

void bip(){
  buzzer.playNote(NOTE_A(4),20,15);
}