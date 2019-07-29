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
long time_offset=3630000 *2;


// AC variables
String ac_power = "on";
int ac_temp = 25;

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
  delay(3000);
  ac_setup();
}

void loop() {
  M5.update();

  if (M5.BtnB.wasPressed()) {
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
  else if (M5.BtnC.pressedFor(2000)) {
    if (ac_power == "on"){
      ac_power = "off";
      main_display();
    }
    else{
      ac_power = "on";
      main_display();
    }
  delay(500);
  }

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
  M5.Lcd.println(ac.getTemp());
  M5.Lcd.setCursor(240, 200);
  M5.Lcd.println(ac_power);
}
