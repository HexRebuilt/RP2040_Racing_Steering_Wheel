//Wheel serial defines on UART0
#define WHEELTX     p0
#define WHEELRX     p1
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
#define ONBOARD_LED_PIN     p16
#define ONBOARD_LED_TYPE    WS2812
#define ONBOARD_LED_COLOR   RGB
#define LED_STRYPE_PIN      p7
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
//#define RPMDCPIN               //D19 

//LCD spi related constants
#define LCD_DIN                 p11
#define LCD_CLK                 p10
#define LCD_CS                  p9
#define DIGIT_UPDATE_MS         5
#define CIPO_DO_NOT_USE                    p12
#define COPI                    p11
#define SCK                     p10

//I2C variables for light sensors:
#define PIN_WIRE_SDA p2
#define PIN_WIRE_SCL p3
#define I2C_SDA p2
#define I2C_SCL p3

//Encoder pins
#define VOLUME_ENCODER_CW    p4
#define VOLUME_ENCODER_DATA  p5
#define VOLUME_ENCODER_PRESS p6
#define MENU_ENCODER_CW      p13
#define MENU_ENCODER_DATA    p14
#define MENU_ENCODER_PRESS   p15

//Button pins
#define RADIO_RED     p8
#define RADIO_BLUE    p12
#define RADIO_GREEN   p17
#define RADIO_BLACK   p18
#define RADIO_BACK    p23
#define RADIO_NEXT    p24
//ecu ones
#define ECU_RED       p19
#define ECU_YELLOW    p20
#define ECU_BLACK     p21
#define ECU_MENU_UP   p22


#define DEFAULTDELAY    10
