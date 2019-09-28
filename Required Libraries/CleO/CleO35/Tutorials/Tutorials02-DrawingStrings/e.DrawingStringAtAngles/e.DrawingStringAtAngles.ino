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
  Tutorial Name: Strings at angles
  Description:
  This tutorial shows how to draw a text/string at various angles.
*/
#include <SPI.h>
#include <CleO.h>

char message[] = "HELLOWORLD!!";
char currentStr[2] = { '\0', '\0' };
int centerX = 320 / 2, centerY = 480 / 2;

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  int angle = 0;
  for (int i = 0; i <= 11; ++i) {
    currentStr[0] = message[i];

    /* Draw text at various angles */
    CleO.StringExt(FONT_MEDIUM, centerX, centerY, LIGHT_GREEN, MM, angle,
    150, currentStr);

    angle = (angle + 30) % 360;
  }

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}