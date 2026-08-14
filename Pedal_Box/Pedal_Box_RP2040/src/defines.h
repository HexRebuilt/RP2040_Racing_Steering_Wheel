//List of all the defines used by the system

//GPS serial defines for UART1
#define GPSBAUD 9600
#define GPSTX   p8 //blue
#define GPSRX   p9 //white

//Wheel serial defines on UART0
#define WHEELTX     p0
#define WHEELRX     p1
#define WHEELBAUD   9600
#define WHEELSERIAL Serial1

#define RPMDCPIN    p15

//radio potentiometer
/* SPI PINS are the pins used for SPI 
(Serial Peripheral Interface) on the Nano RP2040 Connect are the following:
    (CIPO) - D12 (old MISO)
    (COPI) - D11 (old MOSI)
    (SCK)  - D13
    (CS)   - Any GPIO (except for A6/A7)
Source: https://docs.arduino.cc/tutorials/nano-rp2040-connect/rp2040-01-technical-reference
*/
#define RADIO_MPC4131_ADDRESS       0x00
#define POT_MIN                     0
#define POT_MAX                     127
#define RADIO_MPC4131_CS            p2 //is gp25
#define CIPO                        p12
#define COPI                        p11
#define SCK                         p10
#define ANALOG_OUTPUT_TIME          20  //ms 
#define ANALOG_OUTPUT_CONFIG_TIME   5000 //ms

#include "buttonIds.h"

#define DEFAULTDELAY    10
// Low-beam state check
#define LOW_BEAM_PIN    PIN_A1 //p15 //GP27
#define LOW_BEAM_UPDATE_FREQUENCY 500

#define AUTOMATIC_VOLUME_MIN_SPEED      50 //kph or whatever savage measure unit you use
#define AUTOMATIC_SPEED_DELTA_INCREASE  5 //kph