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
   Tutorial Name: Circle Drawing Basics
   Description: This tutorial shows how to draw a circle on screen
   using CircleExt().
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  /*
     Draw a circle with (400, 240) centre,
     150 as outter radius, 140 as innner radius,
     with dark orange color and 0 angle and radiusat
  */
  CleO.CircleExt(400, 240, 140, 150, DARK_ORANGE, MM, 0, 0);

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}
