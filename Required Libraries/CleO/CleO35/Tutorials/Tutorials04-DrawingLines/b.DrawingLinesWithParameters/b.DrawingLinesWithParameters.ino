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
  Tutorial Name: Line Drawing Basics
  Description: This tutorial shows how to draw a line with color and
  width parameters.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  /* select line width - 8 */
  CleO.LineWidth(8);

  /* select line color - yellow */
  CleO.LineColor(YELLOW);

  /* draw a line on screen connecting two points (10, 10) & (310, 470) */
  CleO.Line(10, 10, 310, 470);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}