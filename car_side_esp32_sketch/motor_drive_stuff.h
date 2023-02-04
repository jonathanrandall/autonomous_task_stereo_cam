#ifndef MOTOR_DRIVE_STUFF_H_
#define MOTOR_DRIVE_STUFF_H_

int gamma1 = 0;
// Motor right
 
int enA = 12; //pwm pin white
int lmI1 = 27; //yellow
int lmI2 = 26; //green
 
// Motor left
 
//uint8_t enB = 13; //pwm B pin only use one pwm pin
int rmI1 = 32;  //purple
int rmI2 = 33;  //blue

// Define Speed variables

int noStop = 0;

//Setting Motor PWM properties
long int freq = 2000;
const int motorPWMChannnelA = 7;
//const int motorPWMChannnelB = 2;
uint8_t lresolution = 8;
 
uint8_t  motor_speed   = 200;
volatile unsigned long previous_time = 0;
volatile unsigned long move_interval = 250;



void update_speed()
{ 
    ledcWrite(motorPWMChannnelA, motor_speed);

//    Serial.println("motor_speed");
    
//    ledcWrite(motorPWMChannnelB, motor_speed+gamma1);

//    Serial.println(motor_speed);
    
}


enum state {fwd, back, stp, right, left};
state actstate = stp;
uint8_t robo = 0;

void robot_stop()
{
  digitalWrite(lmI1,LOW);
  digitalWrite(lmI2,LOW);
  digitalWrite(rmI1,LOW);
  digitalWrite(rmI2,LOW);
  actstate = stp;
}

void robot_fwd()
{
  if(actstate != fwd){
    digitalWrite(lmI1,HIGH);
    digitalWrite(lmI2,LOW);
    digitalWrite(rmI1,HIGH);
    digitalWrite(rmI2,LOW);
  move_interval=250;
  actstate = fwd;
  }
  previous_time = millis();  
  
}
 
void robot_back()
{
  if(actstate != back){
    digitalWrite(lmI1,LOW);
    digitalWrite(lmI2,HIGH);
    digitalWrite(rmI1,LOW);
    digitalWrite(rmI2,HIGH);
  move_interval=250;
  actstate = back;
  }
   previous_time = millis();  
}
 
void robot_left()
{
  if(actstate != left){
    digitalWrite(lmI1,LOW);
    digitalWrite(lmI2,HIGH);
    digitalWrite(rmI1,HIGH);
    digitalWrite(rmI2,LOW);
  move_interval=100;
  actstate = left;
  }
   previous_time = millis();
}
 
void robot_right()
{
  if(actstate != right){
    digitalWrite(lmI1,HIGH);
    digitalWrite(lmI2,LOW);
    digitalWrite(rmI1,LOW);
    digitalWrite(rmI2,HIGH);
  move_interval=100;
  actstate = right;
  }
   previous_time = millis();
}

void robot_setup(){

  // Pins for Motor Controller
    pinMode(lmI1,OUTPUT);
    pinMode(lmI2,OUTPUT);
    pinMode(rmI1,OUTPUT);
    pinMode(rmI2,OUTPUT);
    
    // Make sure we are stopped
    robot_stop();
 
    // Motor uses PWM Channel 8
    
    delay(100);
    
    ledcSetup(motorPWMChannnelA, freq, lresolution);
//    ledcSetup(motorPWMChannnelB, freq, lresolution);
    
    
    
    ledcAttachPin(enA, motorPWMChannnelA);
//    ledcAttachPin(enB, motorPWMChannnelB);

    ledcWrite(motorPWMChannnelA, 130);
//    ledcWrite(motorPWMChannnelB, 130);
    
  
}



#endif
