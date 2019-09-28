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
  Tutorial Name: Bitmap Parameters
  Description:
  This tutorial shows how to setup parameters for a bitmap.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Get the handle for the image */
  int16_t im = CleO.LoadImageFile("@Pictures/ctg.jpg", 0);

  /* Start building a screen frame */
  CleO.Start();

  /* Select MM as a justification for bitmap drawing */
  CleO.BitmapJustification(MM);

  /* Draw a bitmap at (150, 200) using im handle */
  CleO.Bitmap(im, 150, 200);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}