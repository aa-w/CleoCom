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
  Tutorial Name: Working with Needles parameters
  Description: This tutorial shows how to draw needle with parameters X,Y,Color,
  Angle and Radius.
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

  /* Draw needle at (400, 240) with 230 angle, 100 length and 20 radius */
  CleO.NeedleExt(400, 240, 100, LIGHT_GREEN, 230, 20);

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
