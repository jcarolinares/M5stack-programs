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
#define IFTTT_URL "YOUR IFTTT REQUEST URL"
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

MPU9250 IMU; // new a MPU9250 object

Ticker tickerSetHigh;
Ticker tickerSetLow;

void free_state();
void busy_state();
void lunch_state();
void pomodoro_state();
void callback_pomodoro();
int create_calendar_event();

// Global variables
int acc_th = 0.4;
unsigned int bright = 100;  // default
unsigned int bright_leds = 5;  // default
unsigned int led_status = 0;
unsigned int ledPacketCounter = 0;
unsigned long pomodoro_time = 0;
int pomodoro_minutes = 20;

int state = 0;
/**
States:
0->Idle
1->Free
2->Busy
3->Lunch
4->Pomodoro
5->Create calendar event
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
  }
  else{
    Serial.println("Not connected- Working without WIFI");
  }
}

// the loop routine runs over and over again forever
void loop() {

M5.update();

if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
{
  IMU.readAccelData(IMU.accelCount);
  IMU.getAres(); // get accelerometer scales saved to "aRes"
  IMU.ay = (float)IMU.accelCount[1] * IMU.aRes; // - accelBias[1];
}


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
    state =0;
  }
}
else if(M5.BtnB.wasPressed()){
  if(state!= 4){
    state = 4;
    pomodoro_state();
  }
  else
  {
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
    state =0;
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

  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

     HTTPClient http;
     http.begin(IFTTT_URL); //Specify destination for HTTP request
     http.addHeader("Content-Type", "text/plain"); //Specify content-type header

     int httpResponseCode = http.POST("post");   //Send the actual POST request

     if(httpResponseCode>0){

      Serial.println(httpResponseCode);   //Print return code
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
