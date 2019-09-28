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
      MikroE 8X8 LED Click module is used as SPI slave device fot this tutorial functioning.
  External component:   
      MikroE 8X8 LED Click module
  REFERENCES:
      1. MikroE 8x8 Click - http://www.mikroe.com/click/8x8-r/
      2. WiFi3 click module - http://www.mikroe.com/click/wifi3/
  Hardware Connection:
      LED Click         Nero
      5V                5V
      GND               GND
      SDI               ~11
      SDO               12
      SCK               13
      CS                8
*/

#include <SPI.h>
#include <CleO.h>

/* MAX7219 based macros for configuration and registers */
#define DECODE_MODE         9
#define INTENSITY           0xA
#define SCAN_LIMIT          0xB
#define SHUTDOWN            0xC
#define DISPLAY_TEST        0xF

#define SHDN_NORMAL_OPERATION   1
#define SHDN_SHUTDOWN           0
#define NO_DECODE_MODE          0
#define INTENSITY_MAX           0x0F
#define INTENSITY_MID           0x04
#define INTENSITY_LOW           0x01
#define SCAN_ALL                0x7 /* enable all 8 digit lines, segments are controlled by the
                                       values in the digit register */
#define NORMAL_MODE             0x00

int16_t HandleSPI = -1; 
int16_t BytesWritten;

void setup()
{
  CleO.begin();
  Serial.begin(115200);
 
  /* Open SPI device using bridging commands */
  /* LED8x8 Click is connected to Click 1 and SPI configuration is Mode0, Single Channel, 10MHz */
  Bridge::spim_options SpiOptions;    
  SpiOptions.SlaveSelectPin = GPIO_SPIM_SS1;
  SpiOptions.ClockDivider = 0;
  SpiOptions.BusWidth = 1; /* only single SPI transfer supported */
  SpiOptions.Mode = Bridge::spi_clock_mode_t::spi_mode_0;
  SpiOptions.Speed = 10000000;  
  HandleSPI = CleO.DeviceOpen(0, BRIDGE_TYPE_SPI, sizeof(SpiOptions), (uint8_t *)&SpiOptions );        
  if(HandleSPI < 0 )
  {   
    /* Failed to open a handle */
    Serial.print("DeviceOpen failed");                            
  }
  else
  {
    /* Success in opening SPI interface */                    
    delay(1000); /* Sufficient Delay for MAX7219 to bootup */

    /* Configure MAX7219 for mode, intensity, scan, shutdown */
    uint8_t d88[2] = { 0, 0 };

    /* Configure MAX7219 for no-decode operation */
    d88[0] = DISPLAY_TEST; d88[1] = NORMAL_MODE; 
    /* Write SPI device using bridging commands */
    CleO.DeviceWrite(HandleSPI,2,d88,BytesWritten);
    d88[0] = INTENSITY; d88[1] = INTENSITY_LOW; 
    CleO.DeviceWrite(HandleSPI,2,d88,BytesWritten);
    d88[0] = DECODE_MODE; d88[1] = NO_DECODE_MODE; 
    CleO.DeviceWrite(HandleSPI,2,d88,BytesWritten);
    d88[0] = SCAN_LIMIT;  d88[1] = SCAN_ALL; 
    CleO.DeviceWrite(HandleSPI,2,d88,BytesWritten);
    d88[0] = SHUTDOWN; d88[1] = SHDN_NORMAL_OPERATION; 
    CleO.DeviceWrite(HandleSPI,2,d88,BytesWritten);

    /* Display a 'X' character on 8x8 */
    uint8_t X[8] = {0x00, 0xC6, 0xEE, 0x38, 0x38, 0xEE, 0xC6, 0x00};

    for (int i = 0; i <= 7; i++) 
    {
      d88[0] = i + 1;   d88[1] = X[i];
      CleO.DeviceWrite(HandleSPI,2,d88,BytesWritten);
    }
    
    /* Close SPI device using bridging commands       
     */
    HandleSPI = CleO.DeviceClose(HandleSPI);  
  }   
}

void loop()
{
}