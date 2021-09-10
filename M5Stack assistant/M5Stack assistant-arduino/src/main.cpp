/**

M5 Stack OFFICE TOOL
* Free-BUSY Indicator
* Lunch Indicator
* Pomodoro timer

Made by Julián Caro Linares
jcarolinares@gmail.com
@jcarolinares

CC-BY-SA

Acknowledgments and Attributions
The LED bar control is based on the code done by G4lileo on the project
ESP32-WiFi-Hash-Monster: https://github.com/G4lile0/ESP32-WiFi-Hash-Monster

**/

#include <Arduino.h>
#include <M5Stack.h>
#include "utility/MPU9250.h"
#include <Ticker.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// Web server running on port 80
WebServer server(80);


#ifdef ARDUINO_M5STACK_FIRE
  #include <FastLED.h>
  #define M5STACK_FIRE_NEO_NUM_LEDS 10
  #define M5STACK_FIRE_NEO_DATA_PIN 15
  // Define the array of leds
  CRGB leds[M5STACK_FIRE_NEO_NUM_LEDS];
#endif

// The screen is 320 x 240 pixels
#define LCD_MAX_X 320
#define LCD_MAX_Y 240
#define IFTTT_URL "WEBHOOK URL"
#define WIFI_SSID "YOUR SSID"
#define WIFI_PASSWORD "YOUR PASSWORD"

MPU9250 IMU; // new a MPU9250 object

Ticker tickerSetHigh;
Ticker tickerSetLow;

void free_state();
void busy_state();
void lunch_state();
void pomodoro_state();
void callback_pomodoro();
int create_calendar_event();
void getState();
void create_json(char *tag, float value, char *unit);
void handle_post_state();



// Global variables
int acc_th = 0.4;
unsigned int bright = 100;  // default
unsigned int bright_leds = 5;  // default
unsigned int led_status = 0;
unsigned int ledPacketCounter = 0;
unsigned long pomodoro_time = 0;
int pomodoro_minutes = 20;

// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[250];

int state = 0;
int stored_state = 0; // Used for getting the state data after a REST request
bool rest_state = false;
/**
States:
0->Idle
1->Free
2->Busy
3->Lunch
4->Pomodoro
5->Create calendar event
11-> Rest idle
**/


// the setup routine runs once when M5Stack starts up
void setup(){

  #ifdef ARDUINO_M5STACK_FIRE
    // load these before M5.begin() so they can eventually be turned off
    FastLED.addLeds<WS2812B, M5STACK_FIRE_NEO_DATA_PIN, GRB>(leds, M5STACK_FIRE_NEO_NUM_LEDS);
    FastLED.clear();
    FastLED.show();
  #endif

  // Initialize the M5Stack object
  M5.begin();
  Wire.begin();
  IMU.initMPU9250(); // this line must be after Wire.begin()
  // IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias); // Use it only if necessary on a flat surface

  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();

  // Set up and turn off the speaker output to avoid most of the anoying sounds
  pinMode(25, OUTPUT);
  M5.Speaker.mute();

  M5.Lcd.setTextSize(10);

  //Wifi connection
  Serial.begin(115200);
  delay(10);
  Serial.print("Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
  }

  if(WiFi.status()== WL_CONNECTED){
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Webserver for rest requests
    server.on("/state", getState);
    server.on("/set_state", HTTP_POST, handle_post_state);
    // start server
    server.begin();
    Serial.println("Server ON");

  }
  else{
    Serial.println("Not connected- Working without WIFI");
  }
}


void loop() {
M5.update();
server.handleClient();

// IMU reading
if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
{
  IMU.readAccelData(IMU.accelCount);
  IMU.getAres(); // get accelerometer scales saved to "aRes"
  IMU.ay = (float)IMU.accelCount[1] * IMU.aRes; // - accelBias[1];
}


if (rest_state == false){

// State machine transitions
if (state != 1 && state!= 3 && state!= 4  && IMU.ay > acc_th) {
  state = 1;
  free_state();
}
else if (state != 2 && state!= 3 && state!= 4 && IMU.ay < -acc_th) {
  state = 2;
  busy_state();
}
else if(M5.BtnA.wasPressed()){
  if(state!= 3){
    state = 3;
    lunch_state();
  }
  else
  {
    stored_state = state;
    state =0;
    Serial.println("unlock");
  }
}
else if(M5.BtnB.wasPressed()){
  if(state!= 4){
    state = 4;
    pomodoro_state();
  }
  else
  {
    stored_state = state;
    state =0;
  }
}
else if(M5.BtnC.wasPressed()){
  if(state!= 5){
    state = 5;
    create_calendar_event();
  }
  else
  {
    stored_state = state;
    state =0;
  }
}

}

else {  // State machine transitions for API rest control
  if (state == 1 ) {
    stored_state = state;
    state = 11;
    free_state();
  }
  else if (state == 2) {
    stored_state = state;
    state = 11;
    busy_state();
  }
  else if(state == 3){
    stored_state = state;
    state = 11;
    lunch_state();
  }
  else if(state == 4){
    stored_state = state;
    state = 11;  
    pomodoro_state();
  }
}

}



void free_state(){
  M5.Lcd.clear();
  M5.Lcd.setRotation(1);
  M5.Lcd.setCursor(75,90);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.print("FREE");

  #ifdef ARDUINO_M5STACK_FIRE
    for (int pixelNumber = 0; pixelNumber < 10; pixelNumber++){
      leds[pixelNumber].setRGB( 0, bright_leds, 0);
    }
    FastLED.show();
  #endif
}

void busy_state(){
  M5.Lcd.clear();
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(75,90);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.print("BUSY");

  #ifdef ARDUINO_M5STACK_FIRE
    for (int pixelNumber = 0; pixelNumber < 10; pixelNumber++){
      leds[pixelNumber].setRGB( bright_leds, 0, 0);
    }
    FastLED.show();
  #endif
}

void lunch_state(){
  M5.Lcd.clear();
  M5.Lcd.setRotation(1);
  M5.Lcd.setCursor(60,90);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.print("LUNCH");

  #ifdef ARDUINO_M5STACK_FIRE
    for (int pixelNumber = 0; pixelNumber < 10; pixelNumber++){
      leds[pixelNumber].setRGB( bright_leds, bright_leds, 0);
    }
    FastLED.show();
  #endif

  Serial.println("lock"); //Serial command to PC external scripts -> \r\n
}

void pomodoro_state(){
  delay(250); //Small delay to avoid bouncing double presses in button B
  int time_fraction = (pomodoro_minutes *60000)/LCD_MAX_Y;
  pomodoro_time = millis();
  M5.Lcd.clear();
  // M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(RED);

  #ifdef ARDUINO_M5STACK_FIRE
    for (int pixelNumber = 0; pixelNumber < 10; pixelNumber++){
      leds[pixelNumber].setRGB( bright_leds, 0, 0);
    }
    FastLED.show();
  #endif

  int i = 0;
  while(i<LCD_MAX_Y){
    if(millis()>=pomodoro_time+time_fraction){
      M5.Lcd.drawFastHLine(0,i, LCD_MAX_X, TFT_BLACK);
      i++;
      pomodoro_time = millis();
    }
    else if(M5.BtnA.wasPressed() || M5.BtnB.read() || M5.BtnC.wasPressed()){
      break;
    }
    M5.update();
  }
  M5.Lcd.clear();
  // M5.Lcd.setRotation(1);
  M5.Lcd.setCursor(60,90);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.print("BREAK");

  #ifdef ARDUINO_M5STACK_FIRE
    for (int pixelNumber = 0; pixelNumber < 10; pixelNumber++){
      leds[pixelNumber].setRGB(0, bright_leds, 0);
    }
    FastLED.show();
  #endif
}

int create_calendar_event(){
  M5.Lcd.clear();
  M5.Lcd.setRotation(1);
  M5.Lcd.setCursor(15,90);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.print("BOOKING");

  #ifdef ARDUINO_M5STACK_FIRE
    for (int pixelNumber = 0; pixelNumber < 10; pixelNumber++){
      leds[pixelNumber].setRGB( bright_leds, bright_leds, 0);
    }
    FastLED.show();
  #endif

  if(WiFi.status()== WL_CONNECTED){   // Check WiFi connection status

     HTTPClient http;
     http.begin(IFTTT_URL); // Specify destination for HTTP request
     http.addHeader("Content-Type", "text/plain"); //Specify content-type header

     int httpResponseCode = http.POST("post");   // Send the actual POST request

     if(httpResponseCode>0){

      Serial.println(httpResponseCode);   // Print return code
      M5.Lcd.clear();
      M5.Lcd.setCursor(40,90);
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.print("BOOKED");
      delay(3000);
      return httpResponseCode;
     }else{

      Serial.print("Error on sending request: ");
      Serial.println(httpResponseCode);
      M5.Lcd.clear();
      M5.Lcd.setCursor(40,90);
      M5.Lcd.setTextColor(RED);
      M5.Lcd.print("ERROR");
      return httpResponseCode;

     }

     http.end();  //Free resources
  }
  else{
    Serial.println("Connection error");
    return -1;
  }
}

// The M5 receive a busy signal from another device and change to busy

void getState() {
  if (state == 0)
  {
    Serial.println("State");
    create_json("State", stored_state, "");
    server.send(200, "application/json", buffer);   
  }
  else
  {
    Serial.println("State");
    create_json("State", state, "");
    server.send(200, "application/json", buffer);     
  }
  
}

void create_json(char *tag, float value, char *unit) { 
  jsonDocument.clear(); 
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
  Serial.println("Buffer:");
  Serial.println(buffer);  
}

void handle_post_state() {
  if (server.hasArg("plain") == false) {
    //handle error here
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  state = jsonDocument["state"];
  rest_state = true;

  // Respond to the client
  server.send(200, "application/json", "{State changed TEST}");
  Serial.println(state);  
}

