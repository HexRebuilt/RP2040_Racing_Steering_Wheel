#include <Arduino.h>
#include <stdlib.h>
#include <hardware/pio.h>
#include <SPI.h>
#include <TinyGPS++.h>
#include <string.h>

#include "defines.h"
#include "apps/utils/timer.h"
  
//GPS stuff
UART SerialGPS(GPSTX, GPSRX, NC, NC);
TinyGPSPlus gps;
unsigned int speed, satellites;

//Radio output stuff
unsigned short radioOutputStep = 0, numberRecieved = 0;
MbedSPI mySPI(CIPO, COPI, SCK);
#include "mpc4131.h" //functions for the digiral pot
Timer timer;

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

}

void loop()
{
  // put your main code here, to run repeatedly:

  //Serial.println("Testing pot output");
  //TestPOT();

  //Testing writing over SERIAL1
  //WHEELSERIAL.write("Hi from the pedal Box\n");
  
  message = "\n";
  while (WHEELSERIAL.available())
  {
    message.concat( (char) WHEELSERIAL.read() );
  }

  if (message.compareTo("\n"))
  {
    Serial.print("Message recieved: ");
    Serial.println(message);

    numberRecieved = message.toInt(); 

    if (numberRecieved <= RPUSH)
    {
      radioOutputStep = map(numberRecieved, RGREEN, RPUSH, POT_MIN, POT_MAX);
      digitalPotWrite(radioOutputStep);
      timer.startTimer();
      numberRecieved = 0;
    }
    
  }
  
  //reset pot value after set ammount of time
  if (timer.timePassed() > ANALOG_OUTPUT_CONFIG_TIME && radioOutputStep != 0)
  {
    digitalPotWrite(0);
    radioOutputStep = 0;
  }
  

  //delay(500);
  //TestPOT();
  

  // GPS reading info
  while (SerialGPS.available())
  {
    gps.encode(SerialGPS.read());
  }

  /*
  Serial.print("gps.speed: ");
  Serial.println((int) gps.speed.kmph());
  Serial.print("gps.satellites: ");
  Serial.println(gps.satellites.value());
  Serial.print("gps.time: ");
  Serial.println(gps.time.hour());
  */

  //Writing GPS info to the wheel
  message = "\n";
  message.concat("gps.speed:\t");
  message.concat((int) gps.speed.kmph());
  message.concat("\n");

 //WHEELSERIAL.write((string) message);

  delay(DEFAULTDELAY);
}