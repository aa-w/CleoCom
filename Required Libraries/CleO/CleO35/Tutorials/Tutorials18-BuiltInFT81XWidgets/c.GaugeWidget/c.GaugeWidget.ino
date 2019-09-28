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
  Tutorial Name: Gauge Widget
  Description:
  This tutorial shows how to use gauge bar widget.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  int x = 160, y = 240;
  uint16_t r = 40, major = 4, minior = 3;
  uint16_t options = 0;
  uint16_t value1 = 24000;
  uint16_t value2 = 12000;
  uint16_t value3 = 48000;
  uint16_t range = 65535;

  /* Draw gauge with various values */
  CleO.eve_gauge(x, y - 100, r, options, major, minior, value1, range);
  CleO.eve_gauge(x, y, r, options, major, minior, value2, range);
  CleO.eve_gauge(x, y + 100, r, options, major, minior, value3, range);

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
