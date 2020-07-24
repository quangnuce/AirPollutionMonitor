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
SoftwareSerial novaSerial(txPin,rxPin);
SdsDustSensor sds(novaSerial);


// SIM 808
#define PIN_TX   2
#define PIN_RX    3
SoftwareSerial mySerial(PIN_TX,PIN_RX);
DFRobot_SIM808 sim808(&mySerial);//Connect RX,TX,PWR,


//data:
float pm25;
float pm10;
float last_lat;
float last_lng;


void initSIM(){
  
  mySerial.begin(9600);
   while(!sim808.init()) { 
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }

  //************* Turn on the GPS power************
  if( sim808.attachGPS())
      Serial.println("Open the GPS power success");
  else 
      Serial.println("Open the GPS power failure");
 }
 void initNova(){
   novaSerial.begin(9600);
    sds.begin();
    //Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
    //Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
    //Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended
  }
 void initTFT(){
  
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

void getGPS(float &lat,float &lng){
    if (sim808.getGPS()) {
       
      Serial.print("latitude :");
      Serial.println(sim808.GPSdata.lat,6);
   
      Serial.print("longitude :");
      Serial.println(sim808.GPSdata.lon,6);

      sim808.detachGPS();
      lat=sim808.GPSdata.lat;
      lng=sim808.GPSdata.lon;
    }
  
  }
 float getPM(float &pm25,float &pm10){
    PmResult pm = sds.readPm();
    if (pm.isOk()) {
     // tft print function!
       Serial.print(pm.pm25);
       Serial.print(pm.pm10);
       pm25=pm.pm25;
       pm10=pm.pm10;
 
    }
    else {
      // notice that loop delay is set to 0.5s and some reads are not available
      Serial.print("Could not read values from sensor, reason: ");
      Serial.println(pm.statusToString());
    }
   
   
 }
 void printScreen(){
  
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
void setup() {   
  Serial.begin(9600);
  initSIM();
  initNova();
  initTFT();
   
}
 void  loop() {
  getGPS(last_lat,last_lng);
  getPM(pm25,pm10);
  printScreen();
  
}
