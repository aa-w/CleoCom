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
      SPI device access through bridging library
  Description: 
      This tutorial shows how to use bridging library calls to access SPI device interface.            
*/

#include <SPI.h>
#include <CleO.h>

int16_t HandleSPI = -1;

void setup()
{
  Bridge::spim_options SpiOptions;    
  int16_t BytesWritten;
  uint8_t WriteBuf = 0x33;
  
  CleO.begin();
  Serial.begin(115200);

  SpiOptions.SlaveSelectPin = GPIO_SPIM_SS3;
  SpiOptions.ClockDivider = 0;
  SpiOptions.BusWidth = 1;
  SpiOptions.Mode = Bridge::spi_clock_mode_t::spi_mode_0;
  SpiOptions.Speed = 10000000; 
  /* Open SPI device using bridging commands */    
  HandleSPI = CleO.DeviceOpen(0, BRIDGE_TYPE_SPI, sizeof(SpiOptions), (uint8_t *)&SpiOptions );        
  if(HandleSPI < 0 )
  {   
    /* Failed to open a handle */
    Serial.print("DeviceOpen failed"); 
  }    
  else
  {        
    /* Write SPI device using bridging commands.
     * DeviceWrite responds 0 for failure and 1 for success
     */
    int16_t RetCode = CleO.DeviceWrite(HandleSPI,1,&WriteBuf,BytesWritten);  
    if(RetCode < 0)
        Serial.println("Write to SPI failed");   
    else
        Serial.println("Write to SPI is successful");

    /* Close SPI device using bridging commands.     
     */
    HandleSPI = CleO.DeviceClose(HandleSPI);
    if(HandleSPI < 0 )
      Serial.println("DeviceClose failed");
  }            
}

void loop()
{    
}