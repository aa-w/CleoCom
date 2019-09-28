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
  Tutorial Name: Working with Needles segments
  Description: This tutorial shows how to draw needle segments with parameters.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  int cx = 400;
  int cy = 240;

  int radius = 160;

  CleO.CircleExt(cx, cy, 0, radius, DARK_GREY, MM, 0, 0);

  CleO.NeedleWidth(20);

  CleO.NeedleColor(RED);
  CleO.Needle(cx - radius, 30, cx + radius, 30);
  CleO.Needle(cx + radius, 60, cx - radius, 60);

  /* draw white needle of length radius at 0 angle */
  CleO.NeedleColor(WHITE);
  CleO.NeedleXY(cx, cy, radius, 0);

  /* draw dark green needle at 120 angle */
  CleO.NeedleColor(DARK_GREEN);
  CleO.NeedleR1R2(cx, cy, 30, 160, 120);

  /* draw pink needle at 240 angle */
  CleO.NeedleColor(PINK);
  CleO.NeedleR1R2(cx, cy, -30, 160, 240);

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
