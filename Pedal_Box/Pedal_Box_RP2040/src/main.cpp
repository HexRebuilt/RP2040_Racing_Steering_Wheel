#include <Arduino.h>
#include <stdlib.h>
#include <hardware/pio.h>
#include <SPI.h>
#include <TinyGPS++.h>
#include <string.h>

#include "defines.h"

unsigned short radioOutputStep = 0;

UART SerialGPS (GPSTX, GPSRX, NC, NC); 
UART SerialWheel (WHEELTX, WHEELRX, NC, NC); 


TinyGPSPlus gps;

void setup() {
  // put your setup code here, to run once:
  //DEBUG OVER USB serial
  Serial.begin(9600);

  //starting the serial communication to the wheel
  SerialWheel.begin(9600);
  Serial.println("Communication to the wheel started...");

  //gps configuration
  SerialGPS.begin(GPSBAUD);
  Serial.println("GPS STARTED");

  Serial.println("Configuration DONE");
}

void loop() {
  // put your main code here, to run repeatedly:

  //GPS reading info
  while (SerialGPS.available()){
    gps.encode(SerialGPS.read());
  }
  
}