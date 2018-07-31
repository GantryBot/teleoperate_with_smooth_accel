//#include<Stepper.h>

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

#define X_STEP_PIN 20 // 4th pin
#define X_DIR_PIN 21  // 2nd from bottom most
#define X_ENABLE_PIN 13

#define LED_PIN 13

int numberOfSteps = 200;
float current_location, location_needed, location_requested, location_change;
long steps_current, steps_needed;
long motor_delay=5000;    //1000microseconds = 1ms, this delay is for max speed
long motor_delay_long= 7500; //this delay is for slow speed
long smoothing_delay=5000; //smoothing delay to add delay at start of pulse and end of pulse
long smoothing_steps=50; 
//long pulsewidth=250; //in microseconds
long pulsewidth=10000; //in microseconds

int StepCounter = 0;
int Stepping = true;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
enum Direction { FORWARD, BACKWORD } direction;
boolean stop_motor=false;
enum MotorDirection {
  MotorDirectionForward = 1,
  MotorDirectionReverse = 0
};

class Motor {
private:
  int motor_pin_1;
  int motor_pin_2;
  int motor_direction; // the direction the motor is driving
  unsigned long step_delay; //  delay between steps
  int current_step;   // which step the motor is currently on
  unsigned long last_step_time; // the last time the motor was stepped
  int number_of_steps; //total number of steps this motor can take
  
  void stepMotor(int this_step);
  void doStep(int current_step_val);
public:
  Motor(int number_of_steps,int motor_pin_1, int motor_pin_2) 
  {
    this->current_step = 0;    // which step the motor is on
    this->motor_direction = 1;      // motor direction
    this->last_step_time = 0; // time stamp in us of the last step taken
    this->number_of_steps = number_of_steps; // total number of steps for this motor

    // Arduino pins for the motor control connection:
    this->motor_pin_1 = motor_pin_1;
    this->motor_pin_2 = motor_pin_2;

    // setup the pins on the microcontroller:
    pinMode(this->motor_pin_1, OUTPUT);
    pinMode(this->motor_pin_2, OUTPUT);

  }
  void motorSpeed(long whatSpeed);
  void moveMotor(int number_of_steps);
  void doStep();
};

void Motor::motorSpeed(long whatSpeed)
 {
  this->step_delay = 60L * 1000L / this->number_of_steps / whatSpeed;
 }

void Motor::moveMotor(int steps_to_move){
   int steps_left = abs(steps_to_move);  // how many steps to take

    // determine direction based on whether steps_to_mode is + or -:
    if (steps_to_move > 0) { this->motor_direction = 1; }
    if (steps_to_move < 0) { this->motor_direction = 0; }

    while (steps_left > 0)
    {
      unsigned long now = micros();
      int delay_value=(int)this->step_delay;
      if (this->motor_direction == 1)
      {
        digitalWrite(this->motor_pin_2, LOW);
        digitalWrite(this->motor_pin_1, HIGH);
        delay(delay_value); 
        digitalWrite(this->motor_pin_1, LOW); 
      }else{
        digitalWrite(this->motor_pin_2, HIGH);
        digitalWrite(this->motor_pin_1, HIGH);
        delay(delay_value); 
        digitalWrite(this->motor_pin_1, LOW); 
        }
        steps_left--;
     }
}


Motor *y1 = NULL, *y2 = NULL;
void setup() 
{
  Serial.begin(9600);
  Serial.println("Acceleration test start");
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
  pinMode(X_ENABLE_PIN, OUTPUT);

  pinMode(LED_PIN,OUTPUT);
  
  digitalWrite(Y1_ENABLE_PIN, LOW);
  digitalWrite(Y2_ENABLE_PIN, LOW);
  digitalWrite(R1_ENABLE_PIN, LOW);
  digitalWrite(R2_ENABLE_PIN, LOW);
  digitalWrite(P_ENABLE_PIN, LOW);
  digitalWrite(X_ENABLE_PIN, LOW);

  y1 = new Motor(200,Y1_STEP_PIN,Y1_DIR_PIN);
  y2 = new Motor(200,Y2_STEP_PIN,Y2_DIR_PIN);
  y1->motorSpeed(60);
  y2->motorSpeed(60);
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
      int steps_to_run=long(getValue(inputString,',',1).toInt()); 
      if(motor_to_run=="r2"){
        digitalWrite(R2_ENABLE_PIN, LOW);
        digitalWrite(R2_DIR_PIN,LOW);
        motor_accl_decl(R2_STEP_PIN, steps_to_run, 1000, 5000, 25,2500);
        stringComplete=false;
        inputString="";
//        digitalWrite(R2_ENABLE_PIN, LOW);
//        digitalWrite(R2_DIR_PIN,LOW);
//        digitalWrite(R2_STEP_PIN, HIGH);
//        delay(5); 
//        digitalWrite(R2_STEP_PIN, LOW); 
//        delay(5); 
//        StepCounter = StepCounter + 1;
//        if (StepCounter == steps_to_run)
//          {
//            digitalWrite(R2_ENABLE_PIN, HIGH);
//            StepCounter = 0;
//            stringComplete=false;
//            Serial.flush();
//          }
       }else if(motor_to_run=="-r2"){
        digitalWrite(R2_ENABLE_PIN, LOW);
        digitalWrite(R2_DIR_PIN,HIGH);
        motor_accl_decl(R2_STEP_PIN, steps_to_run, 1000, 5000, 25,2500);
        stringComplete=false;
        inputString="";

//      direction = BACKWORD;
//        digitalWrite(R2_ENABLE_PIN, LOW);
//        digitalWrite(R2_DIR_PIN,HIGH);
//        digitalWrite(R2_STEP_PIN, HIGH);
//        delay(5); 
//        digitalWrite(R2_STEP_PIN, LOW); 
//        delay(5); 
//        StepCounter = StepCounter + 1;
//        if (StepCounter == steps_to_run)
//          {
//            digitalWrite(R2_ENABLE_PIN, HIGH);
//            StepCounter = 0;
//            stringComplete=false;
//            Serial.flush();
//          }
      }else if(motor_to_run=="r1"){
        digitalWrite(R1_DIR_PIN,LOW);
        motor_accl_decl(R1_STEP_PIN, steps_to_run, pulsewidth, 2*smoothing_delay, 25,motor_delay_long);
        stringComplete=false;
        inputString="";
      }else if(motor_to_run=="-r1"){
        digitalWrite(R1_DIR_PIN,HIGH);
        motor_accl_decl(R1_STEP_PIN, steps_to_run, pulsewidth, 2*smoothing_delay, 25,motor_delay_long);
        stringComplete=false;
        inputString="";
      }else if(motor_to_run=="p"){
        digitalWrite(P_DIR_PIN,LOW);
        motor_accl_decl(P_STEP_PIN, steps_to_run, pulsewidth, smoothing_delay, smoothing_steps/2,motor_delay_long);
        stringComplete=false;
        inputString="";
      }else if(motor_to_run=="-p"){
        digitalWrite(P_DIR_PIN,HIGH);
        motor_accl_decl(P_STEP_PIN, steps_to_run, pulsewidth, smoothing_delay, smoothing_steps/2,motor_delay_long);
        stringComplete=false;
        inputString="";
      }else if(motor_to_run=="y"){
        int steps=(int)steps_to_run;
        move_y_z("y",steps);
      }else if(motor_to_run=="-y"){
        int steps=(int)steps_to_run;
        move_y_z("-y",steps);
      }else if(motor_to_run=="z"){
        int steps=(int)steps_to_run;
        move_y_z("z",steps);
      }else if(motor_to_run=="-z"){
        int steps=(int)steps_to_run;
        move_y_z("-z",steps);
      }else if(motor_to_run=="x"){
        digitalWrite(X_DIR_PIN,LOW);
        motor_accl_decl(X_STEP_PIN, steps_to_run, pulsewidth, smoothing_delay, smoothing_steps/2,motor_delay_long);
        stringComplete=false;
        inputString="";
      }else if(motor_to_run=="-x"){
        digitalWrite(X_DIR_PIN,HIGH);
        motor_accl_decl(X_STEP_PIN, steps_to_run, pulsewidth, smoothing_delay, smoothing_steps/2,motor_delay_long);
        stringComplete=false;
        inputString="";
      }
     }
   }

 void move_y_z(String axis, int steps_to_run){
  if(axis=="y"){
    for(int s=0;s<steps_to_run;s++){
      y1->moveMotor(1);
      y2->moveMotor(1);
      if(s==steps_to_run-1){
        stringComplete = false;
        }
      }
    }else if(axis=="-y"){
    for(int s=0;s<steps_to_run;s++){
      y1->moveMotor(-1);
      y2->moveMotor(-1);
      if(s==steps_to_run-1){
        stringComplete = false;
        }
      }
    }else if(axis=="z"){
    for(int s=0;s<steps_to_run;s++){
      y1->moveMotor(1);
      y2->moveMotor(-1);
      if(s==steps_to_run-1){
        stringComplete = false;
        }
      }
    }else if(axis=="-z"){
    for(int s=0;s<steps_to_run;s++){
      y1->moveMotor(-1);
      y2->moveMotor(1);
      if(s==steps_to_run-1){
        stringComplete = false;
        }
      }
    }
 }

void motor_accl_decl(int step_pin, long steps, long pulseWidth, long smoothing_delay, long smoothing_steps, long motor_delay_long_){
    //for 50 steps i.e. 90 degree, first accelerate and then max speed of motor till next 100 steps and 
    //final 50 steps i.e. after 270degree rotation decelerate.

    const float radian=0.0174532; //1 degree in radian
    const long min_steps_for_smooth = 10; //i.e. if steps are less than min_steps, there is no use for smoothing.

    float accl_angle=0 * radian; //0degree starting point for pulse for accl;
    float decl_angle=90 * radian;
    
    float steps_for_accl=smoothing_steps;
    float steps_for_decl= steps-smoothing_steps;
    
    float accl_angle_perstep= 90 * radian / steps_for_accl;
    float decl_angle_perstep= 90 * radian / steps_for_decl;

    long current_step, final_delay, smoothing_delay_new;
    for(current_step=0;current_step<steps; current_step++){
        smoothing_delay_new=0;
        if(steps > min_steps_for_smooth){
            if(current_step < steps_for_accl || current_step > steps_for_decl){
                if(current_step < steps_for_accl){
                    smoothing_delay_new= long(smoothing_delay - (sin(accl_angle)*smoothing_delay));
                    accl_angle = accl_angle + accl_angle_perstep;
                  }else{
                    smoothing_delay_new = long(smoothing_delay- (sin(decl_angle)*smoothing_delay));
                    decl_angle = decl_angle + decl_angle_perstep;
                    }
                  final_delay= motor_delay + smoothing_delay_new;
              }else{
                    final_delay = motor_delay;  //if steps in between smoothing area, delay is motor_delay
                  }
            
          }else{
              final_delay = motor_delay_long_;   //for steps less than min_steps_for_smoothing, delay is large delay
            }
          // Pulse
          rotate_motor( step_pin, pulseWidth, final_delay );
      }
 }

void rotate_motor (int pin, long pulseWidth, long pulse_delay ){
    digitalWrite(pin,HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(pin,LOW);
    delayMicroseconds(pulse_delay);
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


