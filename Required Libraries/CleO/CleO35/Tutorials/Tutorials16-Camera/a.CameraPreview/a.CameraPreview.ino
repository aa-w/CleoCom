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
    Tutorials16-CameraPreview.ino
  Purpose:              
    To desmonstrate using Camera command
*/

#include <SPI.h>
#include <CleO.h>

void setup() {
  Serial.begin(115200);
  CleO.begin();
  /* Display Camera Recorder*/
  CleO.RecorderPreview();
}

void loop() {}
