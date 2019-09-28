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
  Tutorial Name: Set Date and Time
  Description: This tutorial shows how to use SetDateTime() widget.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  uint32_t t0;
  int16_t year, month, day, h, m, s, wday;
  char buf[20];

  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Invoke widget */
  CleO.SetDateTime("DateTime", t0);

  /* Convert epoch time to date/time */
  CleO.EpochToDate(t0, year, month, day, h, m, s, wday);

  /* Start building a screen frame */
  CleO.Start();

  sprintf(buf, "Time: %02d:%02d:%02d", h, m, s);
  CleO.StringExt(FONT_MEDIUM, 400, 240, LIGHT_GREEN, MM, 0, 0, buf);

  sprintf(buf, "Date: %02d/%02d/%04d", day, month+1, year);
  CleO.StringExt(FONT_MEDIUM, 400, 240 + 60, LIGHT_GREEN, MM, 0, 0, buf);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
