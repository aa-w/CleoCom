/**
    I2C Test ----------------
    Alex.W - September - 2019

    This is a little sideproject, please don't blame me if it doesn't work properly.
    ------------------------
*/

#include <Wire.h>

int RandomAddress = 0;

void setup()
{
  RandomAddress = random(1, 127);
  int add = 90;
  Serial.begin(19200);
  Wire.begin(add); // Start I2C Bus as Master

  Serial.println("Address");
  Serial.println(add);
  Wire.onRequest(requestEvent);
}

void loop()
{
  delay(500);
}

void requestEvent() {
  Serial.println("Sent response");
  char Convert [12];
  itoa (millis(), Convert, 10);
  
  Wire.write(Convert); // respond with message of 6 bytes
  // as expected by master
}
