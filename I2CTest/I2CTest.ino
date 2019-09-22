/**
    I2C Test ----------------
    Alex.W - September - 2019

    This is a little sideproject, please don't blame me if it doesn't work properly.
    ------------------------
*/

#include <Wire.h>

void setup()
{
  Serial.begin(19200);
Wire.begin(2); // Start I2C Bus as Master
Wire.onRequest(requestEvent);
}

void loop()
{

delay(500);
Serial.println("Looped");

}

void requestEvent() {
  Serial.println("Sent response");
  Wire.write("yo"); // respond with message of 6 bytes
  // as expected by master
}
