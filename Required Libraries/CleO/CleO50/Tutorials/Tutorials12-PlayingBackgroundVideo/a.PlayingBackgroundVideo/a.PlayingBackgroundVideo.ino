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
  Tutorial Name: Playing Background Video
  Description: This tutorial shows how to play video in background.
*/
#include <SPI.h>
#include <CleO.h>

#define SCREEN_WIDTH          800
#define SCREEN_HEIGHT         480
#define CENTRE_X              (0.5 * SCREEN_WIDTH)
#define CENTRE_Y              (0.5 * SCREEN_HEIGHT)

void setup() {
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Set background video */
  CleO.SetBackgroundVideo("@Videos/thun100.avi", 0);
}

void loop() {
  /* Start building screen frame */
  CleO.Start();

  CleO.StringExt(FONT_SANS_4, CENTRE_X, CENTRE_Y, BLACK, MM, 0, 0, "Background Video");

  /* Show screen frame */
  CleO.Show();
}
