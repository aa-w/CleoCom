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
  Tutorial Name: Loading External Font
  Description:
  This tutorial shows how to draw string using external font.
*/
#include <SPI.h>
#include <CleO.h>

int16_t font;

void setup()
{
  int screenWidth = 800, screenheight = 480;

  /* Initialize CleO */
  CleO.begin();

  /* Load font file and get the handle for it */
  font = CleO.LoadFont("@Fonts/DSEG7ClassicMini-BoldItalic.ftfont");

  /* Start building a screen frame */
  CleO.Start();

  /* Draw string (digits) using external font */
  CleO.StringExt(font, screenWidth / 2, 15, 0xff4040UL, TM, 0, 0, "1 2 3");
  CleO.StringExt(font, screenWidth / 2, screenheight / 2, 0xff4040UL, MM, 0,
  0, "4 5 6");
  CleO.StringExt(font, screenWidth / 2, 400, 0xff4040UL, MM, 0, 0, "7 8 9");

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}