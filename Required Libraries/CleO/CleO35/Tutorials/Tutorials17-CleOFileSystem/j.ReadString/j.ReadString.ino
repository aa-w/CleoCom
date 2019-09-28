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
  Tutorial Name: File Read String Command
  Description: This tutorial shows how to read from file.
*/
#include <SPI.h>
#include <CleO.h>

void setup() {
  char STR1[] = "HelloWorld";

  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 160, 100, LIGHT_GREEN, TM, 0, 0,
                 "Creating TestGetS.txt...");
  /* open file for write */
  int16_t f = CleO.FOpen("@TestGetS.txt", FILE_CREATE_ALWAYS | FILE_WRITE);

  CleO.StringExt(FONT_MEDIUM, 160, 180, LIGHT_GREEN, TM, 0, 0,
                 "Writing HelloWorld...");
                 
  /* write one string into file */
  CleO.FPutS(f, STR1);
  /* close file using file handle */
  CleO.FClose(f);

  /* Open to read back */
  char buf[30] = {0};
  int16_t actual;
  f = CleO.FOpen("@TestGetS.txt", FILE_READ);
  CleO.FGetS(f, (uint8_t *)buf, 11);
  CleO.StringExt(FONT_MEDIUM, 160, 260, LIGHT_GREEN, TM, 0, 0,
                 "Read back: ");
  CleO.StringExt(FONT_MEDIUM, 160, 340, LIGHT_GREEN, TM, 0, 0, buf);
  /* close file using file handle */
  CleO.FClose(f);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}