#include <AccelStepper.h>
#include <MultiStepper.h>

#include <Arduino.h>
#include "AccelStepper.h"
#include "MultiStepper.h"
#include "axisDirectionStruct.h" //importing AxisAndDirection
#define STEPPER_X_STEP_PIN 2
#define STEPPER_X_DIR_PIN 3

#define STEPPER_Z_STEP_PIN 6
#define STEPPER_Z_DIR_PIN 7

#define STEPPER_Y_STEP_PIN 9
#define STEPPER_Y_DIR_PIN 10
#define enable_x 5
#define led_pin 13
#define goodToGoX_pin 11
enum Direction { FORWARD, BACKWORD } direction;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void blockingRunSpeedToPosition(int direction, String axis);

AccelStepper stepperX(AccelStepper::DRIVER, STEPPER_X_STEP_PIN, STEPPER_X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, STEPPER_Y_STEP_PIN, STEPPER_Y_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, STEPPER_Z_STEP_PIN, STEPPER_Z_DIR_PIN);

MultiStepper steppers;

void setup()
{
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  pinMode(goodToGoX_pin,INPUT);
  pinMode(enable_x, OUTPUT);
  stepperX.setMaxSpeed(200.0);
  stepperX.setAcceleration(100);

  stepperY.setMaxSpeed(200.0);
  stepperY.setAcceleration(100);

  stepperZ.setMaxSpeed(200.0);
  stepperZ.setAcceleration(100);

  steppers.addStepper(stepperY);
  steppers.addStepper(stepperZ);
  digitalWrite(enable_x, HIGH);
}

void loop()
{
  int reading=digitalRead(goodToGoX_pin);
  if(reading==HIGH){
    digitalWrite(led_pin,HIGH);
   }else{
    digitalWrite(led_pin,LOW);
   }
  
  if (stringComplete) {
      Serial.println(inputString);
      // clear the string:
      inputString.trim();
      Serial.println(inputString.length());
      if(inputString == "x") {
          direction = FORWARD;
          blockingRunSpeedToPosition(direction,"x");
      } else if (inputString == "-x") {
          direction = BACKWORD;
          blockingRunSpeedToPosition(direction,"x");
      } else if (inputString =="y") {
          direction = FORWARD;
          blockingRunSpeedToPosition(direction,"y");
      } else if (inputString == "-y") {
          direction = BACKWORD;
          blockingRunSpeedToPosition(direction, "y");
      } else if(inputString == "z"){
          direction = FORWARD;
          blockingRunSpeedToPosition(direction, "z");
      }else if (inputString == "-z") {
          direction = BACKWORD;
          blockingRunSpeedToPosition(direction, "z");
      }
    }

}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
 void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      digitalWrite(enable_x,HIGH);
    }
  }
}

void blockingRunSpeedToPosition(int direction, String axis)
{
  /*
     runSpeedToPosition is non blocking.
     You must call this as frequently as possible, but at least once per step interval,
     But we want blocking so we have to implement own loop using while
   */
   

   Serial.println("got "+axis);
   int goodToGoX_status;
   int steps = 1200;
   if(direction == BACKWORD) {
       steps = -steps;
   }
   goodToGoX_status=digitalRead(goodToGoX_pin);
   ///stepper.move funtion : Set the target position relative to the current position
   /// if position is negative then anticlockwise from the current position, else clockwise from current position
   if(axis.equals("x")){
       stepperX.setCurrentPosition(0);
       stepperX.move(steps);
       while (stepperX.distanceToGo() != 0 && goodToGoX_status==HIGH) {
           stepperX.run();
        }
    }else if (axis.equals("y")) {
        long positions[2];
        positions[0]=steps;
        positions[1]=steps;
        stepperY.setCurrentPosition(0);
        stepperZ.setCurrentPosition(0);

        steppers.moveTo(positions);
        // while (stepperY.distanceToGo()!=0 && stepperZ.distanceToGo()!=0) {
        //     steppers.run();
        // }
        steppers.runSpeedToPosition();
    }else if (axis.equals("z")) {
        long positions[2];
        positions[0]=steps;
        positions[1]=-steps;
        stepperY.setCurrentPosition(0);
        stepperZ.setCurrentPosition(0);

        steppers.moveTo(positions);
        steppers.runSpeedToPosition();
        // while (stepperY.distanceToGo()!=0 && stepperZ.distanceToGo()!=0) {
        //     steppers.run();
        // }
    }
    inputString = "";
    stringComplete = false;
    digitalWrite(enable_x, LOW);
}

