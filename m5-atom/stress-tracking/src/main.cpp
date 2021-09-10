/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with Atom-Lite/Matrix sample source code
*                          配套  Atom-Lite/Matrix 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/products
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/products
*
* describe：EEPROM Read & Write.
* date：2021/7/30
*******************************************************************************
  The values stored in the EEPROM will remain in the EEPROM even after the M5Atom is disconnected.
  When a new program is uploaded to the M5Atom, the values stored in the EEPROM can still be called or modified by the new program.
  储存于EEPROM的数值即使在断开 M5Atom电源后仍会保存在EEPROM中
  当新程序上传到 M5Atom后，储存于EEPROM中的数值仍然可以被新的程序调用或者修改
*/

#include <Arduino.h>
#include <M5Atom.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define EEPROM_SIZE 32 // ESP32 int

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

// Wifi credentials
const char* ssid = "";
const char* password = "";
const char* IFTTT_URL = "https://maker.ifttt.com/trigger/m5atom-stress/with/key/qVxfRktiuEOdpCEbjUiNS";
bool ifttt_flag = false;


int counter_x = -1; // -1 To avoid the firs led to be on before the first press
int counter_y = 0;
int total_counter = 0;

int index_color = 0;
CRGB colors[] = { 0xFF0000, 0x0000FF, 0xFFFF00, 0x00FF00 }; // GRB NOT RGB Green,Blue,Yellow,Red




int ifttt_post(){
  if(WiFi.status()== WL_CONNECTED){   // Check WiFi connection status

     HTTPClient http;
     http.begin(IFTTT_URL); // Specify destination for HTTP request
     
    //  http.addHeader("Content-Type", "text/plain"); //Specify content-type header
    //  int httpResponseCode = http.POST("post");   // Send the actual POST request

    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST("{\"value1\":"+String(total_counter)+"}");


     if(httpResponseCode>0){
      Serial.println(httpResponseCode);   // Print return code
      return httpResponseCode;
     }else{
      Serial.print("Error on sending request: ");
      Serial.println(httpResponseCode);
      return httpResponseCode;
     }

     // If you need an HTTP request with a content type: application/json, use the following:
     //http.addHeader("Content-Type", "application/json");
     //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

     http.end();  //Free resources
  }
  else{
    Serial.println("Connection error");
    return -1;
  }
}

void drawLedMatrix(){
  // Nested FOR for Matrix draw control
  // Serial.print("Counter X e Y\n");
  // Serial.println(counter_x);
  // Serial.println(counter_y);
  for (int j = 0; j <= counter_y; j++) { // Rows Y
    if (j < counter_y)
    {
      // Serial.println("First for");
      for (int i = 0; i <= 4; i++) { // Columns X
        M5.dis.drawpix(i, j, colors[index_color]);
        // Serial.println(i);
        delay(10); // Important to not miss some turning on of some LEDs
      }  
    }
    else{
      // Serial.println("Second for");
      for (int i = 0; i <= counter_x; i++) { // Columns X
        M5.dis.drawpix(i, j, colors[index_color]);
        // Serial.println(i);
        delay(10); // Important to not miss some turning on of some LEDs
      }
    }
    
  }
}

void drawLedReverseMatrix(){
  // Nested FOR for Matrix draw control
  // Serial.print("Counter X e Y\n");
  // Serial.println(counter_x);
  // Serial.println(counter_y);
  for (int j = 0; j <= counter_y ; j++) { // Rows Y
    if (j < counter_y)
    {
      // Serial.println("First for");
      for (int i = 0; i <= 4; i++) { // Columns X
        M5.dis.drawpix(i, 4-j, colors[index_color]);
        // Serial.println(i);
        delay(10); // Important to not miss some turning on of some LEDs
      }  
    }
    else{
      // Serial.println("Second for");
      for (int i = 0; i <= counter_x; i++) { // Columns X
        M5.dis.drawpix(i, 4-j, colors[index_color]);
        // Serial.println(i);
        delay(10); // Important to not miss some turning on of some LEDs
      }
    }
    
  }
}


void matrixCount(){

  // Increase of total counter
  if (total_counter<25){
    total_counter++;
  }

  // Colums and rows control
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

  // It draws the Led Matrix
  drawLedReverseMatrix();
}

void initial_count(){
  counter_y = (total_counter/5); // Where five is the number of LEDs per row
  counter_x = (total_counter%5) -1;
  Serial.print("Counter X e Y\n");
  Serial.println(counter_x);
  Serial.println(counter_y);
  drawLedReverseMatrix();
}

void eepromToZero(){
    // Write last value to eeprom
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.write(0, 0);
    EEPROM.commit();
    EEPROM.end();
}

void setup() {

  // eepromToZero(); // Call it only to reset the memory count, comment after it

  M5.begin(true, true, true); //Init Atom -> bool SerialEnable, bool I2CEnable, bool DisplayEnable
  delay(10);
  setBuff(0x00, 0x00, 0x00); //RGB in hex
  // M5.dis.setBrightness(127); //no reaction
  M5.dis.displaybuff(DisBuff);

  // EEPROM reading
  EEPROM.begin(EEPROM_SIZE);
  total_counter = EEPROM.read(0);
  EEPROM.end();

  index_color = 3; // Red LED
  initial_count();

  Serial.print("Total counter from EEPROM: ");
  Serial.println(total_counter);

  //Wifi connection
  Serial.begin(115200);
  delay(10);
  Serial.print("Connecting");
  WiFi.begin(ssid, password);
  int wifi_timeout=0;
  while (WiFi.status() != WL_CONNECTED && wifi_timeout<150) {
      delay(100);
      Serial.print(".");
      wifi_timeout++;
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

  // EEPROM example
  // if (!EEPROM.begin(SIZE)){  //Request storage of SIZE size(success return 1).  申请SIZE大小的存储(成功返回1)
  //   Serial.println("\nFailed to initialise EEPROM!"); //串口输出格式化字符串.  Serial output format string
  //   delay(1000000);
  // }
  // Serial.println("\nRead data from Flash. Values are:");
  // for (int i = 0; i < SIZE; i++){
  //   Serial.printf("%d ",EEPROM.read(i)); //Reads data from 0 to SIZE in EEPROM.  读取EEPROM中从0到SIZE中的数据
  // }
}

void loop() {

  // Serial.println(counter_x);
  if (M5.Btn.wasReleasefor(2000)) // Reset count
  {
    Serial.println("Total count reset");
    counter_x = -1;
    counter_y = 0;
    total_counter = 0;
    eepromToZero();
    // drawLedMatrix();
    M5.dis.displaybuff(DisBuff);
  }
  else if (M5.Btn.wasPressed()) // Anxiety point -> Red LED
  {
    index_color = 3; // Red LED
    matrixCount();
    
    // Write last value to eeprom
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.write(0, total_counter);
    EEPROM.commit();
    EEPROM.end();

    // IFTTT alert
    if (total_counter>=15)
    {
      ifttt_post(); // Alert to perform an action thought IFTTT
      Serial.println("IFTTT Alert");
      ifttt_flag = true;
    }
    else
    {
      ifttt_flag = false;
    }
  }

  delay(50);  
  M5.update();  //Check button down state
}