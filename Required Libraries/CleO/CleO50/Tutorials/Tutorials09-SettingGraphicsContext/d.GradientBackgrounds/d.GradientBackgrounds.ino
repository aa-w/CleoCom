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
  Tutorial Name: Setting Gradient Screen Background
  Description: This tutorial shows how to gradient screen background.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Set background gradient */
  CleO.SetBackgroundGradient(0, 0, 800, 480, DARK_BLUE, DARK_GREEN);

  /* Start building a screen frame */
  CleO.Start();

  /* Draw string */
  CleO.StringExt(FONT_MEDIUM, 400, 240, WHITE, MM, 0, 0, "Hello world");

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
