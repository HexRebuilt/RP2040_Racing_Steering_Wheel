#include <Arduino.h>
#include <stdlib.h>
#include <hardware/pio.h>
#include <string.h>

#include "defines.h"
String message;

void setup() {
  // put your setup code here, to run once:
  // DEBUG OVER USB serial
  Serial.begin(9600);
  delay(DEFAULTDELAY * 500);

  // starting the serial communication to the wheel on UART0
  WHEELSERIAL.begin(WHEELBAUD);
  Serial.println("Communication to the wheel started...");

}

void loop() {
  // put your main code here, to run repeatedly:
//Testing writing over SERIAL1
  //Serial.println("testing Serial1");
  WHEELSERIAL.write("Hi from the wheel!");
  
  //reading stuff over serial
  message = "\n";
  while (WHEELSERIAL.available())
  {
    message.concat( (char) WHEELSERIAL.read() );
  }
  
  Serial.print("Message recieved: ");
  Serial.println(message);
    
  delay(500);
}