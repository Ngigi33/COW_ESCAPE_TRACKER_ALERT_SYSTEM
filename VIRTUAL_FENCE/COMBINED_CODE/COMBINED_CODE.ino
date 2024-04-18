#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>

#include <LiquidCrystal.h>               // includes the LiquidCrystal Library
LiquidCrystal lcd(4, 10, 12, 9, 11, 8);  // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

SoftwareSerial GPS_SoftSerial(13, A1); /* (Rx, Tx) */
//SoftwareSerial GSM_Serial(2, 3);       /* (Rx, Tx) */
char c;

/* Create an object named gps of the class TinyGPSPlus */
TinyGPSPlus gps;
String lat = "0.0000000";
String lon = "0.0000000";
String googleMapsLink = "https://www.google.com/maps?q=0.0000000,0.0000000";


TinyGPSCustom pdop(gps, "GNGLL", 1);  // $GPGSA sentence, 15th element
TinyGPSCustom hdop(gps, "GNGLL", 3);  // $GPGSA sentence, 16th element

int fence_tamper = A3;
int battery = A4;
int battery_Level;
int pot;
int i = 0;


String createGoogleMapsLink() {
  String googleMapsLink = "https://www.google.com/maps?q=" + lat + "," + lon;
  return googleMapsLink;
}


byte customChar[] = {
  B01110,
  B11111,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111
};



void setup() {
  // put your setup code here, to run once:
  pinMode(fence_tamper, INPUT);
  pinMode(battery, INPUT);
  Serial.begin(115200);       /* Define baud rate for serial communication */
  GPS_SoftSerial.begin(9600); /* Define baud rate for software serial communication */
  //GSM_Serial.begin(9600 );

  //Serial.println("Initializing...........");
  // delay(100);
  // GSM_Serial.println("AT");  //Once the handshake test is successful, it will back to OK
  // update_Serial();
  // GSM_Serial.println("AT+CSQ");  //Signal quality test, value range is 0-31 , 31 is the best
  // update_Serial();
  // GSM_Serial.println("AT+CCID");  //Read SIM information to confirm whether the SIM is plugged
  // update_Serial();
  // GSM_Serial.println("AT+CREG?");  //Check whether it has registered in the network

  lcd.begin(16, 2);  // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display }
  lcd.createChar(0, customChar);
  lcd.home();
  lcd.clear();
}





void Receive_Msg();
void Send_Msg(String data);



void loop() {
  pot = analogRead(battery);
  battery_Level = map(pot, 10, 930, 0, 100);
  while (GPS_SoftSerial.available() > 0)
    gps.encode(GPS_SoftSerial.read());


  lat = String(gps.location.lat(), 4);
  lon = String(gps.location.lng(), 4);
  String googleMapsLink = createGoogleMapsLink();


  if (battery_Level <= 40) {
    String msg = "BATTERY LOW  " + googleMapsLink;
    Send_Msg(msg);
    lcd.clear();
    lcd.print("..BATTERY LOW......");
    lcd.setCursor(3, 2);
    lcd.write((byte)0);
    lcd.setCursor(7, 2);
    lcd.print(battery_Level);
    delay(500);
  } else {

    if (Serial.available() > 0) {
      Receive_Msg();
      String received = Serial.readString();
      if (received.equals("LOCATION")) {
        Send_Msg(googleMapsLink);
      }
    }

    if (digitalRead(fence_tamper) == LOW) {
      while (digitalRead(fence_tamper) == LOW) {
        String msg = "SYSTEM TAMPERED  " + googleMapsLink;
        Send_Msg(msg);
        lcd.clear();
        lcd.print(".....SYSTEM......");
        lcd.setCursor(0, 2);
        lcd.print("....TAMPERED....");
        delay(500);
      }

      i = 0;
    } else {

      if (i == 0) {
        String msg = "TAMPER CORRECTED  " + googleMapsLink;
        Send_Msg(msg);
        delay(200);
        lcd.clear();
        lcd.print(".....TAMPER......");
        lcd.setCursor(0, 2);
        lcd.print("....CORRECTED....");
        delay(2000);
        i++;
      }

      lcd.clear();
      String power_level = String(battery_Level) + "%";
      lcd.print("LAT:" + lat);
      lcd.setCursor(12, 0);
      lcd.print("PWR:");
      lcd.setCursor(0, 1);
      lcd.print("LON:" + lon);
      lcd.setCursor(12, 1);
      lcd.print(power_level);
      delay(500);
    }
  }
  if (lat.equals("-0.3947")) {
    String msg = "COW OUT OF FENCE  " + googleMapsLink;
    Send_Msg(msg);
    lcd.clear();
    lcd.print(".....COW OUT......");
    lcd.setCursor(0, 2);
    lcd.print("....OF FENCE....");
    lat = "0.0000000";
    delay(500);
  }
}

void Send_Msg(String data) {
  // Serial.println("..................Sending Message Started...............");
  Serial.println("AT+CMGF=1");  //Configuring Text mode
  delay(500);
  Serial.println("AT+CMGS=\"+254700724427\"");  // Replace x with mobile number
  //Serial.println("AT+CMGS=\"+254707377751\"");
  //Serial.println("AT+CMGS=\"+254114196099\"");
  // Serial.println("AT+CMGS=\"+254746011197\"");  // Replace x with mobile number
  delay(500);
  // Serial.println("Sending Message.........................");
  Serial.println(data);  //SMS text you want to send
  delay(100);
  Serial.println((char)26);  //ASCII code of CTRL+Z
  delay(500);
  // Serial.println("..................Sending Message Ended...............");
}

void Receive_Msg() {
  Serial.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
}



// void update_Serial() {
//   //read from SIM900 and send to Serial Monitor
//   if (GSM_Serial.available()) {
//     // Serial.println("****GSM_AVAILABLE***");
//     c = GSM_Serial.read();
//     Serial.write(c);
//   }

//   //read from Serial Monitor and send to SIM900
//   if (Serial.available()) {
//     c = Serial.read();
//     GSM_Serial.write(c);
//   }
// }