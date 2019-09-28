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
  Tutorial Name: Bitmaps at Angles
  Description: This tutorial shows how to draw bitmap at various angles.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  int16_t digit[10];
  int radiuses[10] = {165, 155, 125, 145, 155, 155, 145, 125, 155};
  int centerX = 320 / 2, centerY = 480 / 2;

  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Get handles for all bitmap files */
  digit[0] = CleO.LoadImageFile("@Pictures/nixie/1.png", 0);
  digit[1] = CleO.LoadImageFile("@Pictures/nixie/2.png", 0);
  digit[2] = CleO.LoadImageFile("@Pictures/nixie/3.png", 0);
  digit[3] = CleO.LoadImageFile("@Pictures/nixie/4.png", 0);
  digit[4] = CleO.LoadImageFile("@Pictures/nixie/5.png", 0);
  digit[5] = CleO.LoadImageFile("@Pictures/nixie/6.png", 0);
  digit[6] = CleO.LoadImageFile("@Pictures/nixie/7.png", 0);
  digit[7] = CleO.LoadImageFile("@Pictures/nixie/8.png", 0);
  digit[8] = CleO.LoadImageFile("@Pictures/nixie/9.png", 0);

  /* Start building a screen frame */
  CleO.Start();

  /* Set the radius and justification for the bitmap files */
  CleO.BitmapJustification(MM);

  int angle = 0;
  for (int i = 0; i < 9; ++i) {
    CleO.BitmapRadius(radiuses[i]);
    CleO.BitmapAngle(angle);

    /* Draw bitmap at updated angle */
    CleO.Bitmap(digit[i], centerX, centerY);

    angle = (angle + 40) % 360;
    CleO.Free(digit[i]);
  }

  /* Display completed screen frame */
  CleO.Show();
}


void loop() {}