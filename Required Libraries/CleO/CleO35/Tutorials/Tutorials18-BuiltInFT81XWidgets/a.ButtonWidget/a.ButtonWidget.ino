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

  int x = 160, y = 240;
  uint16_t w = 40, h = 30;
  uint16_t options = 0;

  /* Draw buttons */
  CleO.eve_button(x, y, w, h, FONT_MEDIUM, OPT_FLAT, "2D");
  CleO.eve_button(x, y + 50, w, h, FONT_MEDIUM, options, "3D");

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
