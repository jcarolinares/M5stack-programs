#include <Arduino.h>
#include "M5Atom.h"

extern const unsigned char AtomImageData[375 + 2];

uint8_t DisBuff[2 + 5 * 5 * 3]; //No idea

void setBuff(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata) //It seems a buffer for the RGB LED
{
    DisBuff[0] = 0x05;
    DisBuff[1] = 0x05;
    for (int i = 0; i < 25; i++)
    {
        DisBuff[2 + i * 3 + 0] = Rdata;
        DisBuff[2 + i * 3 + 1] = Gdata;
        DisBuff[2 + i * 3 + 2] = Bdata;
    }
}

void setup()
{
    M5.begin(true, false, true); //bool SerialEnable, bool I2CEnable, bool DisplayEnable
    delay(10);
    setBuff(0x00, 0x00, 0x00); //RGB in hex
    // M5.dis.setBrightness(127); //no reaction
    M5.dis.displaybuff(DisBuff);
}

int counter_x = 0;
int counter_y = 0;
int total_counter = 0;

int index_color = 0;
CRGB colors[] = { 0xFF0000, 0x0000FF, 0xFFFF00, 0x00FF00 };

void loop()
{
    if (M5.Btn.wasPressed())
    {

      // Total counter & Color control
      if (total_counter < 25) {
        index_color = 0;
        total_counter++;
      }
      else if (total_counter < 50) {
        index_color = 1;
        total_counter++;
      }
      else if (total_counter < 75) {
        index_color = 2;
        total_counter++;
      }
      else if (total_counter < 100) {
        index_color = 3;
        total_counter++;
      }
      else {
        index_color = 0;
        total_counter = 1;
      }

      // Nested for Matrix draw control
      for (int j = 0; j <= counter_y; j++) { // Rows Y
        for (int i = 0; i <= counter_x; i++) { // Columns X
          M5.dis.drawpix(i, j, colors[index_color]);
        }
      }

      if (counter_x < 4) {
        counter_x++;
      }
      else {
        counter_x = 0;
        if (counter_y < 4) {
           counter_y++;
        }
        else {
          counter_y = 0;
        }
      }

    }

    delay(50);
    M5.update();
}
