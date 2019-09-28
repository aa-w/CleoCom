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
      ADC access through bridging library
  Description: 
      This tutorial shows how to use bridging library calls to access ADC interface.        
  External component:   
      IR Distance click module
  REFERENCES:
      1. IR Distance click - https://shop.mikroe.com/click/sensors/ir-distance                
  Hardware Connection:
      Connect IR Distance click module to CleO50 click 2 header interface.
      When run the tutorial,the device outputs the voltage corresponding to the detected distance.        
      The voltage output is inversely proportional to the distance, this means that when the distance is growing the output is decreasing.
*/

#include <SPI.h>
#include <CleO.h>

int16_t HandleADC = -1; 

void setup()
{     
  CleO.begin();
  Serial.begin(115200);

  Bridge::adc_options adc_opts;
  adc_opts.Mode = Bridge::adc_mode_continuous;
  adc_opts.Channel = CLICK2_ADC_CHANNEL;

  /* Open ADC device using bridging commands */
  HandleADC = CleO.DeviceOpen(0, BRIDGE_TYPE_ADC, sizeof(adc_opts), (uint8_t*)&adc_opts);
  if(HandleADC < 0)
  {
    /* Failed to open a handle */
    Serial.print("DeviceOpen failed");
  }    
}

void loop()
{
  uint16_t AdcReadVal;
  if(HandleADC >= 0)
  {
    /* Read from ADC channel using bridging command.
       CleO ADC supports 10-bit resolution output
     */            
    CleO.DeviceRead(HandleADC, 0,2 , NULL, (uint8_t *)&AdcReadVal);                                  
    Serial.println(AdcReadVal); 
  }
} 