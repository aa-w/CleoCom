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
  Tutorial Name: Bitmap Rotation
  Description:
  This tutorial shows how to rotate a bitmap.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  int centerX = 800 / 2, centerY = 480 / 2;

  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Get the handle by loading bitmap */
  int16_t handle = CleO.LoadImageFile("@Pictures/ctg.jpg", 0);

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, centerX, 50, LIGHT_GREEN, MM, 0, 0,
  "30 degrees rotation");
  CleO.StringExt(FONT_MEDIUM, centerX, 425, LIGHT_GREEN, MM, 0, 0,
  "MM Justification");

  /* Draw bitmap at 30 degrees rotation with MM justification */
  CleO.BitmapJustification(MM);
  CleO.BitmapTransform(1, 30);
  CleO.Bitmap(handle, centerX, centerY);

  /* Draw a point at bitmap justification for a reference */
  CleO.PointRadius(10);
  CleO.PointColor(LIGHT_GREEN);
  CleO.Point(centerX, centerY);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
