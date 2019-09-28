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
  Tutorial Name: Circle Drawing with radius and color.
  Description: This tutorial shows how to draw a circle with radius and
  color parameters.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{

  /* Initialize CleO */
  CleO.begin();

  /* Draw 6 circles with a center circle and 5 circles at equal angle */
  CleO.Start();
  int cx = 400;
  int cy = 240;

  /* Draw center circle with 20 pixels thickness and dark orange in color */
  CleO.CircleExt(cx, cy, 140, 160, DARK_ORANGE, MM, 0, 0);

  /* Draw 5 circles each at 72 degrees starting from bottom down */
  for (int a = 0; a < 360; a += 72)
    CleO.CircleExt(cx, cy, 40, 50, LIGHT_BLUE, MM, a, 150);

  /* Set the background color to be gradient from top left corner to
  bottom right corner */
  CleO.SetBackgroundGradient(0, 0, 800, 480, DARK_BLUE, DARK_RED);
  CleO.Show();
}

void loop()
{
}
