#include "SdsDustSensor.h"
#include <SPI.h>
#include <Adafruit_ST7735.h>
#if defined(ESP32)
  #define TFT_CS         5
  #define TFT_RST        22 
  #define TFT_DC         21
  //
  // define not needed for all pins; reference for ESP32 physical pins connections to VSPI:
  // SDA  GPIO23 aka VSPI MOSI
  // SCLK GPIO18 aka SCK aka VSPI SCK
  // D/C  GPIO21 aka A0 (also I2C SDA)
  // RST  GPIO22 aka RESET (also I2C SCL)
  // CS   GPIO5  aka chip select
  // LED  3.3V
  // VCC  5V
  // GND - GND
  //

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

  

const int rxPin = 0;
const int txPin = 1;
SdsDustSensor sds(rxPin, txPin);

void setup() {
  Serial.begin(9600);
   sds.begin();
 
  tft.initR(INITR_BLACKTAB);
    
    
    uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  
  time = millis() - time;
  Serial.println(time, DEC);
  delay(500);

  
  tft.print("Chi so bui :");
 
}
 void  loop() {
   PmResult pm = sds.readPm();
  if (pm.isOk()) {
    tft.setCursor(0, 0);
    tft.print("PM2.5 = ");
    tft.print(pm.pm25);
    tft.print(", PM10 = ");
    tft.println(pm.pm10);

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
  delay(500);
}
