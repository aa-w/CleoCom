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
  Tutorial Name: Clock Widget
  Description:
  This tutorial shows how to use clock widget.
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
  uint16_t r = 70, h = 8, m = 15, s = 0, ms = 0;
  uint16_t options = 0;

  /* Draw clock with various values */
  CleO.eve_clock(x, y - 150, r, options | OPT_NOSECS, h, m, s, ms);
  CleO.eve_clock(x, y, r, OPT_NOTICKS, 2, 30, 0, ms);
  CleO.eve_clock(x, y + 150, r, OPT_NOBACK, 0, 0, (7 * 3600) + (38 * 60) + 59, 0);

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
