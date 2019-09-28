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
  Tutorial Name: Dial Widget
  Description:
  This tutorial shows how to use dial widget.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  int x = 160, y = 240, radius = 50;
  uint16_t options = 0;
  uint16_t value1 = 24000;
  uint16_t value2 = 12000;
  uint16_t value3 = 48000;

  /* Draw dials with various values */
  CleO.eve_dial(x, y - 130, radius, options, value1);
  CleO.eve_dial(x, y, radius, options, value2);
  CleO.eve_dial(x, y + 130, radius, options, value3);

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
