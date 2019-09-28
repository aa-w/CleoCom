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
  Tutorial Name: Free Handle Command
  Description:
  This tutorial shows how to free handle.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  CleO.BitmapJustification(MM);

  /* Get the handle for bitmap file */
  int16_t hJPEG = CleO.LoadImageFile("@Pictures/ctg.jpg", 0);

  /* Draw a bitmap at (400, 240) using hJPEG handle */
  CleO.Bitmap(hJPEG, 400, 240);

  /* Display completed screen frame */
  CleO.Show();

  /* Free bitmap handle */
  CleO.Free(hJPEG);
}

void loop()
{
}
