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
  Tutorial Name: SketchPad
  Description: This tutorial shows how to use SketchPad() widget.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  /* Enable sketching on the screen */
  CleO.Sketch(0, 0, 800, 480);

  CleO.StringExt(FONT_MEDIUM, 400, 240, YELLOW, MM, 0, 0, "START SKETCHING");

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
