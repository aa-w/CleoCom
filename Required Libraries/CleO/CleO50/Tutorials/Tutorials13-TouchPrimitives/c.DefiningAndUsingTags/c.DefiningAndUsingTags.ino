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
  Tutorial Name: Using Tags
  Description: This tutorial shows how to use Tags
*/
#include <SPI.h>
#include <CleO.h>

bool pointTouched = false, lineTouched = false;

void setup()
{
  /* Initialize CleO */
  CleO.begin();
}

void display()
{
  /* Start drawing the screen shot */
  CleO.Start();

  CleO.StringExt(FONT_SANS_6, 400, 75, LIGHT_GREEN, MM, 0, 0, "TOUCH OBJECTS");

  CleO.Tag(1);
  CleO.LineWidth(12);
  CleO.LineColor(DARK_ORANGE);
  int line_len = 266;
  CleO.Line(400 - (line_len/2), 150, 400 + (line_len/2), 150);

  CleO.Tag(2);
  CleO.PointRadius(40);
  CleO.LineColor(DARK_ORANGE);
  CleO.Point(400, 330);

  if (lineTouched)
    CleO.StringExt(FONT_SANS_6, 400, 230, LIGHT_GREEN, MM, 0, 0,
    "Touching Line");

  if (pointTouched)
    CleO.StringExt(FONT_SANS_6, 400, 430, LIGHT_GREEN, MM, 0, 0,
    "Touching Point");

  /* Show the screenshot */
  CleO.Show();
}

void control()
{
  int16_t x, y, dur, tag, newval;
  CleO.TouchCoordinates(x, y, dur, tag);
  if (tag == 1)
    lineTouched = true;
  else if (tag == 2)
    pointTouched = true;
  else
    pointTouched = lineTouched = false;
}

void loop()
{
  display();
  control();
}
