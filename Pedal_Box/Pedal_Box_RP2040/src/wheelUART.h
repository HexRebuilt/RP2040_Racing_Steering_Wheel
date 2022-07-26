#include <Arduino.h>
#include <string.h>

class WheelUART
{
private:
    /* data */
    String message = "\n";

public:
    WheelUART(/* args */);
    ~WheelUART();

    void Startup(int baud)
    {
        // starting the serial communication to the wheel on UART0
        WHEELSERIAL.begin(WHEELBAUD);
        Serial.println("Communication to the wheel started...");
    }

    String Read()
    {
        message = "\n";
        while (WHEELSERIAL.available())
        {
            message.concat((char)WHEELSERIAL.read());
        }
        return message;
    }

    void Write(String tobesent)
    {
        WHEELSERIAL.write( (u_int16_t) tobesent);
    }
};

WheelUART::WheelUART(/* args */)
{
}

WheelUART::~WheelUART()
{
}
