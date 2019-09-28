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
  Tutorial Name: Drawing a Rectangle with round corners
  Description: This tutorial shows how to draw a rectangle with rounded corners.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  CleO.RectangleJustification(MM);

  /* set the rectangle corner radius */
  CleO.RectangleCorner(20);

  /* Draw a white rectangle at (left, top) - (160, 240)
  point with 200 width, 400 height */
  CleO.RectangleXY(160, 240, 200, 400);

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
