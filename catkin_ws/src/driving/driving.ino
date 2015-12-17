#include <ros.h>
#include <std_msgs/String.h>

class NewHardware : public ArduinoHardware {
  public: NewHardware():ArduinoHardware(&Serial1 , 57600) {};
}; ros::NodeHandle_<NewHardware> nh;

//pin layout robot

//H-bridge
int rev1 = 7; //left motor
int en1 = 24; //encoder pin
int fwd1 = 6;
int rev2 = 3; //right motor
int en2 = 25;
int fwd2 = 2;

//bluetooth
int BT_TX = 18;
int BT_RX = 19;
int pin_code = 1234; //or 0000

//sonar
int sonar_trig = 23;
int sonar_echo = 22;

//led
int led = 13;


int blockade = 0;
int dire = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(rev1, OUTPUT);
  pinMode(rev2, OUTPUT);

  pinMode(fwd1, OUTPUT);
  pinMode(fwd2, OUTPUT);

  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);

  pinMode(sonar_trig, OUTPUT);
  pinMode(sonar_echo, INPUT);

  digitalWrite(en1, HIGH);
  digitalWrite(en2, HIGH);

 
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    dire = Serial.read();

    Serial.print("I received: ");
    Serial.println(dire, DEC);

  }
  else {
    halt(1000);
  }


  if (dire == 49 || dire == 119) { //vooruit 119
    blockade = sonar();
    if (blockade != 1) {
      driving(255, 255, 100);
      dire = 0;
    }
  }

  else if (dire == 50 || dire == 115  ) { //achteruit 115
    //blockade = sonar();
    //if(blockade !=1){
    driving(-255, -255, 100);
    dire = 0;
    //}
  }

  else if (dire == 51 || dire == 97 ) { //links 97
    blockade = sonar();
    if (blockade != 1) {
      driving(-255, 255, 100);
      dire = 0;
    }
  }

  else if (dire == 52 || dire == 100 ) { //rechts 100
    blockade = sonar();
    if (blockade != 1) {
      driving(255, -255, 100);
      dire = 0;
    }
  }

  else { //stop
    halt(1);
  }

  //forward
  //int blockade = sonar();
  // if(blockade !=1){
  //   driving(255,255,100);
  // }

  // sonar();
  //delay(10);

  /*
  //back
  driving(-255,-255,1000);
  delay(10);

  //turn right
  driving(255,-255,1000);
  delay(10);

  //stop
   halt(1000);

  //slow left
  driving(100, 255, 2000);
  delay(10);

  //stop
  halt(10000);
  */

}


//stops the robot if called
void halt(int duration) {
  driving(0, 0, duration);
}

//turns input into movement and will continue for set duration
void driving(int left, int right, int duration) {
  //code here
  int motor_l = 0;
  int motor_r = 0;

  if (left >= 0) {
    //
    motor_l = fwd1;
  }
  else {
    //
    motor_l = rev1;
    left = -left;
  }
  if (right >= 0) {
    //
    motor_r = fwd2;
  }
  else {
    //
    motor_r = rev2;
    right = -right;
  }

  analogWrite(motor_l, left);
  analogWrite(motor_r, right);
  delay(duration);
  analogWrite(motor_l, 0);
  analogWrite(motor_r, 0);


}

//returns 1 when object is closer than 10 cm and stops the robot
bool sonar() {
  long duration;
  long distance = 0;
  digitalWrite(sonar_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar_trig, LOW);
  duration = pulseIn(sonar_echo, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance < 10) {
    halt(1000);
    return 1;
  }

  return 0;
}




//OLD CODE, simple movement tests

void turn_right() {
  analogWrite(fwd1, 255);
  analogWrite(rev2, 255);
  delay(1000);
  analogWrite(fwd1, 0);
  analogWrite(rev2, 0);
  delay(1000);
}

void turn_left() {
  analogWrite(rev1, 255);
  analogWrite(fwd2, 255);
  delay(1000);
  analogWrite(rev1, 0);
  analogWrite(fwd2, 0);
  delay(1000);
}

void drive_forward() {
  analogWrite(fwd1, 255);
  analogWrite(fwd2, 255);
  delay(1000);
  analogWrite(fwd1, 0);
  analogWrite(fwd2, 0);
  delay(1000);
}

void drive_backward() {
  analogWrite(rev1, 255);
  analogWrite(rev2, 255);
  delay(1000);
  analogWrite(rev1, 0);
  analogWrite(rev2, 0);
  delay(1000);
}
