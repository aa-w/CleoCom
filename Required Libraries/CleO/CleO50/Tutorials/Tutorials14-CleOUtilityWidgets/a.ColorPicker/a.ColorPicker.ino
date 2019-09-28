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
  Tutorial Name: ColorPicker
  Description: This tutorial shows how to use ColorPicker() widget.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();
}

uint32_t color1 = ORANGE;
uint32_t color2 = CYAN;

void loop()
{
  /* Start building a screen frame */
  CleO.Start();

  CleO.Tag(1);
  CleO.StringExt(FONT_MEDIUM, 400, 200, color1, MM, 0, 0, "Change color 1");
  CleO.Tag(2);
  CleO.StringExt(FONT_MEDIUM, 400, 250, color2, MM, 0, 0, "Change color 2");

  /* Display completed screen frame */
  CleO.Show();

  int16_t x, y, dur, tag;
  CleO.TouchCoordinates(x, y, dur, tag);
  if (tag == 1)
    CleO.ColorPicker("Changing color 1", color1);
  if (tag == 2)
    CleO.ColorPicker("Changing color 2", color2);
}
