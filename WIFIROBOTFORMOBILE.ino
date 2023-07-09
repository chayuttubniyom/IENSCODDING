#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

int IN_1A = D5;
int IN_2A = D6;
int IN_1B = D7;
int IN_2B = D8;

const int PWM_A = D9;
const int PWM_B = D10;

String command;
float SPEED = 225;

ESP8266WebServer server(80);

String hostname("WIFICAR01");
IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200); // เริ่มการสื่อสารผ่าน Serial
  Serial.println();
  Serial.println("WiFi Robot Remote Control Mode");
  Serial.println("--------------------------------------");

  // ตั้งค่าสถานะเริ่มต้นของขา GPIO
  

  pinMode(IN_1A, OUTPUT);
  pinMode(IN_2A, OUTPUT);
  pinMode(IN_1B, OUTPUT);
  pinMode(IN_2B, OUTPUT);

  digitalWrite(IN_1B, 1);
  digitalWrite(IN_2B, 1);
  digitalWrite(IN_1A, 1);
  digitalWrite(IN_2A, 1);

  String chip_id = String(ESP.getChipId(), HEX); // อ่านรหัสชิป
  int i = chip_id.length() - 4;
  chip_id = chip_id.substring(i);
  chip_id = "wificar-" + chip_id; // สร้างชื่อโฮสต์

  Serial.println();
  Serial.println("Hostname: " + hostname);

  // ตั้ง NodeMCU เป็นโหมด AP พร้อมกับ IP แบบ static
  WiFi.mode(WIFI_AP);

  WiFi.softAPConfig(local_IP, gateway, subnet); // กำหนดค่า IP
  WiFi.softAP(hostname.c_str()); // สร้างจุดเชื่อมต่อ WiFi

  IPAddress myIP = WiFi.softAPIP(); // อ่าน IP ที่ได้รับ
  Serial.println("");
  Serial.println("WiFi-AP-Mode");
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  delay(3000);
  server.on("/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot);
  server.begin();

  ArduinoOTA.begin();
}

void loop() {
    ArduinoOTA.handle();          // listen for update OTA request from clients
    server.handleClient();        // listen for HTTP requests from clients
    Serial.println(SPEED);
      command = server.arg("State");          // check HTPP request, if has arguments "State" then saved the value
      if (command == "F") Forward();          // check string then call a function or set a value
      else if (command == "B") Backward();
      else if (command == "R") TurnRight();
      else if (command == "L") TurnLeft();
      else if (command == "S") Stop();
      else if (command == "1") SPEED = 0;
      else if (command == "2") SPEED = 130;
      else if (command == "3") SPEED = 150;
      else if (command == "4") SPEED = 200;
      else if (command == "5") SPEED = 255;
      delay(10);
}

// function prototypes for HTTP handlers
void HTTP_handleRoot(void) {
  String html = R"html(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no"/>
    <style>
      body {
        background-color: #1111;
      }
      .button {
        position: relative;
        display: inline-block;
        border: 2px solid #000;
        margin: 1px 1px;
        background-color: #fff;
        width: 25vw;
        height: 40vh;
        cursor: pointer;
        transition: background-color .3s ease-in-out;
      }
      .button:active {
        background-color: #aaa;
      }
      .button2 {
        position: relative;
        display: inline-block;
        border: 2px solid #000;
        margin: 1px 1px;
        background-color: #fff;
        width: 15vw;
        height: 80vh;
        cursor: pointer;
        transition: background-color .3s ease-in-out;
      }
      .button2:active {
        background-color: #aaa;
      }
      .slider {
        position: absolute;
        left: 40%;
        top: 40%;
        transform: translate(-50%, -50%);
        width: 20vw;
      }
      .slidecontainer {
        width: 5%;
      }
      #container {
        display: flex;
        justify-content: space-between;
        align-items: center;
        height: 80vh;
      }
      #vertical {
        display: flex;
        flex-direction: column;
        justify-content: space-around;
        height: 100%;
      }
      #horizontal {
        display: flex;
        justify-content: space-around;
        align-items: center;
        height: 100%;
      }
      .slider-container {
        position: relative;
        width: 100%;
        display: flex;
        justify-content: center;
        align-items: center;
      }
      #sliderValue {
        position: absolute;
        top: -40px;
        color: #000000;
        font-size: 24px;
      }
    </style>
  </head>
  <body>
    <div id="container">
      <div id="vertical">
        <div class="button" id="up" ontouchstart="sendCommand('F')" ontouchend="sendCommand('S')"></div>
        <div class="button" id="down" ontouchstart="sendCommand('B')" ontouchend="sendCommand('S')"></div>
      </div>
      <div class="slidecontainer">
        <div class="slider-container">
          <input type="range" min="1" max="5" value="5" class="slider" id="myRange">
          <div id="sliderValue"></div>
        </div>
      </div>
      <div id="horizontal">
        <div class="button2" id="left" ontouchstart="sendCommand('L')" ontouchend="sendCommand('S')"></div>
        <div class="button2" id="right" ontouchstart="sendCommand('R')" ontouchend="sendCommand('S')"></div>
      </div>
    </div>
    <script>
      var slider = document.getElementById("myRange");
      var output = document.getElementById("sliderValue");
      output.innerHTML = getSliderPercent(slider.value) + "%";

      slider.oninput = function() {
        output.innerHTML = getSliderPercent(this.value) + "%";
        var command = getCommandFromSliderValue(this.value);
        sendCommand(command);
        sendSliderValue();
      }

      function percenttagevalue(sliderValue) {
        var percent = ((sliderValue - 1) / 4) * 100;  // คำนวณเปอร์เซ็นต์
        return percent.toString();
      }
      function getCommandFromSliderValue(sliderValue) {
        switch (sliderValue) {
          case "1":
            return "1";
          case "2":
            return "2";
          case "3":
            return "3";
          case "4":
            return "4";
          case "5":
            return "5";
          default: percenttagevalue();
            return "S";
        }
      }

      function getSliderPercent(sliderValue) {
        var percent = ((sliderValue - 1) / 4) * 100;  // คำนวณเปอร์เซ็นต์
        return percent.toFixed(0);
      }

      function sendCommand(command) {
        var sliderValue = slider.value;
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/?State=" + command + "&SliderValue=" + sliderValue, true);
        xhr.send();
      }

      function sendSliderValue() {
        var sliderValue = slider.value;
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/?SliderValue=" + sliderValue, true);
        xhr.send();
      }
    </script>
  </body>
</html>

)html";
  server.send(200, "text/html", html);
  if(server.hasArg("State")) {
     Serial.println(server.arg("State"));
  }
}


void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

// function to move forward
void Forward(){ 
  digitalWrite(IN_1B, 1);
  digitalWrite(IN_2B, 0);
  digitalWrite(IN_1A, 1);
  digitalWrite(IN_2A, 0);
  analogWrite(PWM_A, SPEED);
  analogWrite(PWM_B, SPEED);


}

// function to move backward
void Backward(){
   digitalWrite(IN_1B, 0);
  digitalWrite(IN_2B, 1);
  digitalWrite(IN_1A, 0);
  digitalWrite(IN_2A, 1);
  analogWrite(PWM_A, SPEED);
  analogWrite(PWM_B, SPEED);

}

// function to turn right
void TurnRight(){
  digitalWrite(IN_1B, 1);
  digitalWrite(IN_2B, 0);
  digitalWrite(IN_1A, 0);
  digitalWrite(IN_2A, 1);
  analogWrite(PWM_A, SPEED);
  analogWrite(PWM_B, SPEED);
}

// function to turn left
void TurnLeft(){
  digitalWrite(IN_1B, 0);
  digitalWrite(IN_2B, 1);
  digitalWrite(IN_1A, 1);
  digitalWrite(IN_2A, 0);
  analogWrite(PWM_A, SPEED);
  analogWrite(PWM_B, SPEED);
}

// function to move forward left

// function to stop motors
void Stop(){  
  digitalWrite(IN_1B, 1);
  digitalWrite(IN_2B, 1);
  digitalWrite(IN_1A, 1);
  digitalWrite(IN_2A, 1);
}

