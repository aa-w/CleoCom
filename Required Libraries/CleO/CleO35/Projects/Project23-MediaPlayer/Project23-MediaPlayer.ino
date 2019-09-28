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
    Project23-MediaPlayer.ino
  Purpose:              
    To desmonstrate how to play/view media files
  External component:   
    CleO Camera module
  Hardware Connection:  
    Connect CleO Camera module to CleO35 CN3 connector
*/

#include <SPI.h>
#include <CleO.h>

void setup() {
  /* Initialize CleO - needs to be done only once */
  CleO.begin();
}

void loop() {
  /* Start building screen frame */
  CleO.Start();

  /* Show screen frame */
  CleO.Show();
  
  /* Display recoder */
  CleO.PlayerPlay("/Videos/NerO.avi");
}
