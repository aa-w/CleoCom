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
  Tutorial Name: Screen Capture To File
  Description:
  This tutorial shows how to capture screen-shot in a file.
*/
#include <SPI.h>
#include <CleO.h>

int16_t h0;

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  /* Draw a sample image */
  CleO.SetBackgroundcolor(DARK_BLUE);
  CleO.CircleExt(60, 60, 10, 20, DARKORANGE3, MM, 0, 0);
  CleO.CircleExt(60, 60, 30, 40, DARKORANGE2, MM, 0, 0);
  CleO.CircleExt(60, 60, 50, 60, DARKORANGE1, MM, 0, 0);
  CleO.StringExt(FONT_MEDIUM, 60, 60, WHITE, MM, 0, 0, "CleO");

  /* Display completed screen frame */
  CleO.Show();

  /*
     Capture screen starting from (0,0) and with 120 width and height.
  */
  CleO.CaptureScreen("@Pictures/screenshot.png", 0, 0, 120, 120);

  /* Get the handle of saved screen shot file */
  h0 = CleO.LoadImageFile("@Pictures/screenshot.png", 0);

  /* Fill up the entire screen with captured screen-shot */
  CleO.Start();
  CleO.BitmapJustification(MM);
  for (int x = 0; x < 800 + 60; x += 120)
    for (int y = 0; y < 480 + 60; y += 120)
      CleO.Bitmap(h0, x, y);
  CleO.Show();

  /* Capture the final screen-shot */
  CleO.CaptureScreen("@Pictures/capturescreen.png", 0, 0, 800, 480);
}

void loop() {}