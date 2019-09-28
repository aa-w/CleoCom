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
  Tutorial Name: Loading icons
  Description:
  This tutorial shows how to draw bitmaps and load icons on screen.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  int icons = 170;
  int gap = 47;
  int columns = 17;

  /* Start building a screen frame */
  CleO.Start();

  for (int i = 0 ; i < icons; i++) {
    int x = gap * (i % columns);
    int y = gap * (i / columns);

    /*
       first parameter - handle to icon
       i - index in the bitmap
       (x, y) coordinate at which icons needs to be drawn
    */
    CleO.Bitmap(CleO.LoadIcon("@Icons/m48.ftico", i), x, y);
  }

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
