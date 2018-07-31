#define led_pin 13
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(led_pin,OUTPUT);
}

void loop() {
if (Serial.available()){
    int incoming=Serial.read();
    if(incoming=='1'){
      digitalWrite(led_pin,1);
      }else if(incoming=='0'){
        digitalWrite(led_pin,0);
        }
}
}

