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
  Tutorial Name: Scaling Bitmap
  Description:
  This tutorial shows how to scale a bitmap.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  int centerX = 320 / 2, centerY = 480 / 2;

  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Get the handle by loading bitmap */
  int16_t handle = CleO.LoadImageFile("@Pictures/ctg.jpg", 0);

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, centerX, 0, LIGHT_GREEN, TM, 0, 0, "No Scaling");
  CleO.BitmapJustification(TM);

  /* Display original bitmap */
  CleO.Bitmap(handle, centerX, 40);

  CleO.BitmapJustification(TL);
  CleO.StringExt(FONT_MEDIUM, 30, 320, LIGHT_GREEN, ML, 0, 0, "25%");

  /* Scale down to 25% of bitmap */
  CleO.BitmapTransform(0.25, 0);
  CleO.Bitmap(handle, 30, 340);

  CleO.StringExt(FONT_MEDIUM, 175, 320, LIGHT_GREEN, ML, 0, 0, "50%");

  /* Scale down to 50% of bitmap */
  CleO.BitmapTransform(0.50, 0);
  CleO.Bitmap(handle, 175, 340);

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}