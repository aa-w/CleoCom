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
  Tutorial Name: Loading Icons
  Description:
  This tutorial shows how to load icons
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  for (int i = 0; i < 100; i++) {
    int x = 40 * (i % 8);
    int y = 40 * (i / 8);

    /*
      first parameter - handle to icon
      random() % 900 - index in the bitmap
      (x, y) coordinate at which icons needs to be drawn
    */
    CleO.Bitmap(CleO.LoadIcon("@Icons/m36.ftico", i), x, y);
  }

  /* Display completed screen frame */
  CleO.Show();
}


void loop() {}