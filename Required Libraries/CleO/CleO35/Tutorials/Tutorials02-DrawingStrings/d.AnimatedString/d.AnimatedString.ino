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
  Tutorial Name: Animated string
  Description:
  This tutorial shows how to draw string with animation effect.
*/
#include <SPI.h>
#include <CleO.h>

int minX = 75, minY = 15;
int maxX = 320 - minX, maxY = 480 - minY;
int startX = maxX / 2, startY = maxY / 2;
int deltaX = 2, deltaY = 2;

void setup() {
  /* Initialize CleO - needs to be done only once */
  CleO.begin();
}

void loop() {
  /* Start building a screen frame */
  CleO.Start();

  /* Draw the string and update its coordinates */
  CleO.StringExt(FONT_MEDIUM, startX, startY, LIGHT_GREEN, MM, 0, 0,
  "Hello world");
  startX = startX + deltaX;   startY = startY + deltaY;

  /* Take care of screen boundary cases */
  if (startX >= maxX)
    deltaX = -deltaX;

  if (startX <= minX)
    deltaX = abs(deltaX);

  if (startY >= maxY)
    deltaY = -deltaY;

  if (startY <= minY)
    deltaY = abs(deltaY);

  /* Display completed screen frame */
  CleO.Show();
}