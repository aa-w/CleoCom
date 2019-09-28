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
      PWM access through bridging library
  Description: 
      This tutorial shows how to use bridging library calls to access PWM interface.        
*/

#include <SPI.h>
#include <CleO.h>

int16_t HandlePWM = -1; 

void setup()
{     
  CleO.begin();
  Serial.begin(115200);
  
  Bridge::pwm_options PwmOpts;
  
  /* set the system clock frequency which is same for all channels */
  PwmOpts.Frequency = 1000;
  
  /* Value 0 is for continuous generation
   * 1 for one-shot, 2 to 255 are for so many shots
   */
  PwmOpts.Shots = 0;
  
  /* 2 PWM channels are available */
  PwmOpts.Channels = CLICK2_PWM_CHANNEL | CLICK1_PWM_CHANNEL;
  
  /* Set duty cycle for PWM1 (CLICK1_PWM_CHANNEL) */
  PwmOpts.DutyCycle1 = 25;
  
  /* Set duty cycle for PWM2 (CLICK2_PWM_CHANNEL) */
  PwmOpts.DutyCycle2 = 50;
  
  /* InitState1 and 2 for PWM1 and PWM2 channels respectively , 0= low, 1= high */
  PwmOpts.InitState1 = 1;
  PwmOpts.InitState2 = 0;
  
  HandlePWM = CleO.DeviceOpen(0, BRIDGE_TYPE_PWM, sizeof(PwmOpts), (uint8_t*)&PwmOpts);
  if(HandlePWM < 0)
  {
    /* Failed to open a handle */
    Serial.print("DeviceOpen failed");
  }    
      
  delay(100000);
  CleO.DeviceClose(HandlePWM);
}

void loop()
{
}