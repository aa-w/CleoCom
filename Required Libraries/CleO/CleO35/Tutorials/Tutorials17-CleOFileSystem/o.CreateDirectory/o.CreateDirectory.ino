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
  Tutorial Name: Directory Creation and Delete commands
  Description: This tutorial shows how to use create/delete directory
*/
#include <SPI.h>
#include <CleO.h>

void setup() {

  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 160, 50, LIGHT_GREEN, TM, 0, 0,
  "Creating TestDir...");
  /* Create directory TestDir in eFlash */
  CleO.FMKDir("@TestDir");
  
  /* Confirm if the directory is created successfully */
  if(CleO.FStat("@TestDir", NULL) == 1)
  {
    CleO.StringExt(FONT_MEDIUM, 160, 90, LIGHT_GREEN, TM, 0, 0,
    "Create OK");  
  }
  else
  {
    CleO.StringExt(FONT_MEDIUM, 160, 90, LIGHT_GREEN, TM, 0, 0,
    "Create FAILED");   
  }
  
  CleO.StringExt(FONT_MEDIUM, 160, 140, LIGHT_GREEN, TM, 0, 0,
  "Deleting TestDir..."); 
  /* Delete the directory */
  CleO.FUnLink("@TestDir");
  if(CleO.FStat("@TestDir", NULL) == 0)
  {
    CleO.StringExt(FONT_MEDIUM, 160, 180, LIGHT_GREEN, TM, 0, 0,
    "Delete OK");  
  }
  else
  {
    CleO.StringExt(FONT_MEDIUM, 160, 180, LIGHT_GREEN, TM, 0, 0,
    "Delete FAILED");   
  }  
  
  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
