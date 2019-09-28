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
      UART device access through bridging library
  Description: 
      This tutorial shows how to use bridging library calls to access UART device interface.
      CleOWifi module (which uses ESP8266MOD) is used as UART slave device fot this tutorial functioning.
  External component:   
      CleOWifi module
  REFERENCES:
      1. ESP8266 Developer Resources - https://www.itead.cc/wiki/ESP8266_Serial_WIFI_Module        
  Hardware Connection:
      Connect CleOWifi module to CleO35 CN5 connector
*/

#include <SPI.h>
#include <CleO.h>

#define ESP_READ_TIMEOUT  ((uint16_t)1000u)
char GMR[] = "AT+GMR\r\n";

int16_t HandleUart = -1; 
int16_t HandlePwdPin = -1;
int16_t BytesWritten;
char Buffer[150]; 

void setup()
{     
  CleO.begin();
  Serial.begin(115200);
  
  Bridge::gpio_options goptions;
  goptions.PinMap = Z_GPIO_PINMAP(CLEO35_CN5_2);
  goptions.Direction = Z_GPIO_DIRECTION(CLEO35_CN5_2, Bridge::Output);
  goptions.PullUpCfg = Z_GPIO_PULL_UP_CFG(CLEO35_CN5_2, Bridge::pad_pull_none);
  /* Open GPIO device using bridging commands 
     Enable PWD pin of ESP8266
   */
  HandlePwdPin = CleO.DeviceOpen(0, BRIDGE_TYPE_GPIO, sizeof(goptions), (uint8_t*)&goptions);
  if(HandlePwdPin < 0)
  {
    /* Failed to open a handle */
    Serial.print("DeviceOpen failed");
  }
  else
  {
    uint32_t GpioVal;
    GpioVal = ((uint32_t)0x01 << CLEO35_CN5_2);   
    /* Write to GPIO device using bridging commands */
    CleO.DeviceWrite(HandlePwdPin, sizeof(GpioVal), (uint8_t *)&GpioVal, BytesWritten); 
  }
  
  Bridge::uart_options uoptions;    
  uoptions.BaudRate = Bridge::b115200;
  uoptions.BufferSize = UART_DEFAULT_BUFF_SIZE ;
  uoptions.RxTimeout = ESP_READ_TIMEOUT;
  uoptions.PortID = Bridge::CleO35_CN5;
  /* Open UART device using bridging commands */
  HandleUart = CleO.DeviceOpen(0, BRIDGE_TYPE_UART, sizeof(uoptions), (uint8_t*)&uoptions);  
  if(HandleUart < 0 ) 
  {
    /* Failed to open a handle */
    Serial.print("DeviceOpen failed");
  }        
  else
  {   
    uint16_t rlen = 1;
    /* OPT_BLOCKING enables continuous read from UART until timeout occurs */
    uint8_t ropt = OPT_BLOCKING;
    while(rlen)        
      rlen = CleO.DeviceRead(HandleUart, sizeof(ropt), sizeof(Buffer)-1, &ropt, (uint8_t*)Buffer);                          
      
    /* Write to UART device using bridging command */
    CleO.DeviceWrite(HandleUart, strlen(GMR), (uint8_t *)GMR, BytesWritten);
    unsigned long start = millis();
    bool IsResponseOK = false;
    rlen = 0;          
    ropt = OPT_NONBLOCKING; /* OPT_NONBLOCKING enables reading available bytes from UART without blocking */
      
    while ( ((millis() - start) & ~0UL) < 10000)
    {          
      /* Read from UART device using bridging command */
      rlen += CleO.DeviceRead(HandleUart, sizeof(ropt), 0, &ropt, (uint8_t*)Buffer+rlen);                  
      if( strstr(Buffer, "OK"))
      {               
        Serial.println(Buffer); 
        IsResponseOK = true;                
        break;
      }       
    }       
    if(!IsResponseOK)
      Serial.print("Version Command Response NOT OK ");
        
    /* Close UART device using bridging command 
     */    
    HandleUart = CleO.DeviceClose(HandleUart);
    
    /* Close GPIO device using bridging command 
     */    
    HandlePwdPin = CleO.DeviceClose(HandlePwdPin);
  }
}

void loop()
{
}