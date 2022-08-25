#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <SPI.h>
#include <Wire.h>
#include <i2cdetect.h>
#include <BH1750.h>

#include "defines.h"
#include "button_and_encoders\Encoder_KY040.h"
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

void VolumePress(){
  volumeEncoder.Press();
}

void MenuCount(){
  menuEncoder.Steps();
}

void MenuPress(){
  menuEncoder.Press();
}

void MenuUP(){
  Serial.println("Menu UP");
  lcd8Digit.UpMenu();
}

void MenuDown(){
  Serial.println("Menu DOWN");
  lcd8Digit.DownMenu();
}

void RadioNext(){
  Serial.println("Radio NEXT");
  //TODO
}

void RadioBack(){
  Serial.println("Radio BACK");
  //TODO
}

void setup()
{
  // put your setup code here, to run once:

  // DEBUG OVER USB serial
  Serial.begin(9600);

  

  delay(7000);
  
  volumeEncoder.Startup();
  menuEncoder.Startup();
  // attaching interrupts to the pins
  attachInterrupt(VOLUME_ENCODER_CW, VolumeCount, CHANGE);
  attachInterrupt(VOLUME_ENCODER_DATA, VolumeCount, CHANGE);
  attachInterrupt(VOLUME_ENCODER_PRESS, VolumePress,FALLING);
  attachInterrupt(MENU_ENCODER_CW, MenuCount, CHANGE);
  attachInterrupt(MENU_ENCODER_DATA, MenuCount, CHANGE);
  attachInterrupt(MENU_ENCODER_PRESS, MenuPress, FALLING);

  //configuring the input pins 
  // // rockers
  pinMode(MENU_UP, INPUT_PULLDOWN);
  pinMode(MENU_DOWN, INPUT_PULLDOWN);
  pinMode(RADIO_NEXT, INPUT_PULLDOWN);
  pinMode(RADIO_BACK, INPUT_PULLDOWN);
  attachInterrupt(MENU_UP, MenuUP, RISING);
  attachInterrupt(MENU_DOWN, MenuDown, RISING);
  attachInterrupt(RADIO_NEXT, RadioNext, RISING);
  attachInterrupt(RADIO_BACK, RadioBack, RISING);
  // // buttons
  pinMode(ECU_RED,INPUT_PULLUP);
  pinMode(ECU_YELLOW,INPUT_PULLUP);
  pinMode(ECU_WHITE,INPUT_PULLUP);
  pinMode(RADIO_BLACK,INPUT_PULLUP);
  pinMode(RADIO_RED,INPUT_PULLUP);
  pinMode(RADIO_BLUE,INPUT_PULLUP);
  pinMode(RADIO_GREEN,INPUT_PULLUP);




  // I2C to the light sensor
  Wire1.setSDA(D2);
  Wire1.setSCL(D3);
  Wire1.setClock(400000);
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

  //reading the encoders
  volumeEncoder.Steps();
  menuEncoder.Steps();
  if (volumeEncoder.IsPressed())
  {
    Serial.println("Volume Encoder pressed");
    volumeEncoder.released();
  }
  if (menuEncoder.IsPressed())
  {
    Serial.println("Menu Encoder pressed");
    menuEncoder.released();
  }


  //IO reading test
  if (!digitalRead(ECU_RED))
  {
    Serial.println("HELLO");
  }
  

  // lcd stuff
  //lcd8Digit.DownMenu();
  lcd8Digit.ModifyValues(menuEncoder.Steps());
  lcd8Digit.Update();

  delay(DEFAULTDELAY);
  //delay(500);
}