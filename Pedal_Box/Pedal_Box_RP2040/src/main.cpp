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
Timer timer, speed_timer, lowBeamTimer;
//serail uart message to the wheel
String message;
int lastSpeedVolumeChange = 0;

boolean lowBeamON = false, configurationStatus = false;

void cyclePins() {
  // Simple pin cycling: each 10s, set one pin HIGH, print which
  // Rotates D0-D29, all pins cycled in order
  uint8_t currentPin = 1;
 
  pinMode(p0,INPUT_PULLDOWN);
  
  // Change pin every 10000ms (10s)
  for (currentPin = 1; currentPin < 30u; currentPin++) {
    pinMode(currentPin,OUTPUT);
    digitalWrite(currentPin, LOW);
  }
  // Set current pin HIGH
  for(currentPin=1; currentPin < 30u; currentPin++){
    digitalWrite(currentPin, HIGH);
    // Print which pin is ON
    // Map Arduino D# to user p#: see defines.h mapping
    // D0=p0, D1=p1, D2=p2, D10=p5 (tested), D12=p12, D15=p15, D29=p29=LOW_BEAM_PIN
    const char* pinNames[30] = {
      "D0 (p0) = GP0",                /* D0  – GP0   */
      "D1 (p1) = GP1",                /* D1  – GP1   */
      "D2 (p#) = GP#",                /* D2  – placeholder */
      "D3 (p15) = GP15",              /* D3  – GP15  */
      "D4 (unassigned)",              /* D4  – *Not assigned */ 
      "D5 (p17) = GP17",              /* D5  – GP17  */
      "D6 (p18) = GP18",              /* D6  – GP18  */
      "D7 (p19) = GP19",              /* D7  – GP19  */
      "D8 (p20) = GP20",              /* D8  – GP20  */
      "D9 (p21) = GP21",              /* D9  – GP21  */
      "D10 (p5) = GP4",               /* D10 – GP4   (p5 naming) */
      "D11 (p7) = GP7",               /* D11 – GP7   */
      "D12 (p4) = GP4",               /* D12 – GP4   (p4 naming) */
      "D13 (p6) = GP6",               /* D13 – GP6   */
      "D14 (p26) = GP26",             /* D14 – GP26  */
      "D15 (p27) = GP27",             /* D15 – GP27  */
      "D16 (p28) = GP28",             /* D16 – GP28  */
      "D17 (p29) = GP29",             /* D17 – GP29  */
      "D18 (p12) = GP12",             /* D18 – GP12  */
      "D19 (p13) = GP13",             /* D19 – GP13  */
      "D20 (p2) * = GP2",             /* D20 – shared with D2  (asterisk marks shared) */
      "D21 (p#) = GP#",               /* D21 – placeholder */
      "D22 (unassigned)",             /* D22 – *Not assigned */ 
      "D23 (unassigned)",             /* D23 – *Not assigned */ 
      "D24 (p3) = GP3",               /* D24 – GP3   */
      "D25 (p8) = GP8",               /* D25 – GP8   */
      "D26 (p9) = GP9",               /* D26 – GP9   */
      "D27 (p10) = GP10",             /* D27 – GP10  */
      "D28 (p11) = GP11",             /* D28 – GP11  */
      "D29 (p14) = GP14"              /* D29 – GP14  */
    };
    Serial.print("Test Pin ON= ");
    Serial.println(pinNames[currentPin]);
    //Serial.print("\t pName=p");
    //Serial.println(currentPin);
    delay(100);
    if(digitalRead(p0)){
      digitalWrite(currentPin, LOW);
      Serial.print(pinNames[currentPin]);
      Serial.println(" OFF");
      delay(100);
      if(!digitalRead(p0)){
        Serial.println("Pin found! Please move the probe elsewhere");
        digitalWrite(currentPin, LOW);
        delay(1000);
      }
    }
    //digitalWrite(currentPin, LOW);
    //Serial.print(pinNames[currentPin]);
    //Serial.println(" OFF");
    delay(100);
  } 
}

void checkVolumeBySpeed(int currentSpeed) {
  if (currentSpeed >= AUTOMATIC_VOLUME_MIN_SPEED && (int)abs(currentSpeed - lastSpeedVolumeChange) > AUTOMATIC_SPEED_DELTA_INCREASE ){
    if(currentSpeed>lastSpeedVolumeChange) {//'im going faster
      digitalPotWrite(buttonHandler.convertIDtoPot((String) RRIGHT));
    }
    else{
      digitalPotWrite(buttonHandler.convertIDtoPot((String)RLEFT));
    }
    lastSpeedVolumeChange = currentSpeed;
  }
}

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

  //low beam check
  pinMode(LOW_BEAM_PIN, INPUT_PULLDOWN);
  //pinMode(LOW_BEAM_PIN, INPUT);
  
  //mapping the fucking pins
  /*
  while(1){
    cyclePins();
    delay(100);
  }*/
 /*
  bool out=0;
  pinMode(LOW_BEAM_PIN ,OUTPUT);
  while(1){
    digitalWrite(LOW_BEAM_PIN,out);
    Serial.println(out);
    out=!out;
    delay(1500);
  }
  */
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
    if (message.startsWith("CONFIGURATION"))
    {
      configurationStatus = !configurationStatus;
    }
    //analyzing the message
    digitalPotWrite (buttonHandler.convertIDtoPot(message));
    timer.startTimer();
    message = "\n";
  }
    
  
  //reset pot value after set ammount of time
  if (timer.timePassed() > ANALOG_OUTPUT_CONFIG_TIME && configurationStatus == false)
  {
    digitalPotWrite(0);
  }
  //config mode
  if (timer.timePassed() > ANALOG_OUTPUT_CONFIG_TIME && configurationStatus == true)
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

  //automatic volume control
  checkVolumeBySpeed(gps.speed.kmph());

  //check the lowbeam status
  if(lowBeamTimer.timePassed()>= LOW_BEAM_UPDATE_FREQUENCY){
    lowBeamON = !digitalRead(LOW_BEAM_PIN); //on high side of the optocoupler
    /*
    Serial.print("LOW_BEAM_LINE: ");
    Serial.print(lowBeamON);
    Serial.print("\n");
    
    pinMode(LOW_BEAM_PIN, INPUT);
    int readvalue = analogRead(LOW_BEAM_PIN);
    Serial.println(readvalue);
    */
    if(lowBeamON){
      Serial.print("Low beams ON\n");
      WHEELSERIAL.write("LowBeams ON\n");
      //digitalWrite(LOW_BEAM_PIN,lowBeamON);
    }
    else{
      Serial.print("Low beams OFF\n");
      WHEELSERIAL.write("LowBeams OFF\n");
      //digitalWrite(LOW_BEAM_PIN,lowBeamON);
    }
    lowBeamTimer.startTimer();
    //lowBeamON=!lowBeamON;

  }
  
  
  delay(DEFAULTDELAY);
}