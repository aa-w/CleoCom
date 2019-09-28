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
  Tutorial Name: Setting Landscape and Portrait Screen Modes
  Description: This tutorial shows how to set landscape and portrait screen mode.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 0, 0, LIGHT_GREEN, TL, 0, 0, "Landscape Mode");

  /* Display completed screen frame */
  CleO.Show();

  delay(3000);

  /* Set screen coordinate in portrait mode */
  CleO.DisplayRotate(2, 0);

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 0, 0, LIGHT_GREEN, TL, 0, 0, "Portrait Mode");

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
