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
  Tutorial Name: Creating Touch with Images
  Description: This tutorial shows how to create touch button using image.÷
*/
#include <SPI.h>
#include <CleO.h>

bool penDown = false;

int centerX, centerY;
int16_t handle;

void setup()
{
  /* Initialize CleO */
  CleO.begin();

  centerX = 800 / 2, centerY = 480 / 2;

  /* Get the handle by loading bitmap */
  handle = CleO.LoadImageFile("@Pictures/ctg.jpg", 0);

  CleO.SetBackgroundcolor(BLUE);
}

void display()
{
  /* Start building a screen frame */
  CleO.Start();

  CleO.BitmapJustification(MM);
  CleO.RectangleJustification(MM);

  CleO.StringExt(FONT_MEDIUM, centerX, 0, LIGHT_GREEN, TM, 0, 0, "Press Image");

  /* Draw background rectangles only when image is not touched */
  if (!penDown) {
    CleO.RectangleColor(BLACK);
    CleO.RectangleXY(centerX - 1, centerY - 1, 200, 246);

    CleO.RectangleColor(WHITE);
    CleO.RectangleXY(centerX + 1, centerY + 1, 196, 247);
  }

  CleO.Tag(1);
  /* Display original bitmap */
  CleO.Bitmap(handle, centerX, centerY);

  /* Display the completed screen frame */
  CleO.Show();
}

void loop()
{
  display();
  control();
}

int16_t previousDur = 0;
int16_t previousTag = 0;

void control()
{
  int16_t x, y, dur, tag;
  CleO.TouchCoordinates(x, y, dur, tag);

  /* pen down */
  if (previousDur == 0 && dur != 0) {
    previousTag = tag;
    previousDur = 1;

    if (tag == 1)
      penDown = true;
  }
  /* pen up */
  else if (previousDur == 1 && dur == 0) {
    previousDur = 0;

    if (previousTag == 1)
      penDown = false;
  }
}
