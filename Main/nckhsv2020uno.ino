

#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
//internet config
#define TINY_GSM_MODEM_SIM808
#include <TinyGsmClient.h>
#include "ThingsBoard.h"
// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "v-internet";
const char user[] = "";
const char pass[] = "";

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN               "svnckh2020"
#define THINGSBOARD_SERVER  "iot.sytes.net"
#define THINGSBOARD_PORT    8080
// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   9600

// Serial port for GSM shield
SoftwareSerial serialGsm(2, 3); // RX, TX pins for communicating with modem

// Initialize GSM modem
TinyGsm modem(serialGsm);

// Initialize GSM client
TinyGsmClient client(modem);

// Initialize ThingsBoard instance
ThingsBoardHttp tb(client, TOKEN, THINGSBOARD_SERVER, THINGSBOARD_PORT);

// Set to true, if modem is connected
bool modemConnected = false;




// MEGA
//const int rxPin = 6;
//const int txPin = 5;
//SoftwareSerial megaSerial(txPin, rxPin);





//data:
float pm25 = 12.0;
float pm10;
float last_lat = 20.988322;
float last_lng = 105.842436;

char myData[10] = "";
int i = 0;



void initModem() {

  serialGsm.begin(115200);

  delay(3000);
  // Lower baud rate of the modem.
  // This is highly practical for Uno board, since SoftwareSerial there
  // works too slow to receive a modem data.
  serialGsm.write("AT+IPR=9600\r\n");
  serialGsm.end();
  serialGsm.begin(9600);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println(F("Initializing modem..."));
  modem.restart();

  String modemInfo = modem.getModemInfo();
  Serial.print(F("Modem: "));
  Serial.println(modemInfo);
}

void sendData() {

  delay(1000);
  if (!modemConnected) {
    Serial.print(F("Waiting for network..."));
    if (!modem.waitForNetwork()) {
      Serial.println(" fail");
      delay(10000);
      return;
    }
    Serial.println(" OK");

    Serial.print(F("Connecting to "));
    Serial.print(apn);
    if (!modem.gprsConnect(apn, user, pass)) {
      Serial.println(" fail");
      delay(10000);
      return;
    }

    modemConnected = true;
    Serial.println(" OK");
  }


//  //get GPS
//  double gps_speed, gps_altitude, gps_view_satellites, gps_used_satellites;
//  int gps_fixstatus = modem.getGPS(&last_lat, &last_lng);
//  if ( gps_fixstatus ) {
//    Serial.println("get GPS ok...");
//
//  }


  Serial.println("Sending data...");

  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details
  char* mydata = "";
  char str_pm[5];
  dtostrf(pm25, 5, 1, str_pm);
  char str_lat[17];
  dtostrf(last_lat, 10, 7, str_lat);
  char str_lng[17];
  dtostrf(last_lng, 10, 7, str_lng);
  sprintf(mydata, "{\"p25\":\"%s\", \"lat\":\"%s\",\"lng\":\"%s\"}", str_pm, str_lat, str_lng);
  Serial.println(mydata);
    tb.sendTelemetryJson("{\"p25\":\"240\", \"lat\":\"20.988322\",\"lng\":\"105.842436\"}");

}


void setup() {
  Serial.begin(9600);
  Serial.println("setup");
  initModem();
}

void  loop() {
  // recieveData();
  sendData();
  Serial.print("pm25:");
  Serial.println(pm25);

  delay(1000);

}
