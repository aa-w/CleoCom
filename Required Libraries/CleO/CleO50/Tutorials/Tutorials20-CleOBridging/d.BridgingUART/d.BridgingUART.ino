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
      Wifi3 click module (from MikroE) is used as UART slave device fot this tutorial functioning.
  External component:   
      Wifi3 click module
  REFERENCES:
      1. ESP8266 Developer Resources - https://www.itead.cc/wiki/ESP8266_Serial_WIFI_Module        
      2. WiFi3 click module - http://www.mikroe.com/click/wifi3/
  Hardware Connection:
      Connect Wifi3 module to CleO50 click 2 header interface
*/

#include <SPI.h>
#include <CleO.h>

#define ESP_READ_TIMEOUT  ((uint16_t)1000u)
char GMR[] = "AT+GMR\r\n";

int16_t HandleUart = -1; 
int16_t BytesWritten;
char Buffer[150]; 

void setup()
{     
  CleO.begin();
  Serial.begin(115200);
  
  Bridge::uart_options uoptions;    
  uoptions.BaudRate = Bridge::b115200;
  uoptions.BufferSize = UART_DEFAULT_BUFF_SIZE ;
  uoptions.RxTimeout = ESP_READ_TIMEOUT;
  uoptions.PortID = Bridge::CleO50_click_2;
  /* Open UART device using bridging commands */
  HandleUart = CleO.DeviceOpen(0, BRIDGE_TYPE_UART, sizeof(uoptions), (uint8_t*)&uoptions);  
  if(HandleUart < 0 ) 
  {
    /* Failed to open a handle */
    Serial.print("DeviceOpen failed");
  }        
  else
  {   
    /* Write to UART device using bridging command */    
    CleO.DeviceWrite(HandleUart, strlen(GMR), (uint8_t *)GMR, BytesWritten);
    unsigned long start = millis();
    uint16_t rlen = 0;          
    uint8_t ropt = OPT_NONBLOCKING; /* OPT_NONBLOCKING enables reading available bytes from UART without blocking */
    bool IsResponseOK = false;
    
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
  }
}

void loop()
{
}