#include <Adafruit_NeoPixel.h>

class LedBar : public HumanInterface
{
private:
    /* data */
    int ledbrightness = MAX_BRIGHT_LEDS; //initial level
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_STRYPE_PIN, NEO_GRB);
    uint32_t leds[NUM_LEDS];
    uint8_t offled = NUM_LEDS;
    int rpmDC = 0;

public:
    LedBar(/* args */);

    void Initialize()
    {
        strip.begin();
        strip.show(); // Initialize all pixels to 'off'
        strip.setBrightness(MAX_BRIGHT_LEDS);
        for (int i = 0; i < NUM_LEDS; i++)
        {
            SetRPMDC((i + 1) * 10 + 5);
            Update();
            strip.show();  // ← ADDED: Push color changes to LEDs
            delay(50);
        }

        Serial.println("LED configuration DONE");
    }

    void SetBrightness(short value)
    {
        /*
        if (currentValue == value)
        {
            return; //no action needed
        }

        if (currentValue < value) //need to increase brightness
        {
            delta = value - currentValue;
            //delta = delta / (float) SENSITIVITY;
            ledbrightness += delta * LED_BRIGHT_STEP;
        }
        else
        {
            delta = currentValue - value;
            //delta = delta / (float) SENSITIVITY;
            ledbrightness -= delta * LED_BRIGHT_STEP;
        }
        currentValue = value;

        //keeping the LED brightness in range
        if (ledbrightness > MAX_BRIGHT_LEDS)
        {
            ledbrightness = MAX_BRIGHT_LEDS;
        }
        if (ledbrightness < MIN_BRIGHT_LEDS)
        {
            ledbrightness = MIN_BRIGHT_LEDS;
        }*/
        ledbrightness = map(value,MIN_BRIGHT_LEDS, MAX_BRIGHT_LEDS, 10, 100);
        //changing the brightness
        strip.setBrightness(ledbrightness);
        
    }

    /**
     * Function that sets the color and state of the led depending on the RPM.
     * */
    void Update()
    {
        //Serial.println(rpmDC);
        if (rpmDC == 0 || rpmDC > 100 || rpmDC < 0) //if i am outside my scope i exit
        {
            for (int i = 0; i < NUM_LEDS; i++)
            {
                strip.setPixelColor(i, 0);
                delay(LED_DELAY);
            }
            strip.show();
            return;
        }

        //shiftlight needs to happen asap
        if (rpmDC >= SHIFTLIGHT_RPM)
        {
            //Serial.println("SHIFTLIGHT");
            for (int i = 0; i < NUM_LEDS; i++)
            {
                strip.setPixelColor(i, strip.Color(0, 0, 255));
                delay(LED_DELAY);
            }
            strip.show();
            delay(100);
            for (int i = 0; i < NUM_LEDS; i++)
            {
                strip.setPixelColor(i, 0);
                delay(LED_DELAY);
            }
            strip.show();
            return;
        }

        offled = (uint8_t)map(rpmDC, 0, SHIFTLIGHT_RPM, NUM_LEDS, 0);
        //Serial.println(offled);
        for (int i = 0; i < offled; i++)
        {
            strip.setPixelColor(i, 0);
            delay(LED_DELAY);
        }
        for (int i = offled; i < NUM_LEDS; i++) //the leds are mounted upside down
        {
            //those are the leds on
            if (i > GREEN_LED_INDEX)
            {
                strip.setPixelColor(i, strip.Color(0, 255, 0)); // Green
                delay(LED_DELAY);
                continue;
            }
            if (i <= GREEN_LED_INDEX && i > YELLOW_LED_INDEX)
            {
                strip.setPixelColor(i, strip.Color(255, 150, 0)); // Yellow
                delay(LED_DELAY);
                continue;
            }
            else
            {
                strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
                delay(LED_DELAY);
                continue;
            }
        }
        strip.show();
    }

    void SetRPMDC(int newRPM_DC)
    {
        rpmDC = newRPM_DC;
    }
};

LedBar::LedBar(/* args */)
{
}