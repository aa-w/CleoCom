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
  Tutorial Name: File read character Command
  Description: This tutorial shows how to read characters from file.
*/
#include <SPI.h>
#include <CleO.h>

void setup() {
  int g;
  uint8_t buf[3];
  char data[10];

  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 400, 100, LIGHT_GREEN, TM, 0, 0,
                 "Seeking ball.png...");


  for (size_t i = 0; i < 4; i++) {

    /* Open file in read mode */
    g = CleO.FOpen("@Pictures/ball.png", FILE_READ);

    /* Seek at ith position */
    CleO.FSeek(g, i);

    /* Read a character from seek position */
    buf[i] = CleO.FGetC(g);

    /* Close file */
    CleO.FClose(g);
  }

  sprintf(data, "%x %x %x", buf[0], buf[1], buf[2]);

  /* Display collected data */
  CleO.StringExt(FONT_MEDIUM, 400, 200, LIGHT_GREEN, TM, 0, 0, "Data from file:");
  CleO.StringExt(FONT_MEDIUM, 400, 300, LIGHT_GREEN, TM, 0, 0, data);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
