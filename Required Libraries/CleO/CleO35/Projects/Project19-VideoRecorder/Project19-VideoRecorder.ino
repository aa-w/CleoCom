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


/**
  Sketch:              
    Project19-VideoRecorder.ino
  Purpose:              
    To show the usage of the built-in still camera using the command RecorderPreview()
  External component:   
    CleO Camera module
  Hardware Connection:
    Connect CleO Camera to CleO35 CN3 connector
    
*/

#include <SPI.h>
#include <CleO.h>

void setup() {
  /* Initialize CleO */
  CleO.begin();
}

void loop() {
  int16_t x, y, dur, tag;

  /* Start drawing preview message */
  CleO.Start();

  /* Draw message */
  CleO.StringExt(FONT_MEDIUM, 160, 240, LIGHT_GREEN, MM, 0, 0, "Video Preview");

  /* Display screen frame */
  CleO.Show();

  delay(1000);

  /* Display recoder */
  CleO.RecorderPreview();
}