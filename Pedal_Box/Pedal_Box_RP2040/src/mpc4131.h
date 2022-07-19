void digitalPotWrite(unsigned int value)
{
    Serial.print("POT VALUE: ");
    Serial.println(value);
    digitalWrite(RADIO_MPC4131_CS, LOW);
    mySPI.transfer(RADIO_MPC4131_ADDRESS);
    mySPI.transfer(value);
    digitalWrite(RADIO_MPC4131_CS, HIGH);
}

void TestPOT()
{
    for (int i = 0; i <= 128; i++)
    {
        digitalPotWrite(i);
        delay(500);
    }
    delay(5000);
    for (int i = 128; i >= 0; i--)
    {
        digitalPotWrite(i);
        delay(500);
    }
}