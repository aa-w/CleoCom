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


/**
  Sketch:              
    Project02-ADTMFDialler.ino
  Purpose:              
    To desmonstrate how to create the DTMF dialer using the CleO primitives and touch subsystem.
  External component:   
    CleO Speaker
  Hardware Connection:  
    Connect CleO Speaker to Cleo50 CN2 connector
*/

#include <SPI.h>
#include <CleO.h>

int16_t previousTag = 0, previousDur = 0;

void setup() {
  /* Initialize CleO */
  CleO.begin();
  CleO.SetBackgroundGradient(0, 0, 0, 480, DARK_GRAY, BLACK);
}

void loop() {
  int16_t x, y, dur, tag;
  char *tones = "123456789*0#";
  char k[2] = " ";

  /* Check for touch and play sound for selected tone */
  CleO.TouchCoordinates(x, y, dur, tag);

  /* pen down */
  if (previousDur == 0 && dur != 0) {
    previousTag = tag;
    previousDur = 1;
    if (strchr(tones, tag)) {
      CleO.SetSound(tag, 0, 5, 0);
      CleO.SoundPlay(1);
    }
  }
  /* pen up */
  else if (previousDur == 1 && dur == 0) {
    previousDur = 0;
    CleO.SoundPlay(0);
  }
  /* pen drag */
  else if (previousDur == 1 && dur != 0) {
    /* keep tag to previously selected tone if touch is moved */
    if (tag != previousTag)
      tag = previousTag;
  }

  /* Start drawing screen frame */
  CleO.Start();

  /* Draw circle and assign tag for each tone */
  for (int i = 0; i < 12; i++) {
    int x = (i % 3) * 100 + (800/2) - 100;
    int y = 90 + (i / 3) * 100;
    int key = tones[i];
    int touching = (tag == key);

    k[0] = key;
    CleO.Tag(tones[i]);
    CleO.CircleExt(x, y, 0, 40, touching ? WHITE : DARK_ORANGE, MM, 0, 0);
    CleO.StringExt(FONT_SANS_6, x, y, BLACK, MM, 0, 0, k);
  }

  /* Display screen frame */
  CleO.Show();
}
