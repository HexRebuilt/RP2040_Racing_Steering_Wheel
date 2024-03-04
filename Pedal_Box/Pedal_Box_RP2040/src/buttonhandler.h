class ButtonHandler
{
    private:
        unsigned int buttonID = 0, potOut = 0, counter = 0;
    public:
        short convertIDtoPot(String message){
            potOut = 0;
            buttonID = message.toInt();
            if (buttonID >= 0 && buttonID <= 9) //max radiout value
            {
                potOut = map (buttonID, 0, 9, 0, 128);
                counter++;
                Serial.print("counter: ");
                Serial.println(counter);
            }
            return potOut;
        }

};