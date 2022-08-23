#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <SPI.h>
#include <Wire.h>
#include <i2cdetect.h>
#include <BH1750.h>

#include "defines.h"
#include "buttons\Encoder_KY040.h"
/* data */
int pinA = VOLUME_ENCODER_CW;   // Connected to CLK on KY-040
int pinB = VOLUME_ENCODER_DATA; // Connected to DT on KY-040
int pinSW = VOLUME_ENCODER_PRESS;
int encoderPosCount = 0;
int pinALast = 0;
int aVal = 0;
boolean bCW = 0;

#include "apps/HumanInterface/human_interface.h"
#include "apps/HumanInterface/Lcd8Digit.h"
//#include "apps/HumanInterface/LedBar.h"

String message;

Encoder_KY040 volumeEncoder(VOLUME_ENCODER_CW, VOLUME_ENCODER_DATA, VOLUME_ENCODER_PRESS);
Encoder_KY040 menuEncoder(MENU_ENCODER_CW, MENU_ENCODER_DATA, MENU_ENCODER_PRESS);

Lcd8Digit lcd8Digit;

BH1750 lightMeter;

void VolumeCount(){
  volumeEncoder.Steps();
}

void MenuCount(){
  menuEncoder.Steps();
}

void setup()
{
  // put your setup code here, to run once:

  // DEBUG OVER USB serial
  Serial.begin(9600);

  // attaching interrupts to the pins
  // attachInterrupt(VOLUME_ENCODER_CW, VolumeCount, CHANGE);
  // attachInterrupt(VOLUME_ENCODER_DATA, VolumeCount, CHANGE);
  // attachInterrupt(MENU_ENCODER_CW, MenuCount, CHANGE);
  // attachInterrupt(MENU_ENCODER_DATA, MenuCount, CHANGE);
  delay(7000);
  
  volumeEncoder.Startup();
  menuEncoder.Startup();
  
  // I2C to the light sensor
  Wire1.setSDA(D2);
  Wire1.setSCL(D3);
  Wire1.setClock(300000);
  Wire1.begin();
  Serial.println("I2C Scanning:");
  i2cdetect();
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire1);

  // starting the serial communication to the wheel on UART0
  WHEELSERIAL.setRX(WHEELRX);
  WHEELSERIAL.setTX(WHEELTX);
  WHEELSERIAL.begin(WHEELBAUD);
  Serial.println("Communication to the wheel started...");

  // lcd and led initialization
  lcd8Digit.Initialize();
  Serial.println("LCD & LED configuration DONE");
}

void loop()
{
  // put your main code here, to run repeatedly:

  /*
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
  */

  // Testing writing over SERIAL1
  //Serial.println("testing platformio + encoder");
  WHEELSERIAL.write("Hi from the wheel!");

  // reading stuff over serial
  message = "\n";
  while (WHEELSERIAL.available())
  {
    message.concat((char)WHEELSERIAL.read());
  }
  if (message.compareTo("\n")) // something has arrived
  {
    Serial.print("Message recieved: ");
    Serial.println(message);
  }

  volumeEncoder.Steps();
  menuEncoder.Steps();

  if (volumeEncoder.IsPressed())
  {
    Serial.println("Volume Encoder pressed");
  }
  if (menuEncoder.IsPressed())
  {
    Serial.println("Menu Encoder pressed");
  }

  // lcd stuff
  //lcd8Digit.DownMenu();
  lcd8Digit.Update();

  delay(DEFAULTDELAY);
  //delay(500);
}