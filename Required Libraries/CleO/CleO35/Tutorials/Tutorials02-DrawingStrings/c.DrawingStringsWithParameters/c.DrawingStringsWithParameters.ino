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
  Tutorial Name: Text Basics
  Description:
  This tutorial shows how to draw a text/string on screen using StringExt().
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  /* Draw a string "Hello world" in light green color,
     at (x, y) = (160, 240),
     with 0 angle and radius,
     with medium size font
  */
  CleO.StringExt(FONT_MEDIUM, 160, 240, LIGHT_GREEN, MM, 0, 0, "Hello world");

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}