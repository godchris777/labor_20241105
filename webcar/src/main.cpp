#include <Arduino.h>
#include <WiFi.h>

//系統套件
#include <iostream> 
#include <sstream> 

#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

//定義代碼較容易閱讀 - 前進、後退、左、右、停止
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define STOP 0

#define RIGHT_MOTOR 0 //右輪
#define LEFT_MOTOR 1 //左輪


#define FORWARD 1 //前進
#define BACKWARD -1 //後退

const int PWMFreq = 1000; //1KHz
const int PWMResolution = 8; //8位元解析度，代表最大值255
const int PWMSpeedChannel = 4; //設定4作為控制通道

const char *ssid = "ChrisCar"; //WiFi ssid名稱
const char *password = "12345678"; //WiFi密碼

AsyncWebServer server(80); //初始化80 port。
AsyncWebSocket wsCarInput("/CarInput"); //網頁網址後面加上CarInput

struct MOTOR_PINS{
  int pinEn;
  int pinIN1;
  int pinIN2;
};

std::vector<MOTOR_PINS> motorPins = {
  {22,16,17}, //右輪馬達(EnA, IN1, IN2)
  {23,18,19}, //左輪馬達(EnB, IN3, IN4)
};


void setUpPinModes(){
  ledcSetup(PWMSpeedChannel, PWMFreq,PWMResolution); //PWM設定
  
   //motorPins.size()會得到2，即2列
  for(int i=0; i<motorPins.size();i++){
    pinMode(motorPins[i].pinEn, OUTPUT);
    pinMode(motorPins[i].pinin1, OUTPUT);
    pinMode(motorPins[i].pinin2, OUTPUT);

    ledcAttachPin(motorPins[i].pinEn, PWMSpeedChannel); //可以當作PWM腳位，以了解速度  
  }


}


void handleNotFound(AsyncWebServerRequest *request){
  request->send(404, "text/plain", "File Not Found");
}


void handleRoot(AsyncWebServerRequest *request){
  request->send(200, "text/HTML", htmlHomePage);
}


void setup() {
  setUpPinModes();
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();  //獲取AP的IP
  Serial.print("AP IP address:");
  Serial.println(IP);
  server.on("/", HTTP_GET, handleRoot); //已連到根目錄後
  server.onNotFound(handleNotFound); //無連到或找不到任何網頁後
  wsCarInput.onEvent(onCarInputWebSocketEvent);
  server.addHandler(&wsCarInput);
  server.begin(); //啟用伺服器
  Serial.println("HTTP Server started");
  


}

void loop() {


}