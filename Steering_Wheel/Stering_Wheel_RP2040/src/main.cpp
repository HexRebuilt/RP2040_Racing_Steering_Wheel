#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <SPI.h>
#include <Wire.h>
#include <i2cdetect.h>
#include <BH1750.h>

#include "defines.h"
#include "button_and_encoders\Encoder_KY040.h"
#include "apps/utils/timer.h"

#include "apps/HumanInterface/human_interface.h"
#include "apps/HumanInterface/Lcd8Digit.h"
// #include "apps/HumanInterface/LedBar.h"

String messageIn = "\n";
String messageOut = "\n";

Timer timer;

Encoder_KY040 volumeEncoder(VOLUME_ENCODER_CW, VOLUME_ENCODER_DATA, VOLUME_ENCODER_PRESS);
Encoder_KY040 menuEncoder(MENU_ENCODER_CW, MENU_ENCODER_DATA, MENU_ENCODER_PRESS);
int oldval = 0, val = 0;

Lcd8Digit lcd8Digit;

BH1750 lightMeter;

void VolumeCount()
{
  volumeEncoder.Steps();
}

void VolumePress()
{
  volumeEncoder.Press();
  messageOut = RPUSH;
}

void MenuCount()
{
  menuEncoder.Steps();
}

void MenuPress()
{
  menuEncoder.Press();
  // tbd action
}

void MenuUP()
{
  Serial.println("Menu UP");
  lcd8Digit.UpMenu();
}

void MenuDown()
{
  Serial.println("Menu DOWN");
  lcd8Digit.DownMenu();
}

void RadioNext()
{
  Serial.println("Radio NEXT");
  messageOut = RUP;
}

void RadioBack()
{
  Serial.println("Radio BACK");
  messageOut = RDOWN;
}

void RadioGreen()
{
  Serial.println("Radio GREEN");
  messageOut = RGREEN;
}

void RadioRED()
{
  Serial.println("Radio RED");
  messageOut = RRED;
}

void RadioBlue()
{
  Serial.println("Radio BLUE");
  messageOut = RBLUE;
}

void RadioBlack()
{
  Serial.println("Radio BLACK");
  messageOut = RBLACK;
}

void EcuRed()
{
  Serial.println("ECU RED");
  messageOut = LRED;
}

void EcuYellow()
{
  Serial.println("ECU YELLOW");
  messageOut = LYELLOW;
}

void EcuWhite()
{
  Serial.println("ECU WHITE");
  messageOut = LWHITE;
}

void setup()
{
  // put your setup code here, to run once:

  // DEBUG OVER USB serial
  Serial.begin(9600);

  //delay(7000);
  
  volumeEncoder.Startup();
  menuEncoder.Startup();
  // attaching interrupts to the pins
  attachInterrupt(VOLUME_ENCODER_CW, VolumeCount, CHANGE);
  attachInterrupt(VOLUME_ENCODER_DATA, VolumeCount, CHANGE);
  attachInterrupt(VOLUME_ENCODER_PRESS, VolumePress, FALLING);
  attachInterrupt(MENU_ENCODER_CW, MenuCount, CHANGE);
  attachInterrupt(MENU_ENCODER_DATA, MenuCount, CHANGE);
  attachInterrupt(MENU_ENCODER_PRESS, MenuPress, FALLING);

  // configuring the input pins
  //  // rockers
  pinMode(MENU_UP, INPUT_PULLDOWN);
  pinMode(MENU_DOWN, INPUT_PULLDOWN);
  pinMode(RADIO_NEXT, INPUT_PULLDOWN);
  pinMode(RADIO_BACK, INPUT_PULLDOWN);
  attachInterrupt(MENU_UP, MenuUP, RISING);
  attachInterrupt(MENU_DOWN, MenuDown, RISING);
  attachInterrupt(RADIO_NEXT, RadioNext, RISING);
  attachInterrupt(RADIO_BACK, RadioBack, RISING);
  // // buttons
  pinMode(ECU_RED, INPUT_PULLUP);
  pinMode(ECU_YELLOW, INPUT_PULLUP);
  pinMode(ECU_WHITE, INPUT_PULLUP);
  pinMode(RADIO_BLACK, INPUT_PULLUP);
  pinMode(RADIO_RED, INPUT_PULLUP);
  pinMode(RADIO_BLUE, INPUT_PULLUP);
  pinMode(RADIO_GREEN, INPUT_PULLUP);
  // pinMode(TESTPIN,INPUT_PULLUP);
  attachInterrupt(ECU_RED, EcuRed, FALLING);
  attachInterrupt(ECU_YELLOW, EcuYellow, FALLING);
  attachInterrupt(ECU_WHITE, EcuWhite, FALLING);
  attachInterrupt(RADIO_BLACK, RadioBlack, FALLING);
  attachInterrupt(RADIO_BLUE, RadioBlue, FALLING);
  attachInterrupt(RADIO_RED, RadioRED, FALLING);
  attachInterrupt(RADIO_GREEN, RadioGreen, FALLING);

  // I2C to the light sensor
  Wire1.setSDA(I2C_SDA);
  Wire1.setSCL(I2C_SCL);
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

  //pinMode(TESTPIN,INPUT_PULLUP);
}

void loop()
{
  // put your main code here, to run repeatedly:
  
  /*
  //IO reading test
  if (!digitalRead(TESTPIN))
  {
    Serial.println("HELLO");
  }
  Serial.println("test2");
  delay(1000);
  //


  /*
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
  */

  // Testing writing over SERIAL1
  //Serial.println("testing platformio + encoder");
  //WHEELSERIAL.write("Hi from the wheel!");

  // reading stuff over serial
  messageIn = "\n";
  while (WHEELSERIAL.available())
  {
    messageIn.concat((char)WHEELSERIAL.read());
  }
  if (messageIn.compareTo("\n")) // something has arrived
  {
    Serial.print("Message recieved: ");
    Serial.println(messageIn);
  }

  // reading the encoders
  menuEncoder.Steps();
  val = volumeEncoder.Steps();
  // volumewheel logic
  if (val != oldval) // something happened
  {
    // val increases if turned right, decreases if turned left
    if (val - oldval > 0)
    {
      Serial.println("Volume UP");
      messageOut = RRIGHT;
    }
    else
    {
      Serial.println("Volume DOWN");
      messageOut = RLEFT;
    }

    oldval = val;
  }
  if (volumeEncoder.IsPressed())
  {
    Serial.println("Volume Encoder pressed");
    volumeEncoder.released();
  }
  if (menuEncoder.IsPressed())
  {
    Serial.println("Menu Encoder pressed");
    menuEncoder.released();
    messageOut = RPUSH;
  }

  if (timer.timePassed() > MIN_INPUT_DELAY)
  {
    // write to the pedalbox
    while (messageOut.compareTo("\n"))
    {
      Serial.println("mesageout: " + messageOut);
      messageOut = messageOut + "\n";
      WHEELSERIAL.write(messageOut.c_str());
      messageOut = "\n";
      timer.startTimer();
    }
  }
  else{
    messageOut = "\n";
  }

  

  

  // lcd stuff
  lcd8Digit.ModifyValues(menuEncoder.Steps());
  lcd8Digit.Update();

  delay(DEFAULTDELAY);
  // delay(500);
}