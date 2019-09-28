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
  Tutorial Name: Loading and Playing Audio Files
  Description: This tutorial shows how to play audio file.
*/
#include <SPI.h>
#include <CleO.h>

int16_t icon_play, icon_loop, icon_stop;

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Get handles to icons */
  icon_play = CleO.LoadIcon("@Icons/m48.ftico", ICON_PLAY_CIRCLE_FILLED);
  icon_loop = CleO.LoadIcon("@Icons/m48.ftico", ICON_REPLAY);
  icon_stop = CleO.LoadIcon("@Icons/m48.ftico", ICON_STOP);

  /* Start building a screen frame */
  CleO.Start();
  CleO.BitmapJustification(MM);

  /* Display and tag play icon */
  CleO.Tag(100);
  CleO.Bitmap(icon_play, 400, 100);

  /* Display and tag loop icon */
  CleO.Tag(101);
  CleO.Bitmap(icon_loop, 400, 200);

  /* Display and tag stop icon */
  CleO.Tag(102);
  CleO.Bitmap(icon_stop, 400, 300);

  CleO.Show();
}

int16_t previousDur = 0;
int16_t previousTag = 0;

void loop()
{
  int16_t x, y, dur, tag;
  CleO.TouchCoordinates(x, y, dur, tag);

  /* pen down */
  if (previousDur == 0 && dur != 0) {
    previousTag = tag;
    previousDur = 1;
  }
  /* pen up */
  else if (previousDur == 1 && dur == 0) {
    previousDur = 0;

    switch (tag) {
      case 100:
        CleO.AudioPlay("@Music/stinger.wav", PLAY_ONCE);
        break;
      case 101:
        CleO.AudioPlay("@Music/stinger.wav", PLAY_LOOP);
        break;
      case 102:
        CleO.AudioStop();
        break;
    }
  }
}
