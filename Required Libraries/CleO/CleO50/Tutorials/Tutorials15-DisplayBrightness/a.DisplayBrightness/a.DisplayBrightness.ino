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
  Tutorial Name: Display Brightness
  Description: This tutorial shows how to change display brightness.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();
  CleO.SetBackgroundcolor(WHITE);
  CleO.StringExt(FONT_MEDIUM, 400, 240, BLACK, MM, 0, 0, "Brightness - 100%");
  /* Display completed screen frame */
  CleO.Show();

  delay(1000);

  /* Start building a screen frame */
  CleO.Start();
  CleO.SetBackgroundcolor(WHITE);
  CleO.StringExt(FONT_MEDIUM, 400, 240, BLACK, MM, 0, 0, "Brightness - 10%");
  /* Display completed screen frame */
  CleO.Show();

  /* set brightness to 10% */
  CleO.Display(255 / 10);
  delay(3000);

  /* Start building a screen frame */
  CleO.Start();
  CleO.StringExt(FONT_MEDIUM, 400, 240, BLACK, MM, 0, 0, "Brightness - 100%");
  /* Display completed screen frame */
  CleO.Show();
  /* set brightness to 100% */
  CleO.Display(255);
}

void loop() {}
