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
  Tutorial Name: Line Segments Drawing with Angles
  Description: This tutorial shows how to draw lines with angles.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  /* Inner radius */
  int radius1 = 143;

  /* Outer radius */
  int radius2 = 155;

  for (int deg = 0; deg < 360; deg += 6)
  {
    if (deg == 0 || deg == 90 || deg == 180 || deg == 270)
      radius1 = 135;
    else
      radius1 = 143;

    /* Draw a line with center (400, 240) at deg angle */
    CleO.LineR1R2(400, 240, radius1, radius2, deg);
  }

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
