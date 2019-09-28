/* (C) Copyright Bridgetek Pte Ltd.
* ============================================================================
*
* This source code ("the Software") is provided by Bridgetek Pte Ltd
*  ("Bridgetek") subject to the licence terms set out
* http://brtchip.com/BRTSourceCodeLicenseAgreement/ ("the Licence Terms").
* You must read the Licence Terms before downloading or using the Software.
* By installing or using the Software you agree to the Licence Terms. If you
* do not agree to the Licence Terms then do not download or use the Software.
*/


/*
  Tutorial Name: 
      I2C device access through bridging library
  Description: 
      This tutorial shows how to use bridging library calls to access I2C device interface.            
*/

#include <SPI.h>
#include <CleO.h>

int16_t HandleI2C = -1;

void setup()
{       
  Bridge::i2cm_options i2c_open_opt;
  byte Reg = 0x07; /* Register address*/
  byte Data;
  byte Len = 1;
  
  CleO.begin();
  Serial.begin(115200);

  i2c_open_opt.SlaveAddress = 0x57;
  i2c_open_opt.ClockSpeed = 400000;
  /* Open I2C device using bridging commands */
  HandleI2C = CleO.DeviceOpen(0, BRIDGE_TYPE_I2CM, sizeof(i2c_open_opt), (uint8_t*)&i2c_open_opt);    
  if(HandleI2C < 0)
  {
    /* Failed to open a handle */
    Serial.println("DeviceOpen failed");  
  }
  else
  {           
    /* Read I2C device using bridging commands.
     * DeviceRead returs number of bytes read.
     */
    int16_t RetCode = CleO.DeviceRead(HandleI2C, 1, Len, &Reg, &Data);
    Serial.print("Read I2C : ");Serial.print(RetCode,DEC);Serial.println(" Bytes");
    
    /* Close I2C device using bridging commands.     
     */
    HandleI2C = CleO.DeviceClose(HandleI2C);
    if(HandleI2C < 0)
        Serial.println("DeviceClose failed");
  }
}

void loop()
{       
}

