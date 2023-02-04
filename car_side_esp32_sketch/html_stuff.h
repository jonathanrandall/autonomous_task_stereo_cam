
/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-web-server-slider-pwm/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#ifndef HTML_STUFF_H_
#define HTML_STUFF_H_

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "motor_drive_stuff.h"

//use ipconfig to find details of your ip
// Set your Static IP address
IPAddress local_IP(192, 168, 1, 182);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

typedef struct test_struct  {
  String sliderValue;
  String variable;
  
} test_struct ;

test_struct myData;

float param_theta;
float param_dist;

bool dist_set = false;
bool theta_set = false;

const char* ssid = "WiFi-C5BFGF";
const char* password = "****";

String sliderValue = "000";
String variable;

const char* PARAM_INPUT_1 = "distance";
const char* PARAM_INPUT_2 = "angle";


// setting PWM properties
//const int freq = 5000;
//const int ledChannel = 0;
//const int resolution = 8;

const char* PARAM_INPUT = "value";
const char* PARAM_CMD = "go";

/*
 * from gpt 3
<form>
  <label for="text">Enter text:</label>
  <input type="text" id="textInput" name="text">
  <button type="button" onclick="readText()">Read Text</button>
</form>

<script>
function readText() {
  var text = document.getElementById("textInput").value;
  console.log(text);
  // Do something with the text here
}
</script>

and for mobile
<form>
  <label for="text">Enter text:</label>
  <input type="text" id="textInput" name="text">
  <button type="button" id="readTextBtn">Read Text</button>
</form>

<script>
document.getElementById("readTextBtn").addEventListener("click", readText);
document.getElementById("readTextBtn").addEventListener("touchstart", readText);

function readText() {
  var text = document.getElementById("textInput").value;
  console.log(text);
  // Do something with the text here
}
</script>
<form action="/get">
    angle: <input type="text" name="angle">
    <input type="submit" value="Submit">
 */

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
    <title>ESP32-CAM Robot</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
      table { margin-left: auto; margin-right: auto; }
      td { padding: 8 px; }
      .button {
        background-color: #2f4468;
        border: none;
        color: white;
        padding: 10px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 18px;
        margin: 6px 3px;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }
       .grid-container {
        display: grid;
        grid-template-columns: repeat(2, 1fr); /* 2 columns */
        grid-gap: 10px;
      }

      .grid-item {
        width: 100%%;
      }
    </style>
  </head>
  <body>
    <h1>ESP32 Robot</h1>
    <form>
    <label for="text">Distance:</label>
    <input type="text" id="distance" name="distin">
    <button type="button" onclick="readText('distance')" ontouchstart="readText('distance')">Read Text</button>
    </form>
  <form>
    <label for="text">Angle:</label>
    <input type="text" id="angle" name="angleing">
    <button type="button" onclick="readText('angle')" ontouchstart="readText('angle')">Read Text</button>
  </form><br>
    <div class="grid-container">
      <img src="http://192.168.1.121:81/stream" class="grid-item" id="rt_img">
      <img src="http://192.168.1.129:81/stream" class="grid-item" id = "lft_img">
    </div>
  <p><span id="textSliderValue">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="255" value="%SLIDERVALUE%" step="1" class="slider"></p>
    <table>
      <tr><td colspan="3" align="center"><button class="button" onmousedown="toggleCheckbox('forward');" ontouchstart="toggleCheckbox('forward');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Forward</button></td></tr>
      <tr><td align="center"><button class="button" onmousedown="toggleCheckbox('left');" ontouchstart="toggleCheckbox('left');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Left</button></td><td align="center"><button class="button" onmousedown="toggleCheckbox('stop');" ontouchstart="toggleCheckbox('stop');">Stop</button></td><td align="center"><button class="button" onmousedown="toggleCheckbox('right');" ontouchstart="toggleCheckbox('right');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Right</button></td></tr>
      <tr><td colspan="3" align="center"><button class="button" onmousedown="toggleCheckbox('backward');" ontouchstart="toggleCheckbox('backward');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Backward</button></td></tr>                   
    </table>
   <script>
   function readText(x) {
    var text = document.getElementById(x).value;
    var xhr = new XMLHttpRequest();
     xhr.open("GET", "/action?"+x+"=" + text, true);
     xhr.send();
  
    }
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/action?go=" + x, true);
     xhr.send();
   }
  function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}
  </script>
  </body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}

void start_server(){
  // Route for root / web page
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      myData.sliderValue = inputMessage;
      sliderValue =  myData.sliderValue;
      motor_speed = (uint8_t) sliderValue.toInt(); //ledcWrite(ledChannel, sliderValue.toInt());
      Serial.println(motor_speed);
//      Serial.println("request here3");
//      ledcWrite(motorPWMChannnelA, 100);
      update_speed();
//      Serial.println("request here4");
//      if(!robot_send()) Serial.println("something wrong with robot send speed");
    }
    else {
      inputMessage = "No message sent";
    }
//    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      param_dist = (request->getParam(PARAM_INPUT_1)->value()).toDouble();
      Serial.println(param_dist);
      dist_set = true;
//      inputParam = PARAM_INPUT_1;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      param_theta = (request->getParam(PARAM_INPUT_2)->value()).toDouble();
      theta_set = true;
//      inputParam = PARAM_INPUT_2;
    }
    else {
      Serial.println("something is not right here");
//      inputMessage = "No message sent";
//      inputParam = "none";
    }
    
//    request->send(200, "text/plain", "OK");
    request->send_P(200, "text/html", index_html, processor);
  
  });
  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/action", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_CMD)) {
      inputMessage = request->getParam(PARAM_CMD)->value();
      myData.variable = inputMessage.c_str();
      variable = myData.variable;
      if (variable== "stop"){
        robot_stop();
      }
      if (variable== "backward"){
        robot_back();
        robo = 1;
      }
      if (variable== "forward"){
        robot_fwd();
        robo = 1;
      }
      if (variable== "left"){
//        Serial.println("left");
        robot_left();
        robo = 1;
      }
      if (variable== "right"){
        robot_right();
        robo = 1;
      }
    }
    else if (request->hasParam(PARAM_INPUT_1)) {
      param_dist = (request->getParam(PARAM_INPUT_1)->value()).toDouble();
      Serial.println(param_dist);
      dist_set = true;
//      inputParam = PARAM_INPUT_1;
    }
    else if (request->hasParam(PARAM_INPUT_2)) {
      param_theta = (request->getParam(PARAM_INPUT_2)->value()).toDouble();
      theta_set = true;
//      inputParam = PARAM_INPUT_2;
    }
    else {
      inputMessage = "No message sent";
    }
//    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}

#endif
