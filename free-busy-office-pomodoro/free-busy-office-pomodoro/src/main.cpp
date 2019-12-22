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


MPU9250 IMU; // new a MPU9250 object

Ticker tickerSetHigh;
Ticker tickerSetLow;

void free_state();
void busy_state();
void lunch_state();
void pomodoro_state();
void callback_pomodoro();

// Global variables
int acc_th = 0.4;
unsigned int bright = 100;  // default
unsigned int bright_leds = 5;  // default
unsigned int led_status = 0;
unsigned int ledPacketCounter = 0;
unsigned long pomodoro_time = 0;
int pomodoro_minutes = 1;

int state = 0;
/**
States:
0->Idle
1->Free
2->Busy
3->Lunch
4->Pomodoro
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
}

// the loop routine runs over and over again forever
void loop() {

M5.update();

if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
{
  IMU.readAccelData(IMU.accelCount);
  IMU.getAres(); // get accelerometer scales saved to "aRes"
  IMU.ay = (float)IMU.accelCount[1] * IMU.aRes; // - accelBias[1];
  // M5.Lcd.print("Y-acceleration: "); M5.Lcd.print(1000 * IMU.ay);
  // M5.Lcd.clear();
  // M5.Lcd.setCursor(0, 0);
  // M5.Lcd.print("Y-acceleration: ");
  // M5.Lcd.print(IMU.ay);

  // M5.Lcd.println(" mg ");
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
