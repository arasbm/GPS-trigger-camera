/**
 *
 * GPS Logger and Camera Trigger
 * This program records a GPS point in the SD card and triggers a camera snapshot using PWM output
 *
 * Author: Aras Balali Moghaddam
 *
 * SD card pins:
 * MOSI - pin 11
 * MISO - pin 12
 * CLK - pin 13
 * CS - pin 4
 * 
 * Wiring 
 *
 */

#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

int gpsRX = 2;
int gpsTX = 3;
char inByte;
int photoCount;
int triggerPin = 5;
int ledPin = 6;
String gpsData = "";


boolean hasSD = false;
File gpsFile;

SoftwareSerial gpsSerial(gpsRX, gpsTX);

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (!SD.begin(4)) {
    Serial.println("SD initialization failed!");
    hasSD = false;
  } else {
    Serial.println("SD initialization done.");
    hasSD = true;
  }

  gpsSerial.begin(57600);
  photoCount = 0;
}

void triggerCamera() {
    analogWrite(triggerPin, 200);
    delay(100);
    analogWrite(triggerPin, 0);
}

void loop() { 
  gpsData = "";
  gpsSerial.listen();

  // If gps status is ok blink led otherwise solid on
  digitalWrite(ledPin, HIGH);
  if (hasSD) {
    gpsFile = SD.open("gps.txt", FILE_WRITE);
    if (gpsFile) {
      if(gpsSerial.available()){    
        photoCount = photoCount + 1;
        gpsData = gpsData + "Photo #" + photoCount + "\n";
        //triggerCamera();
        while(gpsSerial.available()) {
          inByte = gpsSerial.read();
          gpsData = gpsData + inByte;
        }
        gpsData = gpsData + "\n";
        gpsFile.print(gpsData);
        Serial.println(gpsData);
      }
    } else {
      Serial.println("Error opening gps.txt");
    }
    gpsFile.close();
  } else {
    //gpsData = gpsData + "Photo #" + photoCount + "\n";
    //triggerCamera();
    while(gpsSerial.available()) {
      inByte = gpsSerial.read();
      gpsData = gpsData + inByte;
    }
    //gpsData = gpsData + "\n";
    Serial.print(gpsData);
  }
}

