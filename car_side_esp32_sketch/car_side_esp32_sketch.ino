

// Import required libraries

/*
 * pin connections. Both motor drivers are connected identically
 *        ESP32                  Both Motor Drivers
 *        -----                  ----------------
 *        12                     PWMA and PWMB  //all motors run at the same spped
 *        27                     AIN1
 *        26                     AIN2
 *        32                     BIN1
 *        33                     BIN2
 *        5V                     STDBY
 *        5V                     VCC
 *        
 *        ESP32                  Rotary Encoder
 *        -----                  ----------------
 *        23                     Interrupt output
 */

#include <esp_now.h>
#include <WiFi.h>

#include "html_stuff.h"
//#include "motor_drive_stuff.h"
#include "rotary_encoder_stuff.h"


float d1=0.0;
float d2=0.0;

const int output = 2; //can't remember what this is for
const float holes = 40.0;
const float circumference = 21.05; //cm. 
const float robot_width = 15;//cm
const float turning_circle = 48; //cm. Distance between wheels times pi. (pi*d).
const float pi = 3.14159265359;


float theta;
float delta_theta;
float w1, w2;

bool test1, test2;



void turn_robot(float thet){
  counter1 = 0;
  counter2 = 0;
  if (thet > 0) robot_right();
  if (thet < 0) robot_left();
  d2 = ((float) counter2)/((float)holes)*circumference;
  
  while (d2 < turning_circle*abs(thet)/(2.0*pi)){
    d2 = ((float) counter2 )/((float)holes)*circumference;
   //wait for robot to complete turn
   //blocking code here. Can deal with this with rtos.
    //probably don't need these two lines, but just give the loop something to do.
    if (thet > 0) robot_right();
    if (thet < 0) robot_left();
  } 
  counter1 = 0;
  counter2 = 0;
  robot_stop();
}

void move_robot_dist(float dist_to_move){
  counter1 = 0;
  counter2 = 0;
  if (dist_to_move > 0) robot_fwd();
  if (dist_to_move < 0) robot_back();
  d2 = 0.0;// ((float) counter2)/(holes)*circumference;
  
  while (d2 < abs(dist_to_move)){
    Serial.println(counter2);
    d2 = ((float) counter2 )/(holes)*circumference;
   //wait for robot to complete turn
   //blocking code here. Can deal with this with rtos.
    //probably don't need these two lines, but just give the loop something to do.
    if (dist_to_move > 0) robot_fwd();
    if (dist_to_move < 0) robot_back();
  } 
  counter1 = 0;
  counter2 = 0;
  robot_stop();
}


double constrainAngle(double x){
    x = fmod(x + 180,360);
    if (x < 0)
        x += 360;
    return x - 180;
}


void setup(){
  
  // Serial port for debugging purposes
  Serial.begin(115200);
  robot_setup();

  //configure the static IP
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  

  WiFi.mode(WIFI_AP_STA);
  
 

//  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  
  start_rotary_encoder(); //set up rotary encoder. attach interupts
  start_server();
//  start_sonar(); //sonar set up.
//  start_gyro();
  
}

void loop() {
  if(dist_set){
    move_robot_dist(param_dist);
    dist_set = false;
  }
  delay(50);
  if(theta_set){
    turn_robot(param_theta*2.0*pi/360.0);
    theta_set = false;
  }
  delay(20);
}
