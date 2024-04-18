#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/* Create object named bt of the class SoftwareSerial */
SoftwareSerial GPS_SoftSerial(13, A1); /* (Rx, Tx) */
/* Create an object named gps of the class TinyGPSPlus */
TinyGPSPlus gps;
String lat = "0.0000000";
String lon = "0.0000000";

volatile float old_latitude;
volatile float new_latitude;
volatile float old_longitude;
volatile float new_longitude;

volatile float minutes, seconds;
volatile int degree, secs, mins;

volatile float earth_Circumference=40030170.0;
volatile float change_lat;
volatile float change_long;

TinyGPSCustom pdop(gps, "GNGLL", 1);  // $GPGSA sentence, 15th element
TinyGPSCustom hdop(gps, "GNGLL", 3);  // $GPGSA sentence, 16th element

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);         /* Define baud rate for serial communication */
  GPS_SoftSerial.begin(9600); /* Define baud rate for software serial communication */
}

void loop() {
  // put your main code here, to run repeatedly:
  while (GPS_SoftSerial.available() > 0)
    gps.encode(GPS_SoftSerial.read());


  lat = String(gps.location.lat(), 8);
  lon = String(gps.location.lng(), 8);

  change_lat=earth_Circumference*




  // lat = String(atof(pdop.value())/100);
  // lon = String(atof(hdop.value())/100);


  Serial.println("LATITUDE:" + lat + " " + "LONGITUDE:" + lon);
}
