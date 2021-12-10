#include "Adafruit_VL53L0X.h"
#include<Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Servo servo;

// To connect to WiFi
const char* ssid = "ssid";                 
const char* password = "password";          

ESP8266WebServer server(80);//initialise at port no.80

int M3 = D6;    // Right wheel +ve
int M4 = D5;    // Right wheel -ve
int M2 = D0;    // Left wheel +ve
int M1 = D4;    // Left wheel -ve
float pos;

void setup() {
  delay(100);
  Serial.begin(9600);
  servo.attach(0);    // D3
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  if (!lox.begin())
  {
    Serial.println("Failed to boot VL53L0X");
    while(1);
  } 

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  server.on("/",handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println(WiFi.localIP());
}

void loop() {
  for(pos = 0; pos <= 90; pos+=1){
    servo.write(pos);
    server.handleClient();
    while(LIDAR_Dist() < 50){
      servo.write(90);
      server.handleClient();
      bot_move("LEFT");
      if(LIDAR_Dist() < 5){
        bot_move("BACKWARD");
      }
    }
    bot_move("FORWARD");
    delay(100);
  }
  for(pos = 90; pos <= 180; pos+=1){
    servo.write(pos);
    server.handleClient();
    while(LIDAR_Dist() < 50){
      servo.write(90);
      server.handleClient();
      bot_move("RIGHT");
      if(LIDAR_Dist() < 5){
        bot_move("BACKWARD");
      }
    }
    bot_move("FORWARD");
    delay(100);
  }
}  
void bot_move(String cmd){
  if(cmd == "FORWARD"){
    digitalWrite(M2, HIGH);
    digitalWrite(M1, LOW);
    digitalWrite(M3, HIGH);
    digitalWrite(M4, LOW);
  }
  if(cmd == "BACKWARD"){
    digitalWrite(M2, LOW);
    digitalWrite(M1, HIGH);
    digitalWrite(M3, LOW); 
    digitalWrite(M4, HIGH);
  }
  if(cmd == "LEFT"){
    digitalWrite(M2, LOW);
    digitalWrite(M1, HIGH);
    digitalWrite(M3, HIGH);
    digitalWrite(M4, LOW);
  }
  if(cmd == "RIGHT"){
    digitalWrite(M2, HIGH);
    digitalWrite(M1, LOW);
    digitalWrite(M3, LOW);
    digitalWrite(M4, HIGH);
  }
  if(cmd == "STOP"){
    digitalWrite(M2, LOW);
    digitalWrite(M1, LOW);
    digitalWrite(M3, LOW);
    digitalWrite(M4, LOW);
  }
}

int LIDAR_Dist(){
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  if(measure.RangeStatus != 4)
  {  
    return(measure.RangeMilliMeter);
  }
  else
  {
    return 10000;
  }
}

void handle_OnConnect(){
  String to_be_sent = String(pos)+" "+String(LIDAR_Dist());
  server.send(200,"text/plain",to_be_sent);
  Serial.println(to_be_sent);
}
void handle_NotFound(){
  server.send(404,"text/plain","Not found");
} 
