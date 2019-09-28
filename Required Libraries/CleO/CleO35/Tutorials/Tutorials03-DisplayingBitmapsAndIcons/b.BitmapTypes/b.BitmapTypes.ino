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
  Tutorial Name: Bitmap Types
  Description:
  This tutorial shows how to draw different types of bitmaps.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  /* Get the handle for JPEG, BMP and PNG bitmap files */
  int16_t hJPEG = CleO.LoadImageFile("@Pictures/speaker.jpg", 0);
  int16_t hBMP = CleO.LoadImageFile("@Pictures/Spectrum/15.bmp", 0);
  int16_t hPNG = CleO.LoadImageFile("@Pictures/ball.png", 0);

  CleO.StringExt(FONT_MEDIUM, 10, 50, LIGHT_GREEN, TL, 0, 0, "JPEG Bitmap");

  /* Draw a bitmap at (250, 40) using hJPEG handle */
  CleO.Bitmap(hJPEG, 250, 40);

  CleO.StringExt(FONT_MEDIUM, 10, 225, LIGHT_GREEN, TL, 0, 0, "BMP Bitmap");

  /* Draw a bitmap at (250, 150) using hBMP handle */
  CleO.Bitmap(hBMP, 250, 150);

  CleO.StringExt(FONT_MEDIUM, 10, 375, LIGHT_GREEN, TL, 0, 0, "PNG Bitmap");

  /* Draw a bitmap at (250, 375) using hPNG handle */
  CleO.Bitmap(hPNG, 250, 375);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}