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
      GPIO access through bridging library
  Description: 
      This tutorial shows how to use bridging library calls to access GPIO interface.        
  Hardware Connection:
      Pin 1(P6/ADC1) and 2(P8/ADC3) of CN18 and CN15 connector respectively is connected in loopback fashion and signal driven on Pin1 is read at Pin 2 and verified.  
*/

#include <SPI.h>
#include <CleO.h>

int16_t HandlePin1 = -1; 
int16_t HandlePin2 = -1; 

void setup()
{     
  CleO.begin();
  Serial.begin(115200);
  
  Bridge::gpio_options goptions;
  goptions.PinMap = Z_GPIO_PINMAP(CLICK2_AN);
  goptions.Direction = Z_GPIO_DIRECTION(CLICK2_AN, Bridge::Output);
  goptions.PullUpCfg = Z_GPIO_PULL_UP_CFG(CLICK2_AN, Bridge::pad_pull_none);
  /* Open GPIO device using bridging commands */
  HandlePin1 = CleO.DeviceOpen(0, BRIDGE_TYPE_GPIO, sizeof(goptions), (uint8_t*)&goptions);
  if(HandlePin1 < 0)
  {
    /* Failed to open a handle */
    Serial.print("DeviceOpen on Pin1 failed");
  }
  else
  {        
    goptions.PinMap = Z_GPIO_PINMAP(CLICK1_AN);
    goptions.Direction = Z_GPIO_DIRECTION(CLICK1_AN, Bridge::Input);
    goptions.PullUpCfg = Z_GPIO_PULL_UP_CFG(CLICK1_AN , Bridge::pad_pull_none);
    /* Open GPIO device using bridging commands */
    HandlePin2 = CleO.DeviceOpen(0, BRIDGE_TYPE_GPIO, sizeof(goptions), (uint8_t*)&goptions);
    if(HandlePin2 < 0)
    {
      /* Failed to open a handle */
      Serial.print("DeviceOpen on Pin2 failed");
    }
    else
    {
      uint32_t Pin1Val, Pin2Val = 0;
      int16_t BytesWritten;    
      int LoopCount = 4;
      uint32_t WriteValue = 0x1; 			
       
      while( LoopCount > 0)
      { 
        Pin1Val = (WriteValue << CLICK2_AN);   
        /* Write to Pin1 using bridging commands */
        CleO.DeviceWrite(HandlePin1, sizeof(Pin1Val), (uint8_t *)&Pin1Val, BytesWritten);                
        delay(1000);
        /* Read from Pin2 using bridging command */            
        CleO.DeviceRead(HandlePin2, 0, 4, NULL, (uint8_t*)&Pin2Val);                        
        if(Z_BitIsSet(Pin2Val, (MASK_ONE_U32 << CLICK1_AN)) == WriteValue)                
          Serial.println("Successfully Read the value written!");
        else
          Serial.println("Failed to read the value written!");
        
        LoopCount --;
        WriteValue = !WriteValue;
        delay(1000);
      }
    }
  }
}

void loop()
{
}