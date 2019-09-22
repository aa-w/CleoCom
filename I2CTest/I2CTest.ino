/**
    I2C Test ----------------
    Alex.W - September - 2019

    This is a little sideproject, please don't blame me if it doesn't work properly.
    ------------------------
*/

#include <Wire.h>

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin(8);
  Serial.begin(19200);
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);

}

void requestEvent()
{
  Wire.write("I");
  Serial.println("Data Sent");
}
