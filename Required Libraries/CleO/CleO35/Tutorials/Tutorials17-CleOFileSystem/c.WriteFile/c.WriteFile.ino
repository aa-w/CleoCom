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
  Tutorial Name: File Write Command
  Description: This tutorial shows how to write to file.
*/
#include <SPI.h>
#include <CleO.h>

void setup() {
  char STR1[] = "ABCDEFG";
  int16_t actual;

  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 160, 100, LIGHT_GREEN, TM, 0, 0,
  "Creating test.txt...");
  /* open file for write */
  int16_t f = CleO.FOpen("@test.txt", FILE_CREATE_ALWAYS | FILE_WRITE);

  CleO.StringExt(FONT_MEDIUM, 160, 240, LIGHT_GREEN, TM, 0, 0,
  "Writing ABCDEFG...");
  /* write into file */
  CleO.FWrite(f, strlen(STR1) + 1, (uint8_t*)STR1, actual);

  /* print number of bytes written */
  char buf[30];
  sprintf(buf, "%d bytes written", actual);
  CleO.StringExt(FONT_MEDIUM, 160, 380, LIGHT_GREEN, TM, 0, 0, buf);

  /* close file using file handle */
  CleO.FClose(f);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
