#include <Arduino.h>
#include <M5Stack.h>
#include "utility/MPU9250.h"

MPU9250 IMU; // new a MPU9250 object

// The screen is 320 x 240 pixels
void free_state();
void busy_state();

// Global variables
int state = 0;
/**
States:
0->Idle
1->Free
2->Busy
3->Pomodoro
**/

// the setup routine runs once when M5Stack starts up
void setup(){



  // Initialize the M5Stack object
  M5.begin();
  Wire.begin();
  IMU.initMPU9250(); // this line must be after Wire.begin()
  IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias);

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
  //   M5.Lcd.setCursor(0, 0);
  // M5.Lcd.print("Y-acceleration: "); M5.Lcd.print(IMU.ay);

  // M5.Lcd.println(" mg ");
}


// State machine transitions
if (state != 1 && IMU.ay > 0.20) {
  state = 1;
}
else if (state != 2 && IMU.ay < -0.20) {
  state = 2;
}


switch (state) {
  case 0:
    //It just keep the last state
    break;

  case 1:
    free_state();
    break;
  case 2:
    busy_state();
    break;
  default:
    M5.Lcd.clear();
    break;
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
  M5.Lcd.setCursor(100,90);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.print("RED");
}
