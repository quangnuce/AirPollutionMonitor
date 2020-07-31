#include "SdsDustSensor.h"
#include <SPI.h>
#include <Adafruit_ST7735.h>
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>



// MAN HINH TFT 1.44'
#if defined(ESP32)
#define TFT_CS         5
#define TFT_RST        22
#define TFT_DC         21
#elif defined(ESP8266)
#define TFT_CS         4
#define TFT_RST        16
#define TFT_DC         5
#else
// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_CS        10
#define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         8
#endif
Adafruit_ST7735 tft = Adafruit_ST7735 (TFT_CS, TFT_DC, TFT_RST);


// NOVA PM SENSOR
const int rxPin = 16;
const int txPin = 17;
//SoftwareSerial novaSerial(Serial2);
SdsDustSensor sds(Serial1);
//SdsDustSensor sds(Serial2);

// SIM 808
//#define PIN_TX   2
//#define PIN_RX   3
//SoftwareSerial mySerial(PIN_TX, PIN_RX);
DFRobot_SIM808 sim808(&Serial2);//Connect RX,TX,PWR,


//data:
float pm25;
float pm10;
float last_lat;
float last_lng;
int Data = 0;

 void initNova(){
    //novaSerial.begin(9600);
    sds.begin(9600);
    Serial.print("ok roi");
    Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
    Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
    Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended
  }


void initTFT() {

  // KHOI TAO MAN HINH TFT 1.44'
  //Serial.begin(9600);
  Serial.print(F("Hello MY BOSS !"));
  tft.initR(INITR_144GREENTAB);
  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;
  Serial.println(time, DEC);
  delay(500);
  tft.fillScreen(ST77XX_BLACK);
  //testdrawtext("KHOI TAO MAN HINH TFT THANH CONG \n THANK YOU ", ST77XX_WHITE);
  delay(2000);
}


void getPM() {
  
  PmResult pm = sds.readPm();
  if (pm.isOk()) {
    // tft print function!

   // Serial.print(pm.pm25);
    //Serial.print(pm.pm10);
    pm25 = pm.pm25;
    pm10 = pm.pm10;

  }
  else {
    // notice that loop delay is set to 0.5s and some reads are not available
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
  }
 
 
}
void printScreen() {

  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 40);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.println("CHI SO BUI TRONG KHONG KHI:");
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.print("PM2.5 = ");
  tft.println(pm25);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(1);
  tft.print("PM10 = ");
  tft.println(pm10);
  tft.invertDisplay(true);
  delay(2000);


}
void sendData(){
    Data = (int) round(pm25 * 10);
    Serial2.println(Data);
    Serial.println("Data to send");
    Serial.println(Data);
  }
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  initNova();

  initTFT();

}
void  loop() {

  getPM();
  sendData();
  printScreen();
  


}
