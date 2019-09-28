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
  Tutorial Name: Scrollbar Widget
  Description:
  This tutorial shows how to use scrollbar widget.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  int x = 100, y = 240, w = 166, h = 10;
  uint16_t options = 0;

  uint16_t value1 = 24000;
  uint16_t value2 = 12000;
  uint16_t value3 = 48000;
  uint16_t range = 65535;
  uint16_t size = 32768;

  /* Draw scrollbar with various values */
  CleO.eve_scrollbar(x, y - 100, w, h, options, value1 / 2, size, range);
  CleO.eve_scrollbar(x, y, w, h, options, value2 / 2, size, range);
  CleO.eve_scrollbar(x, y + 100, w, h, options, value3 / 2, size, range);

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}