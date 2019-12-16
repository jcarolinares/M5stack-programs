#include <Arduino.h>
#include <M5Stack.h>


// The screen is 320 x 240 pixels

void free_state();
void busy_state();

// the setup routine runs once when M5Stack starts up
void setup(){

  // Initialize the M5Stack object
  M5.begin();

  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();
  M5.Lcd.setTextSize(10);
  free_state();
  // busy_state();
}

// the loop routine runs over and over again forever
void loop() {
// M5.Lcd.fillScreen(RED);
}

void free_state(){
  M5.Lcd.setRotation(1);
  M5.Lcd.setCursor(75,90);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.print("FREE");
}

void busy_state(){
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(100,90);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.print("RED");
}
