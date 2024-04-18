#include <SoftwareSerial.h>
// #include <AltSoftSerial.h>


SoftwareSerial GSM_Serial(2, 3); /* (Rx, Tx) */
char c;

void update_Serial();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  GSM_Serial.begin(9600);

  Serial.println("Initializing...........");
  delay(1000);

  GSM_Serial.println("AT");  //Once the handshake test is successful, it will back to OK
  update_Serial();
  GSM_Serial.println("AT+CSQ");  //Signal quality test, value range is 0-31 , 31 is the best
  update_Serial();
  GSM_Serial.println("AT+CCID");  //Read SIM information to confirm whether the SIM is plugged
  update_Serial();
  GSM_Serial.println("AT+CREG?");  //Check whether it has registered in the network

  Send_Msg();
}

void loop() {
  // put your main code here, to run repeatedly:

  // if(GSM_Serial.isListening()){
  //   Serial.println("$$$$$$$$$$$$$$$");
  // }
  update_Serial();
}

void update_Serial() {
  //read from SIM900 and send to Serial Monitor
  if (GSM_Serial.available()) {
    // Serial.println("****GSM_AVAILABLE***");
    c = GSM_Serial.read();
    Serial.write(c);
  }

  //read from Serial Monitor and send to SIM900
  if (Serial.available()) {
    c = Serial.read();
    GSM_Serial.write(c);
  }
}


void Send_Msg() {
  Serial.println("..................Sending Message Started...............");
  GSM_Serial.println("AT+CMGF=1");  //Configuring Text mode
  delay(2000);

  GSM_Serial.println("AT+CMGS=\"+254114196099\"");  // Replace x with mobile number
  delay(2000);

  Serial.println("Sending Message.........................");
  GSM_Serial.println("Temperature Over 30.....");  //SMS text you want to send
  delay(2000);

  GSM_Serial.println((char)26);  //ASCII code of CTRL+Z
  delay(2000);

  Serial.println("..................Sending Message Ended...............");
}