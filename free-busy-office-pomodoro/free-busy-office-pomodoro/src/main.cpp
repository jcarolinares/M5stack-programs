#include <Arduino.h>
#include <M5Stack.h>
#include "utility/MPU9250.h"

MPU9250 IMU; // new a MPU9250 object

// The screen is 320 x 240 pixels
void free_state();
void busy_state();
void lunch_state();
void pomodoro_state();

// Global variables
int state = 0;
int acc_th = 0.4;
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
// M5.Lcd.fillScreen(RED);
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
  }
  else
  {
    state =0;
  }
  lunch_state();
}
else if(M5.BtnB.wasPressed()){
  if(state!= 4){
    state = 4;
  }
  else
  {
    state =0;
  }
  pomodoro_state();
}

}

void free_state(){
  M5.Lcd.clear();
  M5.Lcd.setRotation(1);
  M5.Lcd.setCursor(75,90);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.print("FREE");
}

void busy_state(){
  M5.Lcd.clear();
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(75,90);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.print("BUSY");
}

void lunch_state(){
  M5.Lcd.clear();
  M5.Lcd.setRotation(1);
  M5.Lcd.setCursor(60,90);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.print("LUNCH");
}

void pomodoro_state(){
  M5.Lcd.clear();
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(RED);
}
