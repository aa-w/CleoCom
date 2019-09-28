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
  Tutorial Name: Keys Widget
  Description:
  This tutorial shows how to use keys widget.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  int x = 0, w = 320, h = 30;
  uint16_t options = 0;
  uint8_t key = 0;

  /* Draw keys */
  int y = 268;
  const char *rows[4] = { "0123456789", "QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM,."};
  for (int i = 0; i < 4; i++) {
    CleO.eve_keys(x, y, w, h, FONT_MEDIUM, options | OPT_CENTER | key, rows[i]);
    y += 34;
  }

  /* Display completed screen frame */
  CleO.Show();
}

void loop()
{
}