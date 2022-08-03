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
#define LED_PIN             p7
#define NUM_LEDS            8
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB
#define LED_DELAY           5//ms
#define GREEN_LED_INDEX     4
#define YELLOW_LED_INDEX    1
#define RED_LED_INDEX       0
#define MIN_RPM_LIGHT       2500 //under this rpm the leds will stay off
#define SHIFTLIGHT_RPM      6000
#define REDLINE_DUTY_CYCLE  75 //to be tested
#define MINRPM              0 
#define MAXRPM              7000
#define RPMDCPIN               //D19 

//LCD spi related constants
#define LCD_DIN                 p11
#define LCD_CLK                 p10
#define LCD_CS                  p9
#define DIGIT_UPDATE_MS         5
#define CIPO                        p12
#define COPI                        p11
#define SCK                         p10

//I2C variables for light sensors:
#define I2C_SDA p2
#define I2C_SCL p3

#define DEFAULTDELAY    10