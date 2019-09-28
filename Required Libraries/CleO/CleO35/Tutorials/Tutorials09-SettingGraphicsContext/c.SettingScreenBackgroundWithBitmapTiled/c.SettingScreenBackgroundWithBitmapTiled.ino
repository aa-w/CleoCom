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
  Tutorial Name: Setting Screen Background Image
  Description: This tutorial shows how to set screen background image.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Get handle to an image */
  int16_t h = CleO.LoadImageFile("@Pictures/ctg.jpg", 0);

  /* Set screen background */
  CleO.SetBackgroundImage(h, TILED);

  /* Release image handle */
  CleO.Free(h);

  /* Start building a screen frame */
  CleO.Start();

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}