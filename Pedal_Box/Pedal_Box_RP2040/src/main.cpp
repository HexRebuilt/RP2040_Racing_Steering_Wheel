#ifndef NATIVE_BUILD
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
#endif

#ifdef NATIVE_BUILD
// Native stub
int main() {
    return 0;
}
#else

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

#endif
// LOW_BEAM state monitoring via UART
// Transmits "LOW_BEAM_ON" or "LOW_BEAM_OFF" every 500ms via Serial1 (WHEELSERIAL)

unsigned long previousBeamMillis = 0;
bool lastBeamState = LOW_BEAM_ON;

void checkBeamSignal()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousBeamMillis >= 500)
    {
        previousBeamMillis = currentMillis;

        bool currentBeamState = LOW_BEAM_ON;

        if (currentBeamState != lastBeamState)
        {
            lastBeamState = currentBeamState;

            if (currentBeamState)
            {
                // Low-beam is ON - transmit "LOW_BEAM_ON"
                WHEELSERIAL.write("LOW_BEAM_ON\n");
            }
            else
            {
                // Low-beam is OFF - transmit "LOW_BEAM_OFF"
                WHEELSERIAL.write("LOW_BEAM_OFF\n");
            }
        }
    }
}

// In loop() - call the beam signal checker
void loop()
{
    // ... existing loop code ...

    checkBeamSignal();

    // ... remaining loop code ...
}
