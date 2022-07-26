#include <Arduino.h>
#include <stdlib.h>
#include <hardware/pio.h>
#include <string.h>

#include "defines.h"

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
}