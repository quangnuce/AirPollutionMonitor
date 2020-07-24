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
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

  
// NOVA PM SENSOR 
const int rxPin = 5;
const int txPin = 6;
SdsDustSensor sds(rxPin, txPin);


// SIM 808
#define PIN_TX    5
#define PIN_RX    6
SoftwareSerial mySerial(PIN_TX,PIN_RX);
DFRobot_SIM808 sim808(&mySerial);//Connect RX,TX,PWR,




void setup() {
  // KHOI TAO SIM 808
  mySerial.begin(9600);
  Serial.begin(9600);
  //******** Initialize sim808 module *************
  while(!sim808.init()) { 
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }

  //************* Turn on the GPS power************
  if( sim808.attachGPS())
      Serial.println("Open the GPS power success");
  else 
      Serial.println("Open the GPS power failure");
  
  
  // KHOI TAO NOVA PM SENSOR
  //Serial.begin(9600);
  //sds.begin();

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
  testdrawtext("KHOI TAO MAN HINH TFT THANH CONG \n THANK YOU ", ST77XX_WHITE);
  delay(2000);


  
 
}
 void  loop() {
    /*PmResult pm = sds.readPm();
    if (pm.isOk()) {
     // tft print function!
    tftPrintTest();
    delay(2000);
    // if you want to just print the measured values, you can use toString() method as well
    tft.println(pm.toString());
  } else {
    // notice that loop delay is set to 0.5s and some reads are not available
    tft.print("Could not read values from sensor, reason: ");
    tft.println(pm.statusToString());
  }
  tft.invertDisplay(true);
  delay(500);
  tft.invertDisplay(false);
  delay(500);*/

  //SIM 808
  //************** Get GPS data *******************
   if (sim808.getGPS()) {
    Serial.print(sim808.GPSdata.year);
    Serial.print("/");
    Serial.print(sim808.GPSdata.month);
    Serial.print("/");
    Serial.print(sim808.GPSdata.day);
    Serial.print(" ");
    Serial.print(sim808.GPSdata.hour);
    Serial.print(":");
    Serial.print(sim808.GPSdata.minute);
    Serial.print(":");
    Serial.print(sim808.GPSdata.second);
    Serial.print(":");
    Serial.println(sim808.GPSdata.centisecond);
    Serial.print("latitude :");
    Serial.println(sim808.GPSdata.lat,6);
    sim808.latitudeConverToDMS();
    Serial.print("latitude :");
    Serial.print(sim808.latDMS.degrees);
    Serial.print("\^");
    Serial.print(sim808.latDMS.minutes);
    Serial.print("\'");
    Serial.print(sim808.latDMS.seconeds,6);
    Serial.println("\"");
    Serial.print("longitude :");
    Serial.println(sim808.GPSdata.lon,6);
    sim808.LongitudeConverToDMS();
    Serial.print("longitude :");
    Serial.print(sim808.longDMS.degrees);
    Serial.print("\^");
    Serial.print(sim808.longDMS.minutes);
    Serial.print("\'");
    Serial.print(sim808.longDMS.seconeds,6);
    Serial.println("\"");
    
    Serial.print("speed_kph :");
    Serial.println(sim808.GPSdata.speed_kph);
    Serial.print("heading :");
    Serial.println(sim808.GPSdata.heading);

    //************* Turn off the GPS power ************
    sim808.detachGPS();
  }
}
void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 40);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}
void tftPrintTest() // HÀM IN RA CHỈ SỐ BỤI MIN TRONG KHÔNG KHÍ 
{
  PmResult pm = sds.readPm();
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 40);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.println("CHI SO BUI TRONG KHONG KHI:");
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.print("PM2.5 = ");
  tft.println(pm.pm25);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(1);
  tft.print("PM10 = ");
  tft.println(pm.pm10);
  delay(2000);
  /*tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");*/
}
