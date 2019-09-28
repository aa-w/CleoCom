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
  Tutorial Name: File Read Command
  Description: This tutorial shows how to read from file.
*/
#include <SPI.h>
#include <CleO.h>

void setup() {
  char STR1[] = "ABCDEFG";
  int16_t actual;
  char buf[30];
  uint32_t u32;

  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 400, 50, LIGHT_GREEN, TM, 0, 0,
  "Opening ball.png...");
  /* Open ball.png file in read mode */
  int16_t g = CleO.FOpen("@Pictures/ball.png", FILE_READ);

  /* request for file size */
  CleO.FSize(g, u32);
  sprintf(buf, "File size:%d bytes", u32);
  CleO.StringExt(FONT_MEDIUM, 400, 100, LIGHT_GREEN, TM, 0, 0, buf);

  /* request for file pointer location */
  CleO.FTell(g, u32);
  sprintf(buf, "File pointer at:%d", u32);
  CleO.StringExt(FONT_MEDIUM, 400, 150, LIGHT_GREEN, TM, 0, 0, buf);


  CleO.StringExt(FONT_MEDIUM, 400, 200, LIGHT_GREEN, TM, 0, 0,
  "Reading file...");
  uint8_t buf1[5];  char buf2[5];
  /* perform file read */
  CleO.FRead(g, buf1, 4, actual);
  sprintf(buf2, "%x %x %x %x", buf1[0], buf1[1], buf1[2], buf1[3]);
  CleO.StringExt(FONT_MEDIUM, 400, 250, LIGHT_GREEN, TM, 0, 0, (char *) buf2);

  /* print number of actual bytes read */
  sprintf(buf, "Bytes read:%d", actual);
  CleO.StringExt(FONT_MEDIUM, 400, 300, LIGHT_GREEN, TM, 0, 0, buf);

  /* request for file pointer location */
  CleO.FTell(g, u32);
  sprintf(buf, "File pointer at:%d", u32);
  CleO.StringExt(FONT_MEDIUM, 400, 350, LIGHT_GREEN, TM, 0, 0, buf);

  /* check for end of file */
  if (CleO.FEOF(g))
    CleO.StringExt(FONT_MEDIUM, 400, 400, LIGHT_GREEN, TM, 0, 0, "EOF reached");

  /* close file using file handle */
  CleO.FClose(g);
  CleO.StringExt(FONT_MEDIUM, 400, 450, LIGHT_GREEN, TM, 0, 0, "File closed");

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
