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
  Tutorial Name: Point Drawing with radius and color.
  Description: This tutorial shows how to draw a point with radius and
  color parameters.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  /* select point radius - 3 */
  CleO.PointRadius(3);

  /* select point color - red */
  CleO.PointColor(RED);

  for (int x = 10; x < 800; x += 20)
    for (int y = 10; y < 480; y += 20)
      /* draw a point at (x, y) coordinate */
      CleO.Point(x, y);

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}