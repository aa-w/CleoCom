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
  Tutorial Name: Button Widget
  Description:
  This tutorial shows how to use button widget.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  uint16_t w = 120, h = 100;
  int x = 400 - (w/2), y1 = 240 - (2*h), y2 = 240 + h;
  uint16_t options = 0;

  /* Draw buttons */
  CleO.eve_button(x, y1, w, h, FONT_HUGE, OPT_FLAT, "2D");
  CleO.eve_button(x, y2, w, h, FONT_HUGE, options, "3D");

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
