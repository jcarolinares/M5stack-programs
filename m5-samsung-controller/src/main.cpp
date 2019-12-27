/*
Based on the codes of
https://arduinostuff.blogspot.com/2011/06/samsung-remote-ir-codes.html

*/

// IR DEFINES SAMSUNG TV 38 KHZ
#define S_PWR {4600, 4350, 700, 1550, 650, 1550, 650, 1600, 650, 450, 650, 450, 650, 450, 650, 450, 700, 400, 700, 1550, 650, 1550, 650, 1600, 650, 450, 650, 450, 650, 450, 700, 450, 650, 450, 650, 450, 650, 1550, 700, 450, 650, 450, 650, 450, 650, 450, 650, 450, 700, 400, 650, 1600, 650, 450, 650, 1550, 650, 1600, 650, 1550, 650, 1550, 700, 1550, 650, 1550, 650}

#define S_0 {4650,4300,700,1550,700,1500,700,1550,700,400,700,400,700,400,700,450,650,450,650,1550,700,1550,650,1550,700,400,700,400,700,400,700,450,700,400,700,1550,650,400,700,450,700,400,650,1550,700,400,700,450,700,400,700,400,700,1500,700,1550,700,1500,700,400,700,1550,650,1550,700,1500,700}
#define S_1 {4650,4300,700,1550,700,1550,650,1550,700,400,700,400,700,400,700,450,700,400,700,1500,700,1500,700,1550,700,450,650,400,700,450,650,450,700,400,700,400,700,450,650,1550,700,400,700,400,700,400,700,450,650,450,650,1550,700,1500,700,450,650,1550,700,1550,650,1550,700,1500,700,1550,650}
#define S_2 {4600,4350,650,1550,700,1500,700,1550,700,400,700,400,700,450,650,450,700,400,700,1500,700,1500,700,1550,700,400,700,450,650,450,700,400,700,400,700,1500,700,400,700,1550,700,400,700,400,700,450,650,450,700,400,700,400,700,1550,650,450,700,1500,700,1550,650,1550,700,1500,700,1550,650}
#define S_3 {4600,4350,700,1500,700,1550,650,1600,650,400,700,450,700,400,700,400,700,400,700,1550,650,1550,700,1500,700,400,700,450,700,400,700,400,700,400,700,400,700,1550,700,1500,700,450,650,450,700,400,700,400,700,400,700,1550,700,400,700,400,700,1550,650,1550,700,1500,700,1550,700,1500,700}
#define S_4 {4600,4350,650,1550,700,1500,700,1550,700,400,700,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,400,700,450,650,450,700,400,700,400,700,400,700,400,700,450,650,1550,700,400,700,400,700,450,700,400,700,1500,700,1550,650,1550,700,400,700,1550,650,1550,700,1500,700,1550,650}
#define S_5 {4650,4350,700,1500,700,1550,650,1550,700,400,700,450,700,400,700,400,700,400,700,1500,700,1550,700,1500,700,450,650,450,700,400,700,400,700,400,700,1550,700,400,700,400,650,1550,700,450,650,450,700,400,700,450,650,450,650,1550,650,1550,700,400,700,1550,700,1500,700,1500,700,1550,700}
#define S_6 {4600,4350,650,1550,700,1500,700,1550,700,400,700,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,400,700,450,700,400,700,400,700,400,700,400,700,1550,700,400,700,1500,700,450,650,450,700,400,700,400,700,1550,650,450,650,1550,700,400,700,1550,650,1550,700,1500,700,1550,650}
#define S_7 {4600,4350,700,1500,700,1550,650,1550,700,400,700,450,700,400,700,400,700,400,700,1550,650,1550,700,1500,700,400,700,450,700,400,700,400,700,400,700,450,650,450,650,1550,700,1500,700,450,700,400,700,400,700,450,650,1550,650,1550,700,450,650,400,700,1550,700,1500,700,1550,650,1550,700}
#define S_8 {4600,4350,650,1600,650,1500,700,1550,700,400,700,400,700,400,700,450,700,400,700,1500,700,1550,650,1550,700,400,700,450,650,450,700,400,700,400,700,1550,650,450,650,1550,700,1500,700,450,700,400,700,400,700,400,700,400,700,1550,700,400,700,450,650,1550,650,1550,700,1500,700,1550,650}
#define S_9 {4600,4350,700,1500,700,1550,650,1550,700,400,700,450,650,450,650,450,700,400,700,1500,700,1550,700,1550,650,400,700,450,700,400,700,400,700,400,700,450,650,1550,650,1600,650,1550,650,450,700,400,700,400,700,400,700,1550,700,400,700,400,700,400,700,1550,700,1500,700,1500,700,1550,700}

#define S_MUTE {4650,4350,650,1550,650,1550,700,1550,700,400,700,400,700,400,700,450,650,450,650,1550,700,1500,700,1550,700,400,700,450,650,400,700,450,700,400,700,1500,700,1550,650,1550,700,1500,700,450,700,400,700,400,700,400,700,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,1500,700}
#define S_SOURCE {4600,4350,700,1550,650,1550,700,1500,700,450,650,450,700,400,700,400,700,400,700,1550,700,1500,700,1550,700,400,700,400,700,400,700,400,700,400,700,1550,700,400,700,450,650,450,650,450,700,400,700,400,700,400,700,450,650,1550,700,1500,700,1550,650,1550,700,1500,700,1550,700,1500,700}
#define S_CH_UP {4600,4350,700,1500,700,1500,700,1550,700,450,650,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,450,650,450,700,400,700,400,700,400,700,400,700,1550,700,400,700,400,700,1550,650,450,700,400,700,400,700,1550,650,450,650,1600,650,1550,650,450,700,1500,700,1500,700,1550,650}
#define S_CH_DOWN {4650,4300,700,1550,700,1500,700,1550,700,400,700,400,700,400,700,450,650,450,650,1550,700,1500,700,1550,700,400,700,400,700,400,700,450,700,400,700,400,700,400,700,450,650,450,650,1550,700,400,700,450,650,400,700,1550,700,1500,700,1550,700,1500,700,400,700,1550,650,1550,700,1500,700}
#define S_VL_UP {4600,4350,650,1550,700,1500,700,1550,700,400,700,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,400,700,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,400,700,450,700,400,700,400,700,400,700,450,650,450,650,450,650,1550,700,1500,700,1550,700,1500,700,1550,650}
#define S_VL_DOWN {4600,4350,700,1550,650,1550,700,1500,700,450,650,450,700,400,700,400,700,400,700,1550,700,1500,700,1550,700,400,700,400,700,400,700,450,650,450,650,1550,700,1500,700,450,650,1550,700,400,700,400,700,450,700,400,700,400,700,400,700,1550,700,400,700,1500,700,1500,700,1550,700,1500,700}
#define S_EXIT {4650,4300,700,1550,650,1550,700,1550,700,400,700,400,700,450,650,450,650,450,650,1550,700,1500,700,1550,700,450,650,400,700,450,650,450,700,400,700,1500,700,400,700,1550,700,1500,700,400,700,1550,700,450,650,400,700,450,650,1550,700,400,700,400,700,1550,650,450,650,1550,700,1500,700}

#include <Arduino.h>
#include <M5Stack.h>

// IR libraries
#include <IRremoteESP8266.h>
#include <IRsend.h>

// IR setup
const uint16_t kIrLed = 26; // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.


// Power ON/OFF
uint16_t S_pwr[68]={0x4600,4350,700,1550,650,1550,650,1600,650,450,650,450,650,450,650,450,700,400,700,1550,650,1550,650,1600,650,450,650,450,650,450,700,450,650,450,650,450,650,1550,700,450,650,450,650,450,650,450,650,450,700,400,650,1600,650,450,650,1550,650,1600,650,1550,650,1550,700,1550,650,1550,650};
// 0b1010

// Functions declaration
void display_tv();

void setup() {
  irsend.begin();

  #if ESP8266
    Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  #else  // ESP8266
    Serial.begin(115200, SERIAL_8N1);
  #endif  // ESP8266
  M5.begin();

  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();
  // Set up and turn off the speaker output to avoid most of the anoying sounds
  // pinMode(25, OUTPUT);
  // M5.Speaker.mute();

  M5.Lcd.setTextSize(10);
  display_tv();

}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()){

    uint16_t ir_code[] = S_CH_UP;
    Serial.println("a rawData capture from IRrecvDumpV2");
    irsend.sendRaw(ir_code, 68, 38);  // Send a raw data capture at 38kHz.

    M5.Lcd.clear();
    M5.Lcd.setRotation(1);
    M5.Lcd.setCursor(65,90);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.print("1");
    delay(2000);
    display_tv();
  }

  else if (M5.BtnB.wasPressed()){

    uint16_t ir_code[] = S_PWR;
    Serial.println("a rawData capture from IRrecvDumpV2");
    irsend.sendRaw(ir_code, 68, 38);  // Send a raw data capture at 38kHz.

    M5.Lcd.clear();
    M5.Lcd.setRotation(1);
    M5.Lcd.setCursor(65,90);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.print("POWER");
    delay(2000);
    display_tv();
  }

  else if (M5.BtnC.wasPressed()){

    uint16_t ir_code[] = S_CH_DOWN;
    Serial.println("a rawData capture from IRrecvDumpV2");
    irsend.sendRaw(ir_code, 68, 38);  // Send a raw data capture at 38kHz.

    M5.Lcd.clear();
    M5.Lcd.setRotation(1);
    M5.Lcd.setCursor(75,90);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.print("MUTE");
    delay(2000);
    display_tv();
  }

}

void display_tv(){
  M5.Lcd.clear();
  M5.Lcd.setRotation(1);
  M5.Lcd.setCursor(120,90);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.print("TV");
}