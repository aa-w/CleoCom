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
      MikroE Heart Rate Click module is used as I2C device fot this tutorial functioning.
      The Heart Rate Click in turn uses the MAX30100 Pulse Oximeter sensor.
  External component:   
      Mikroe Heart Rate module
  REFERENCES:
      1. MAX30100 Pulse Qximeter -
           https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/MAX30100.html
      2. MikroE Heart Rate Click - http://www.mikroe.com/click/heart-rate/
  Hardware Connection:
      HeartRate Click   Nero
      VCC               3.3V
      GND               GND 
      SDA               SDA
      SCL               SCL
*/

#include <SPI.h>
#include <CleO.h>

/* MAX30100 definitions */
#define I2C_ADR 0x57
#define EXP_PART_ID  0x11

#define REG_CONFIG_MODE             0x06
#define REG_CONFIG_LED              0x09
#define REG_INT_ENABLE              0x01
#define REG_CONFIG_SPIO2            0x07
#define REG_FIFO_WRITE_PTR          0x02
#define REG_FIFO_READ_PTR           0x04
#define REG_FIFO_OVERFLOW_CNTR      0x03
#define REG_PART_ID                 0xFF
#define REG_REV_ID                  0xFE

#define MODE_RESET                  (0x01 << 6)
#define LED_CURRENT                 0x0F
#define ENABLE_HR_INT               (0x01 << 5)
#define SAMPLE_RATE_CFG             (0x01 << 2)
#define LED_PW_400                  0x01
#define MODE_HR_ONLY                0x02

int16_t HandleI2C = -1; 
int16_t BytesWritten;

void setup()
{    
  CleO.begin();
  Serial.begin(115200);
  
  /* Open I2C device using bridging commands */
  Bridge::i2cm_options i2c_open_opt;
  i2c_open_opt.SlaveAddress = I2C_ADR;
  i2c_open_opt.ClockSpeed = 400000;
  HandleI2C = CleO.DeviceOpen(0, BRIDGE_TYPE_I2CM, sizeof(i2c_open_opt), (uint8_t*)&i2c_open_opt);
  if(HandleI2C < 0 )
  {
    /* Failed to open a handle */
    Serial.print("DeviceOpen failed");  
  }
  else
  {
    /* Success in opening I2C interface */
    uint8_t data[4];
    /* reset the sensor */		
    data[0] = REG_CONFIG_MODE;
    data[1] = MODE_RESET;
    /* Write I2C device using bridging commands */
    CleO.DeviceWrite(HandleI2C, 2, data, BytesWritten);        
    delay(500);
    
    data[0] = REG_CONFIG_LED;
    data[1] = (uint8_t)LED_CURRENT << 4 | (uint8_t)LED_CURRENT;
    CleO.DeviceWrite(HandleI2C, 2, data, BytesWritten);        
    data[0] = REG_CONFIG_LED;
    /* Read I2C device using bridging commands */
    CleO.DeviceRead(HandleI2C, 1, 1, &data[0], &data[1]);
    
    data[0] = REG_INT_ENABLE;
    data[1] = ENABLE_HR_INT;
    CleO.DeviceWrite(HandleI2C, 2, data, BytesWritten);    
    data[0] = REG_CONFIG_SPIO2;
    data[1] = SAMPLE_RATE_CFG | LED_PW_400;
    CleO.DeviceWrite(HandleI2C, 2, data, BytesWritten);    

    data[0] = REG_CONFIG_SPIO2;
    CleO.DeviceRead(HandleI2C, 1, 1, &data[0], &data[1]);
    /* Clear FIFO pointers */
    data[0] = REG_FIFO_WRITE_PTR;
    data[1] = 0;
    CleO.DeviceWrite(HandleI2C, 2, data, BytesWritten);    
    data[0] = REG_FIFO_READ_PTR;
    data[1] = 0;
    CleO.DeviceWrite(HandleI2C, 2, data, BytesWritten);    
    data[0] = REG_FIFO_OVERFLOW_CNTR;
    data[1] = 0;
    CleO.DeviceWrite(HandleI2C, 2, data, BytesWritten);    
    data[0] = REG_CONFIG_MODE;
    data[1] = MODE_HR_ONLY;
    CleO.DeviceWrite(HandleI2C, 2, data, BytesWritten);    		

    data[0] = REG_CONFIG_MODE;
    CleO.DeviceRead(HandleI2C, 1, 1, &data[0], &data[1]);		
    data[0] = REG_FIFO_OVERFLOW_CNTR;
    CleO.DeviceRead(HandleI2C, 1, 1, &data[0], &data[1]);
    
    data[0] = REG_PART_ID;
    CleO.DeviceRead(HandleI2C, 1, 1, &data[0], &data[1]);
    if(data[1] != EXP_PART_ID)        
        Serial.println("HR Click Init failed!");        
    else
        Serial.print("Part ID : ");Serial.println(data[1]);
    
    data[0] = REG_REV_ID;
    CleO.DeviceRead(HandleI2C, 1, 1, &data[0], &data[1]);
    Serial.print("Rev ID : ");
    Serial.println(data[1]); 
    
    /* Close I2C device using bridging commands */
    HandleI2C = CleO.DeviceClose(HandleI2C);  /* DeviceClose returns 255 for I2C close */
  }
}

void loop()
{
}