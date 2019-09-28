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
  Tutorial Name: Creating Touch Buttons With Lines and Points
  Description: This tutorial shows how to touch button using lines and points.
*/
#include <SPI.h>
#include <CleO.h>

bool penDownOnLine = false, penDownOnPoint = false;
int16_t previousDur = 0, previousTag = 0;

void setup()
{
  /* Initialize CleO */
  CleO.begin();

  CleO.SetBackgroundcolor(BLUE);
}

void loop()
{
  display();
  control();
}

void display()
{
  /* Start drawing screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 160, 150, LIGHT_GREEN, MM, 0, 0, "Press Button");
  CleO.LineWidth(20);

  if (!penDownOnLine) {
    CleO.LineColor(BLACK);
    CleO.Line(103, 228, 218, 228);

    CleO.LineColor(WHITE);
    CleO.Line(106, 231, 221, 231);
  }

  if (!penDownOnPoint) {
    CleO.PointExt(158, 328, 30, BLACK, 0, 0);
    CleO.PointExt(161, 331, 30, WHITE, 0, 0);
  }

  CleO.Tag(1);
  CleO.LineColor(RED);
  CleO.Line(105, 230, 220, 230);

  CleO.Tag(2);
  CleO.PointExt(160, 330, 30, RED, 0, 0);

  /* Display the completed screen frame */
  CleO.Show();
}

void control()
{
  int16_t x, y, dur, tag;
  CleO.TouchCoordinates(x, y, dur, tag);

  /* pen down */
  if (previousDur == 0 && dur != 0) {
    previousTag = tag;
    previousDur = 1;

    if (tag == 1)
      penDownOnLine = true;
    else if (tag == 2)
      penDownOnPoint = true;
  }
  /* pen up */
  else if (previousDur == 1 && dur == 0) {
    previousDur = 0;

    if (previousTag == 1)
      penDownOnLine = false;
    else if (previousTag == 2)
      penDownOnPoint = false;
  }
}