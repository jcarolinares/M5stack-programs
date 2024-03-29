#include <Arduino.h>
#include <M5Stack.h>
#include "DHT12.h"
#include <Wire.h> //The DHT12 uses I2C comunication.
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <HTTPClient.h>

// IR libraries
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Daikin.h>

// Led M5stack fire library
#ifdef ARDUINO_M5STACK_FIRE
  #include <FastLED.h>
  #define M5STACK_FIRE_NEO_NUM_LEDS 10
  #define M5STACK_FIRE_NEO_DATA_PIN 15
  // Define the array of leds
  CRGB leds[M5STACK_FIRE_NEO_NUM_LEDS];
#endif

// Functions
void main_display();
void temp_display();
void ac_setup();

long time_abs=0;
long time_offset=3630000 *2;
float tmp = 0;
bool temp_flag= false;
float hum = 0;
float pressure = 0;
float hot_threshold = 28;

// AC variables
String ac_power = "on";
int ac_temp = 25;
bool screen_on = true;

// IR setup
const uint16_t kIrLed = 26; // M5 Stack bottom port B // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRDaikinESP ac(kIrLed);  // Set the GPIO to be used to sending the message

// Env setup
DHT12 dht12;
Adafruit_BMP280 bme;

#ifdef ARDUINO_M5STACK_FIRE
  int led_bright = 5;
  bool leds_on = true;
#endif
void setup() {

  #ifdef ARDUINO_M5STACK_FIRE
    // load these before M5.begin() so they can eventually be turned off
    FastLED.addLeds<WS2812B, M5STACK_FIRE_NEO_DATA_PIN, GRB>(leds, M5STACK_FIRE_NEO_NUM_LEDS);
    FastLED.clear();
    FastLED.show();
  #endif

  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */


  time_abs=millis();
  M5.begin();
  Wire.begin();
  M5.Power.begin();
  
  // Set up and turn off the speaker output to avoid most of the anoying sounds
  pinMode(25, OUTPUT);
  M5.Speaker.mute();

  while (!bme.begin(0x76)){
    Serial.println("Could not find a valid BMP280 sensor on port A");
    M5.Lcd.println("Could not find a valid BMP280 sensor on port A");
  }
  Serial.begin(115200);

  //IR setup
  ac.begin();
  main_display();
  delay(3000);
  ac_setup();
}

void loop() {
  M5.update();

  //Env readings
  tmp = dht12.readTemperature();
  temp_display();
  // hum = dht12.readHumidity();
  // pressure = bme.readPressure();
  
  // Buttons behaviour
  if (M5.BtnC.pressedFor(1500)) {
    if (ac_power == "on"){
      ac_power = "off";

      main_display();
    }
    else{
      ac_power = "on";
    }
  delay(500);
  }
  else if (M5.BtnA.pressedFor(1500)) {
    if (screen_on == true){
      M5.Lcd.sleep();
      M5.Lcd.setBrightness(0);
      screen_on = false;
      #ifdef ARDUINO_M5STACK_FIRE
        leds_on = false;
        FastLED.clear();
        FastLED.show();
      #endif
    }
    else{
      M5.Lcd.wakeup();
      M5.Lcd.setBrightness(127);
      screen_on = true;
      #ifdef ARDUINO_M5STACK_FIRE
        leds_on = true;
        main_display();
      #endif
    }
    delay(500);

  }
  else if (M5.BtnB.wasPressed()) {
    ac_setup();
  }
  else if(M5.BtnA.wasPressed()){
    if (ac_temp >10){
      ac_temp --;
    }
    ac.setTemp(ac_temp);
    main_display();
  }
  else if(M5.BtnC.wasPressed()){
    if (ac_temp <32){
      ac_temp ++;
    }
    ac.setTemp(ac_temp);
    main_display();
  }

  // Timer behaviour
  if (millis()>=(time_abs+time_offset)){
    time_abs=millis();
    ac.setCurrentTime(22 * 60 + 0);
    ac.enableOffTimer(22 * 60 + 30);
    ac.enableOnTimer(23 * 60 + 30);

    // Now send the IR signal.
    #if SEND_DAIKIN
      ac.send();
    #endif  // SEND_DAIKIN
  }

  // Too hot behaviour
  if (tmp>=hot_threshold && temp_flag == false){
    delay(5000);
    temp_flag = true;
    // ac_power = "on"; // Commented this, it can result in not desiderable beaviours as AC on with the windows open
    time_abs=millis();
    ac_setup();
    // Now send the IR signal.
    #if SEND_DAIKIN
      ac.send();
    #endif  // SEND_DAIKIN
    Serial.println("DONE");
  }

  else if (tmp<=hot_threshold -2 && temp_flag == true){
    temp_flag = false;
  }

  delay(100);
}


void ac_setup(){
  M5.Lcd.setCursor(45, 150);
  M5.Lcd.println("\nSending");
  // Set up what we want to send. See ir_Daikin.cpp for all the options.
  if (ac_power == "on"){
    ac.on();
  }
  else{
    ac.off();
  }
  ac.setFan(kDaikinFanQuiet);
  ac.setMode(kDaikinCool);
  ac.setTemp(ac_temp);
  ac.setSwingVertical(false);
  ac.setSwingHorizontal(false);

  // half hour working
  ac.setCurrentTime(22 * 60 + 0);
  ac.enableOffTimer(22 * 60 + 30);
  ac.enableOnTimer(23 * 60 + 30);

  // Now send the IR signal.
  #if SEND_DAIKIN
    ac.send();
  #endif  // SEND_DAIKIN

  delay(1000);
  M5.Lcd.clearDisplay(ILI9341_BLACK);
  main_display();
}

void main_display(){
  M5.Lcd.clearDisplay(ILI9341_BLACK);
  M5.Lcd.setTextSize(4);
  M5.Lcd.setCursor(50, 50);
  M5.Lcd.println("IR DAIKIN");
  M5.Lcd.setCursor(45, 100);
  M5.Lcd.println("CONTROLLER ");
  M5.Lcd.setCursor(135, 150);
  temp_display(); //Temp display
  M5.Lcd.setCursor(240, 200);
  M5.Lcd.println(ac_power);
}

void temp_display()
{
  M5.Lcd.setCursor(55, 150);
  M5.Lcd.print(tmp);
  M5.Lcd.print("->");
  M5.Lcd.println(ac.getTemp());
  // M5.Lcd.printf("Temp: %2.1f", tmp);
  
  #ifdef ARDUINO_M5STACK_FIRE
    if (leds_on == true)
    {
      if (tmp< hot_threshold - 4)
      {
        for (int pixelNumber = 0; pixelNumber < 5; pixelNumber++){ // Right Led bar
          leds[pixelNumber].setRGB( 0, 0, led_bright);
        }
      }
      else if (tmp < hot_threshold -2)
      {
        for (int pixelNumber = 0; pixelNumber < 10; pixelNumber++){ // Right Led bar
          leds[pixelNumber].setRGB( 0, led_bright, 0);
        }
      }
      else if (tmp < hot_threshold)
      {
        for (int pixelNumber = 0; pixelNumber < 10; pixelNumber++){ // Right Led bar
          leds[pixelNumber].setRGB( led_bright, led_bright, 0);
        }
      }
      else{
        for (int pixelNumber = 0; pixelNumber < 10; pixelNumber++){ // Right Led bar
          leds[pixelNumber].setRGB( led_bright, 0, 0);
        }   
      }
      FastLED.show();
    }
  #endif
}
