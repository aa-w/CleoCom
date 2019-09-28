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
  Tutorial Name: Point Drawing with angles
  Description: This tutorial shows how to draw a points at angles.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  for (int deg = 0; deg < 360; deg += 6)
  {
    /* Draw white point at 150 distance from center (400, 235) with
    a radius of 4 */
    CleO.PointExt(400, 235, 4, WHITE, deg, 150);
  }

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}