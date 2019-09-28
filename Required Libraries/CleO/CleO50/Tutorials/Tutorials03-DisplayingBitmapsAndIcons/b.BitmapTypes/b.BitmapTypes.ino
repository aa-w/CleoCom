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

  CleO.BitmapJustification(ML);

  CleO.StringExt(FONT_MEDIUM, 350, 40, LIGHT_GREEN, MR, 0, 0, "JPEG Bitmap");

  /* Draw a bitmap at (450, 40) using hJPEG handle */
  CleO.Bitmap(hJPEG, 450, 40);

  CleO.StringExt(FONT_MEDIUM, 350, 240, LIGHT_GREEN, MR, 0, 0, "BMP Bitmap");

  /* Draw a bitmap at (450, 240) using hBMP handle */
  CleO.Bitmap(hBMP, 450, 240);

  CleO.StringExt(FONT_MEDIUM, 350, 440, LIGHT_GREEN, MR, 0, 0, "PNG Bitmap");

  /* Draw a bitmap at (450, 440) using hPNG handle */
  CleO.Bitmap(hPNG, 450, 440);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
