//Wheel serial defines on UART0
#define WHEELTX     (0u)
#define WHEELRX     (1u)
#define WHEELBAUD   9600
#define WHEELSERIAL Serial1

//steering wheel outputs
#define MAX_BRIGHT_LEDS 50 //due to power limitations
#define MAX_BRIGHT_LCD  15
#define MIN_BRIGHT_LCD  1 //to be modified based on needs
#define MIN_BRIGHT_LEDS 1 //to be modified based on needs
#define SENSITIVITY     2
#define LED_BRIGHT_STEP 1 // 255/MAXBRIGHTLCD=17 steps
#define MILLISECONDS_PER_MENU 3000

//NeoPixel led bar
#define ONBOARD_LED_PIN     D16
#define ONBOARD_LED_TYPE    WS2812B
#define ONBOARD_LED_COLOR   RGB
#define LED_STRYPE_PIN      D6
#define NUM_LEDS            8
#define LED_STRYPE_TYPE     WS2812B
#define LED_STRYPE_COLOR    GRB
#define LED_DELAY           5//ms
#define GREEN_LED_INDEX     4
#define YELLOW_LED_INDEX    1
#define RED_LED_INDEX       0
#define MIN_RPM_LIGHT       2500 //under this rpm the leds will stay off
#define SHIFTLIGHT_RPM      6000
#define REDLINE_DUTY_CYCLE  75 //to be tested
#define MINRPM              0 
#define MAXRPM              7000
//#define RPMDCPIN            D15 //see the pedalbox

//LCD spi related constants
#define LCD_DIN                 D11
#define LCD_CLK                 D10
#define LCD_CS                  D9
#define DIGIT_UPDATE_MS         5
#define CIPO_DO_NOT_USE         D12
#define COPI                    D11
#define SCK                     D10

//I2C variables for light sensors:
#define PIN_WIRE_SDA D2
#define PIN_WIRE_SCL D3
#define I2C_SDA D2
#define I2C_SCL D3

//Encoder pins
#define VOLUME_ENCODER_CW    D4
#define VOLUME_ENCODER_DATA  D5
#define VOLUME_ENCODER_PRESS D7
#define MENU_ENCODER_CW      D13
#define MENU_ENCODER_DATA    D15
#define MENU_ENCODER_PRESS   D14

//Button pins
#include "button_and_encoders/buttonIds.h"
//#define TESTPIN       D23
#define RADIO_RED     D23 
#define RADIO_BLUE    D22 
#define RADIO_GREEN   D25 
#define RADIO_BLACK   D8 
#define RADIO_BACK    D26
#define RADIO_NEXT    D27
#define RADIO_NUM     9
//ecu ones
#define ECU_RED       D18 //TESTED
#define ECU_YELLOW    D19 //TESTED
#define ECU_WHITE     D17 //TESTED
#define ECU_NUM       3
//menu ones
#define MENU_UP       D28
#define MENU_DOWN     D29
#define WHEEL_NUM     5

//for the maximum allowed buttons in an handler
#define MAX_BUTTONS     10
#define DEFAULTDELAY    10
