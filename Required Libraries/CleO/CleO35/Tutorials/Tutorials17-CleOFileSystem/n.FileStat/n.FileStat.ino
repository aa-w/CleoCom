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
  Tutorial Name: File Stat Command
  Description: This tutorial shows how to check file/dir existance.
*/
#include <SPI.h>
#include <CleO.h>

void setup() {
  int file;
  char buf[9];
  int bytewritten;
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 160, 100, LIGHT_GREEN, TM, 0, 0,
  "Create TestFStat.txt...");
  file = CleO.FOpen("@TestFStat.txt", FILE_CREATE_ALWAYS | FILE_WRITE);
  CleO.FClose(file);
  
  //Confirm the file should exist
  if(CleO.FStat("@TestFStat.txt", NULL) == 1) {
    CleO.StringExt(FONT_MEDIUM, 160, 150, LIGHT_GREEN, TM, 0, 0,
    "TestFStat.txt OK");    
  } else {
    CleO.StringExt(FONT_MEDIUM, 160, 200, LIGHT_GREEN, TM, 0, 0,
    "TestFStat.txt FAILED");     
  }
  
  CleO.Show();
}

void loop() {}
