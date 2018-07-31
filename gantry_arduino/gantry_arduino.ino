#include<Stepper.h>

#define R2_STEP_PIN 54
#define R2_DIR_PIN 55
#define R2_ENABLE_PIN 38

#define Y1_STEP_PIN 60
#define Y1_DIR_PIN 61
#define Y1_ENABLE_PIN 56

#define R1_STEP_PIN 46
#define R1_DIR_PIN 48
#define R1_ENABLE_PIN 62

#define Y2_STEP_PIN 26
#define Y2_DIR_PIN 28
#define Y2_ENABLE_PIN 24

#define P_STEP_PIN 36
#define P_DIR_PIN 34
#define P_ENABLE_PIN 30

#define X_STEP_PIN 10
#define X_DIR_PIN 8

int numberOfSteps = 200;  //1.8 degree step size, means 200 steps per revolution
int StepCounter = 0;
int Stepping = true;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
enum Direction { FORWARD, BACKWORD } direction;

Stepper stepper_r1(numberOfSteps,R1_STEP_PIN,R1_DIR_PIN);
Stepper stepper_p(numberOfSteps,P_STEP_PIN,P_DIR_PIN);
Stepper stepper_r2(numberOfSteps,R2_STEP_PIN,R2_DIR_PIN);
Stepper stepper_y1(numberOfSteps,Y1_STEP_PIN,Y1_DIR_PIN);
Stepper stepper_y2(numberOfSteps,Y2_STEP_PIN,Y2_DIR_PIN);
Stepper stepper_x(numberOfSteps,X_STEP_PIN,X_DIR_PIN);

void setup() 
{
  Serial.begin(9600);
  Serial.println("Starting StepperTest");
  pinMode(R1_DIR_PIN, OUTPUT);
  pinMode(R1_STEP_PIN, OUTPUT);
  pinMode(R1_ENABLE_PIN,OUTPUT);
  
  pinMode(R2_DIR_PIN, OUTPUT);
  pinMode(R2_STEP_PIN, OUTPUT);
  pinMode(R2_ENABLE_PIN,OUTPUT);
  
  pinMode(Y1_DIR_PIN, OUTPUT);
  pinMode(Y1_STEP_PIN, OUTPUT);
  pinMode(Y1_ENABLE_PIN,OUTPUT);
  
  pinMode(Y2_DIR_PIN, OUTPUT);
  pinMode(Y2_STEP_PIN, OUTPUT);
  pinMode(Y2_ENABLE_PIN,OUTPUT);

  pinMode(P_DIR_PIN, OUTPUT);
  pinMode(P_STEP_PIN, OUTPUT);
  pinMode(P_ENABLE_PIN,OUTPUT);

  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_STEP_PIN, OUTPUT);
  
  digitalWrite(Y1_ENABLE_PIN, LOW);
  digitalWrite(Y2_ENABLE_PIN, LOW);
  digitalWrite(R1_ENABLE_PIN, LOW);
  digitalWrite(R2_ENABLE_PIN, LOW);
  digitalWrite(P_ENABLE_PIN, LOW);

  stepper_r1.setSpeed(40);
  stepper_p.setSpeed(10);
  stepper_r2.setSpeed(40);
  stepper_y1.setSpeed(40);
  stepper_y2.setSpeed(40);
  stepper_x.setSpeed(40);
}

void loop() 
{
  if(Serial.available()>0){
    inputString=Serial.readString();
    stringComplete=true; 
    Serial.flush();
  }
  
  if (stringComplete) {
      inputString.trim();
      String motor_to_run=getValue(inputString,',',0);
      if(motor_to_run == "r1"){
          direction = FORWARD;
          int steps_to_run=getValue(inputString,',',1).toInt();
          runStepperToPosition(direction,"r1",steps_to_run);
      }else if (motor_to_run == "-r1") {
          direction = BACKWORD;
          int steps_to_run=getValue(inputString,',',1).toInt();
          runStepperToPosition(direction,"r1",steps_to_run);
      }else if(motor_to_run == "p"){
          direction = FORWARD;
          int steps_to_run=getValue(inputString,',',1).toInt();
          runStepperToPosition(direction,"p",steps_to_run);
      }else if (motor_to_run == "-p") {
          direction = BACKWORD;
          int steps_to_run=getValue(inputString,',',1).toInt();
          runStepperToPosition(direction,"p",steps_to_run);
      }else if(motor_to_run == "r2"){
          direction = FORWARD;
          int steps_to_run=getValue(inputString,',',1).toInt();
          Serial.println("steps ="+String(steps_to_run));
          runStepperToPosition(direction,"r2",steps_to_run);
     }else if (motor_to_run == "-r2") {
          direction = BACKWORD;
          int steps_to_run=getValue(inputString,',',1).toInt();
          runStepperToPosition(direction,"r2",steps_to_run);
     }else if(motor_to_run == "y"){
          direction = FORWARD;
          int steps_to_run=getValue(inputString,',',1).toInt();
          runStepperToPosition(direction,"y",steps_to_run);
     }else if (motor_to_run == "-y") {
          direction = BACKWORD;
          int steps_to_run=getValue(inputString,',',1).toInt();
          runStepperToPosition(direction,"y",steps_to_run);
     }else if(motor_to_run == "z"){
          direction = FORWARD;
          int steps_to_run=getValue(inputString,',',1).toInt();
          runStepperToPosition(direction,"z",steps_to_run);
     }else if (motor_to_run == "-z") {
          direction = BACKWORD;
          int steps_to_run=getValue(inputString,',',1).toInt();
          runStepperToPosition(direction,"z",steps_to_run);
     }
  }
}


void runStepperToPosition(int direction, String axis, int steps_to_run)
{
   if(axis.equals("r1")){
        if(direction == BACKWORD){
            stepper_r1.step(-steps_to_run);
          }else{
            stepper_r1.step(steps_to_run);
          }
    }else if(axis.equals("p")){
        if(direction == BACKWORD){
            stepper_p.step(-steps_to_run);
          }else{
            stepper_p.step(steps_to_run);
          }
    }if(axis.equals("r2")){
        if(direction == BACKWORD){
            stepper_r2.step(-steps_to_run);
          }else{
            stepper_r2.step(steps_to_run);
          }
    }else if (axis.equals("y")) {
          for(int s=0; s<steps_to_run; s++)
            {
              if(direction == BACKWORD){
                stepper_y1.step(-1);
                stepper_y2.step(-1);
              }else{
                  stepper_y1.step(1);
                  stepper_y2.step(1);
                }
            }
    }else if (axis.equals("z")) {
        int steps_at_a_time=1;
        for(int s=0; s<steps_to_run; s++)
            {
              if(direction == BACKWORD){
                stepper_y1.step(-1);
                stepper_y2.step(1);
              }else{
                  stepper_y1.step(1);
                  stepper_y2.step(-1);
                }
            }  
    }
    inputString = "";
    stringComplete = false;
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


