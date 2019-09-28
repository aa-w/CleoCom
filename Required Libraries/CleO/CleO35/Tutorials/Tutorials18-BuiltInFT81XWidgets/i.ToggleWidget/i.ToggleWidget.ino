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
  Tutorial Name: Toggle Widget
  Description:
  This tutorial shows how to use toggle widget.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  int x = 100, y = 240;
  uint16_t width = 130;
  int font = FONT_MEDIUM;
  uint16_t options = 0;

  /* state of the toggle : 0 is off, 65535 is on */
  uint16_t value1 = 65535;
  uint16_t value2 = 0;
  uint16_t value3 = 30000;

  /* Draw toggles with various state values */
  CleO.eve_toggle(x, y - 75, width, font, options, value1, "off" "\xff" "on");
  CleO.eve_toggle(x, y, width, font, options, value2, "off" "\xff" "on");
  CleO.eve_toggle(x, y + 75, width, font, options, value3, "off" "\xff" "on");

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}