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
  Tutorial Name: Working with Needles Angle
  Description: This tutorial shows how to draw needle at an angle.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  /* Select needle width*/
  CleO.NeedleWidth(20);

  /* Draw needles at different angles 0, 90, 180 and 270 */
  CleO.NeedleXY(400, 240, 200, 0);
  CleO.NeedleXY(400, 240, 170, 90);
  CleO.NeedleXY(400, 240, 200, 180);
  CleO.NeedleXY(400, 240, 170, 270);

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
