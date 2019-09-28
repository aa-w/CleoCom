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
  Tutorial Name: Read Directory commands
  Description: This tutorial shows how to use read directory
*/
#include <SPI.h>
#include <CleO.h>

void setup() {
  int16_t handle;
  file_info_t obj_info;   //to store information of items belong to a directory
  
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 400, 50, LIGHT_GREEN, TM, 0, 0,
  "Creating TestDir...");
  /* Create directory TestDir in eFlash */
  CleO.FMKDir("@TestDir");
  
  /* Confirm if the directory is created successfully */
  if(CleO.FStat("@TestDir", NULL) == 1)
  {
    CleO.StringExt(FONT_MEDIUM, 400, 90, LIGHT_GREEN, TM, 0, 0,
    "Create OK");  
  }
  else
  {
    CleO.StringExt(FONT_MEDIUM, 400, 90, LIGHT_GREEN, TM, 0, 0,
    "Create FAILED");   
  }
  CleO.StringExt(FONT_MEDIUM, 400, 130, LIGHT_GREEN, TM, 0, 0,
  "Read TestDir...");  
  
  /* Create some files and some subdirs inside the top dir */
  handle = CleO.FOpen("@TestDir/testfile_1.txt", FILE_CREATE_ALWAYS | FILE_WRITE);
  CleO.FClose(handle);
  handle = CleO.FOpen("@TestDir/testfile_2.txt", FILE_CREATE_ALWAYS | FILE_WRITE);
  CleO.FClose(handle);
  CleO.FMKDir("@TestDir/subdir_1");
  CleO.FMKDir("@TestDir/subdir_2");  
  
  /* Open top dir to read */
  uint8_t buf[30];
  handle = CleO.FOpenDir("@TestDir");
  for(int i = 0; i < 4; i++){
    CleO.FReadDir(handle, (uint8_t *)&obj_info, sizeof(file_info_t));
    /*If it is a file */
    if(obj_info.type == CLEO_FILE){
      sprintf((char *)buf, "File: %s", obj_info.filename);
    }
    /*If it is a sub directory */
    else if (obj_info.type == CLEO_DIR) {
      sprintf((char *)buf, "Subdir: %s", obj_info.filename);
    }
    /*If there are no files/dirs to read */
    else {
      break;
    }
    CleO.StringExt(FONT_MEDIUM, 400, 170 + 40*i, LIGHT_GREEN, TM, 0, 0, (char *)buf);
  }
  
  /*Close directory handle*/
  CleO.FCloseDir(handle);
  
  /* Delete the top dir */
  CleO.FUnLink("@TestDir/subdir_1");
  CleO.FUnLink("@TestDir/subdir_2");
  CleO.FUnLink("@TestDir/testfile_1.txt");
  CleO.FUnLink("@TestDir/testfile_2.txt");   
  CleO.FUnLink("@TestDir");
  
  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
