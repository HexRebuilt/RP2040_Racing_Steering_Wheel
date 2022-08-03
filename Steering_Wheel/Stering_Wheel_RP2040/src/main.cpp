#include <Arduino.h>
#include <stdlib.h>
#include <hardware/pio.h>
#include <string.h>
#include <SPI.h>
//#include <BH1750.h>
//#include <Wire.h>

#include "defines.h"
#include "buttons\Encoder_KY040.h"
//#include "apps/HumanInterface/human_interface.h"
//#include "apps/HumanInterface/Lcd8Digit.h"
//#include "apps/HumanInterface/LedBar.h"

String message;

Encoder_KY040 volumeEncoder(VOLUME_ENCODER_CW, VOLUME_ENCODER_DATA);
Encoder_KY040 menuEncoder(MENU_ENCODER_CW, MENU_ENCODER_DATA);

// Lcd8Digit lcd8Digit;
//#include <DigitLedDisplay.h>
//MbedSPI mySPI(CIPO, COPI, SCK);
//DigitLedDisplay lcd = DigitLedDisplay(COPI, LCD_CS, SCK);

//BH1750 lightMeter;
/*
void spitest(int addr, int data){
  digitalWrite(LCD_CS, LOW);
  mySPI.transfer(addr);
  mySPI.transfer(data);
  digitalWrite(LCD_CS, HIGH);
}

void spiclear() {
  Serial.println("Clear");
   for (int i = 8; i >= 1; i--)
  {
    spitest(i, 0b0001101);
  }
}*/

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

  //attaching interrupts to the pins
  attachInterrupt(VOLUME_ENCODER_CW, VolumeCount, CHANGE);
  attachInterrupt(VOLUME_ENCODER_DATA, VolumeCount, CHANGE);
  attachInterrupt(MENU_ENCODER_CW, MenuCount, CHANGE);
  attachInterrupt(MENU_ENCODER_DATA, MenuCount, CHANGE);


  
  // starting the serial communication to the wheel on UART0
  WHEELSERIAL.begin(WHEELBAUD);
  Serial.println("Communication to the wheel started...");

  //I2C to the light sensor
  //Wire.setSDA(I2C_SDA);
  //Wire.setSCL(I2C_SCL);
  //Wire.begin(I2C_SDA, I2C_SCL);
  //lightMeter.begin();

  //mySPI.begin();

  // lcd and led initialization  
  //lcd.on();
  digitalWrite(LCD_CS, LOW);
  //mySPI.transfer(12);
  //mySPI.transfer(0x01);
  digitalWrite(LCD_CS, HIGH);
  
  //lcd.setDigitLimit(8); // 8 digit
  //lcd.setBright(MAX_BRIGHT_LCD);
  //spiclear();
  //lcd.clear();
  // lcd8Digit.Initialize();
  Serial.println("LCD & LED configuration DONE");
}

void loop()
{
  // put your main code here, to run repeatedly:
  /*Serial.println("LCD test");
  spitest(12, 1);
  spitest(4, 0b0101101);

  delay(1000);
  spiclear();

  delay(1000);
  spitest(12, 0);*/

  //float lux = lightMeter.readLightLevel();
  //Serial.print("Light: ");
  //Serial.print(lux);
  //Serial.println(" lx");


  // Testing writing over SERIAL1
  // Serial.println("testing Serial1");
  WHEELSERIAL.write("Hi from the wheel!");

  // reading stuff over serial
  message = "\n";
  while (WHEELSERIAL.available())
  {
    message.concat((char)WHEELSERIAL.read());
  }

  Serial.print("Message recieved: ");
  Serial.println(message);

  delay(500);
}