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
  Tutorial Name: Set Angle Type
  Description:
  This tutorial shows how to set different unit for angle.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  CleO.LineColor(LIGHT_GREEN);
  CleO.LineWidth(5);

  /* Draw a line with default angle type - 0 degree */
  CleO.StringExt(FONT_MEDIUM, 10, 50, LIGHT_GREEN, TL, 0, 0, "Default Angle");
  CleO.StringExt(FONT_SANS_2, 10, 100, LIGHT_GREEN, TL, 0, 0, "0 Degree");
  CleO.LineXY(250, 25, 100);

  /* Draw a line with 120 degree */
  CleO.SetAngleType(DEGREES);
  CleO.StringExt(FONT_MEDIUM, 750, 240 - 40, LIGHT_GREEN, MR, 0, 0, "120 Degree");
  CleO.LineAngle(120);
  CleO.LineXY(550, 240, 100);

  CleO.StringExt(FONT_SANS_2, 75, 320, LIGHT_GREEN, TL, 0, 0, "180 Degree");
  CleO.StringExt(FONT_MEDIUM, 75, 350, LIGHT_GREEN, TL, 0, 0, "32760 FURMANS");
  CleO.StringExt(FONT_SANS_0, 90, 400, LIGHT_GREEN, TL, 0, 0,
  "1 degree is approx. 182 FURMANS");

  /* Change angle unit */
  CleO.SetAngleType(FURMANS);

  /* 1 degree is approximately 182 Furmans */
  CleO.LineAngle(180 * 182);
  CleO.LineXY(25, 425, 100);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
