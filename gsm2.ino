
#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Suli.h>
/////////////////

#include <TinyGPS++.h>
#include <SPI.h>
#include <SD.h>

////////////////////////
#define PIN_TX    7
#define PIN_RX    8
#define BAUDRATE  9600

#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;

char phone[16];
char datetime[24];


//////////////////////////////
static const int RXPin = 2, TXPin = 3;
static const unsigned long GPSBaud = 9600;
const int chipSelect = 4;

char* gpsfileName = "gps.csv";
File gpsfile;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial gpsserial(RXPin, TXPin);

String FullGpsMsg;
  char toemptytemp[20];
  
///////////////////////////////////
GPRS gprsTest(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,PWR,BaudRate
/////////////////////////////////
void setup() {
  Serial.begin(9600);
  Serial.println("WWWWWWWWWWWWWWOOOOOOOOOOOOOOOOOOOOOOOOO");
     while(0 != gprsTest.init()) {
      delay(1000);
      Serial.print("init error\r\n");
  }
  delay(3000);  
  Serial.println("Init Success, please send SMS message to me!"); 

Serial.println("WWWWWWWWWWWWWWOOOOOOOOOOOOOOOOOOOOOOOOO");
  /////////////////////////////
  gpsserial.begin(GPSBaud);
  //pinMode(sdokpin, OUTPUT);
  
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
    if (SD.exists(gpsfileName)) {
    Serial.println("gps.csv exists.");
  } else {
    Serial.println("Creating gps.csv...");
    gpsfile = SD.open(gpsfileName, FILE_WRITE);
    gpsfile.println("latitude,longitude,alt(feet),date,time");
    
    gpsfile.close();
    Serial.println("gps.csv doesn't exist.So i create it.");
  }
  Serial.println("WWWWWWWWWWWWWWOOOOOOOOOOOOOOOOOOOOOOOOO");
}

void loop() {
  Serial.println("WWWWWWWWWWWWWWOOOOOOOOOOOOOOOOOOOOOOOOO");
   messageIndex = gprsTest.isSMSunread();
   if (messageIndex > 0) { //At least, there is one UNREAD SMS
      gprsTest.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);           
      //In order not to full SIM Memory, is better to delete it
      gprsTest.deleteSMS(messageIndex);
      Serial.print("From number: ");
      Serial.println(phone);  
      Serial.print("Datetime: ");
      Serial.println(datetime);        
      Serial.print("Recieved Message: ");
      Serial.println(message);
      
      String gogo = message;
      if(gogo == "hello"){
       Serial.println("WWWWWWWWWWWWWWOOOOOOOOOOOOOOOOOOOOOOOOO");
         Serial.println("gprs init success");
         Serial.println("start to send message ...");
         gprsTest.sendSMS(phone,"hello to you"); //define phone number and text
   }
   //////////////////////////////
     // This sketch displays information every time a new sentence is correctly encoded.
 while (gpsserial.available() > 0)
    if (gps.encode(gpsserial.read()))
      displayInfo();
      
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
    }
 
}


void displayInfo()
{
  FullGpsMsg = "";

  if (gps.location.isValid())
  {
    FullGpsMsg += dtostrf(gps.location.lat(),2, 6,toemptytemp);
    FullGpsMsg += " , ";
    FullGpsMsg += dtostrf(gps.location.lng(),2, 6,toemptytemp);
    FullGpsMsg += " , ";
    FullGpsMsg += dtostrf(gps.altitude.feet(),2, 6,toemptytemp);
    FullGpsMsg += " , ";
    FullGpsMsg += String(gps.date.day());
    FullGpsMsg += "/";
    FullGpsMsg += String(gps.date.month());
    FullGpsMsg += "/";
    FullGpsMsg += String(gps.date.year());
    FullGpsMsg += ","; 
    
    FullGpsMsg += String(gps.time.hour());
    FullGpsMsg += ":";
    FullGpsMsg += String(gps.time.minute());
    FullGpsMsg += ":";
    FullGpsMsg += String(gps.time.second());
    FullGpsMsg += ":";   
    FullGpsMsg += String(gps.time.centisecond());
    savegpslog();
  }
  else
  {
    Serial.println(F("INVALID"));
  }


  
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

void savegpslog(){

  gpsfile = SD.open(gpsfileName, FILE_WRITE);
  if (!gpsfile) {
    Serial.println(F("open failed"));
    return;
  }
  Serial.println(F("I opened File: "));
  Serial.println("I am Writing to File...");
  gpsfile.println(FullGpsMsg);
  gpsfile.close();
}

