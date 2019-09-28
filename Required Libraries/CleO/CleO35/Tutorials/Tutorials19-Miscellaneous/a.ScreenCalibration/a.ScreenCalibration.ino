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
  Tutorial Name: Calibrate Display
  Description:
  This tutorial shows how to calibrate screen display.
*/
#include <SPI.h>
#include <CleO.h>

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 480
void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start screen calibration */
  CleO.Calibration();
}

void loop()
{
  int16_t x, y, dur, tag;
  CleO.TouchCoordinates(x, y, dur, tag);

  CleO.Start();
  
  CleO.RectangleJustification(TL);
  CleO.RectangleColor(BLACK);
  CleO.RectangleXY(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); 
  
  CleO.StringExt(FONT_MEDIUM, 160, 150, LIGHT_GREEN, MM, 0, 0,
  "touch test started");

  if (dur)
    CleO.CircleExt(x, y, 10, 20, DARK_ORANGE, MM, 0, 0);
  else
    CleO.StringExt(FONT_MEDIUM, 160, 240, LIGHT_GREEN, MM, 0, 0,
    "no touch detected");

  CleO.Show();
}
