#include <Arduino.h>
#include <stdlib.h>
#include <hardware/pio.h>
#include <SPI.h>
#include <TinyGPS++.h>
#include <string.h>

#include "defines.h"
#include "buttonhandler.h"
#include "buttonIds.h"
#include "apps/utils/timer.h"
  
//GPS stuff
UART SerialGPS(GPSTX, GPSRX, NC, NC);
TinyGPSPlus gps;
unsigned int speed, satellites;

//Radio output stuff
MbedSPI mySPI(CIPO, COPI, SCK);
#include "mpc4131.h" //functions for the digiral pot
ButtonHandler buttonHandler;
Timer timer, speed_timer;

String message;

void setup()
{
  // put your setup code here, to run once:
  // DEBUG OVER USB serial
  Serial.begin(9600);
  delay(DEFAULTDELAY * 500);

  // starting the serial communication to the wheel on UART0
  WHEELSERIAL.begin(WHEELBAUD);
  Serial.println("Communication to the wheel started...");

  // gps configuration on UART1
  SerialGPS.begin(GPSBAUD);
  Serial.println("GPS started...");

  // Digital potentiometer startup
  pinMode(RADIO_MPC4131_CS, OUTPUT);
  mySPI.begin();
  digitalPotWrite(0);
  Serial.println("SPI initialized..");
  Serial.println("Configuration DONE");

  speed_timer.startTimer();

}

void loop()
{
  // put your main code here, to run repeatedly:
  //Serial.println("Testing pot output");
  //TestPOT();
 
  
  message = "\n";
  /*while (WHEELSERIAL.available())
  {
    message.concat( (char) WHEELSERIAL.read() );
  }*/
  if (WHEELSERIAL.available())
  {
    message=WHEELSERIAL.readStringUntil('\n');
  }
  

  if (message.compareTo("\n"))
  {
    Serial.print("Message recieved: ");
    Serial.println(message);
    //analyzing the message
    digitalPotWrite (buttonHandler.convertIDtoPot(message));
    timer.startTimer();
    message = "\n";
  }
    
  
  //reset pot value after set ammount of time
  if (timer.timePassed() > ANALOG_OUTPUT_CONFIG_TIME)
  {
    digitalPotWrite(0);
  }
  

  // GPS reading info
  while (SerialGPS.available())
  {
    gps.encode(SerialGPS.read());
  }

  //Writing GPS info to the wheel depending on the time passed

  if (speed_timer.timePassed()>=20)
  {
    message = "g.sp.";
    message.concat((int) gps.speed.kmph());
    message.concat("\t");
    message.concat("g.st.");
    message.concat((int) gps.satellites.value());
    message.concat("\t");
    message.concat("g.h.");
    message.concat((int) gps.time.hour());
    message.concat("\t");
    message.concat("g.m.");
    message.concat((int) gps.time.minute());
    message.concat("\t");
    message.concat("g.s.");
    message.concat((int) gps.time.second());
    message.concat("\n");
    WHEELSERIAL.write(message.c_str());
    speed_timer.startTimer();
  }
  


  delay(DEFAULTDELAY);
}