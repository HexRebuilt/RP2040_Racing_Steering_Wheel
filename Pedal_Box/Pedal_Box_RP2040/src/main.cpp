#include <Arduino.h>
#include <stdlib.h>
#include <hardware/pio.h>
#include <SPI.h>
#include <TinyGPS++.h>
#include <string.h>

#include "defines.h"
#include "mpc4131.h"

unsigned short radioOutputStep = 0;

UART SerialGPS (GPSTX, GPSRX, NC, NC); 

MPC4131 radioPot;

TinyGPSPlus gps;

void setup() {
  // put your setup code here, to run once:
  //DEBUG OVER USB serial
  Serial.begin(9600);

  //starting the serial communication to the wheel on UART0
  Serial1.begin(WHEELBAUD);
  Serial.println("Communication to the wheel started...");

  //gps configuration on UART1
  SerialGPS.begin(GPSBAUD);
  Serial.println("GPS STARTED");

  //Potentiometer startup
  radioPot.Startup();
  Serial.println("Potentiometer initialized");

  Serial.println("Configuration DONE");
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("SON VIVO");
  delay(1000);

  //GPS reading info
  while (SerialGPS.available()){
    gps.encode(SerialGPS.read());
  }
  
}