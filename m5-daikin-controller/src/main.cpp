#include <Arduino.h>
#include <M5Stack.h>

// IR libraries
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Daikin.h>

// Functions
void main_display();
void ac_setup();

long time_abs=0;
long time_offset=3630000;


// IR setup
const uint16_t kIrLed = 26; //4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRDaikinESP ac(kIrLed);  // Set the GPIO to be used to sending the message

void setup() {
  time_abs=millis();
  M5.begin();
  Serial.begin(115200);

  //IR setup
  ac.begin();
  main_display();
  delay(10000);
  ac_setup();
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    ac_setup();
  }

  if (millis()>=(time_abs+time_offset)){
    time_abs=millis();
    ac.setCurrentTime(22 * 60 + 0);
    ac.enableOffTimer(23 * 60 + 0);
    ac.enableOnTimer(24 * 60 + 0);

    // Now send the IR signal.
    #if SEND_DAIKIN
      ac.send();
    #endif  // SEND_DAIKIN
  }

  delay(100);
}


void ac_setup(){
  M5.Lcd.setCursor(45, 150);
  M5.Lcd.println("\nSending");
  // Set up what we want to send. See ir_Daikin.cpp for all the options.
  ac.on();
  ac.setFan(kDaikinFanQuiet);
  ac.setMode(kDaikinCool);
  ac.setTemp(25);
  ac.setSwingVertical(false);
  ac.setSwingHorizontal(false);

  // One hour working
  ac.setCurrentTime(22 * 60 + 0);
  ac.enableOffTimer(23 * 60 + 0);
  ac.enableOnTimer(24 * 60 + 0);

  // Now send the IR signal.
  #if SEND_DAIKIN
    ac.send();
  #endif  // SEND_DAIKIN

  delay(1000);
  M5.Lcd.clearDisplay(ILI9341_BLACK);
  main_display();
}

void main_display(){
  M5.Lcd.setTextSize(4);
  M5.Lcd.setCursor(50, 50);
  M5.Lcd.println("IR DAIKIN");
  M5.Lcd.setCursor(45, 100);
  M5.Lcd.println("CONTROLLER ");
  M5.Lcd.setCursor(135, 150);
  M5.Lcd.println(ac.getTemp());
  M5.Lcd.setCursor(135, 200);
  // M5.Lcd.println(ac.getOffTime());
}
