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
  Tutorial Name: Creating Basic Sounds
  Description: This tutorial shows how to create basic sounds.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();
}

void loop()
{
  /* select Sound, pitch, volume and duration */
  CleO.SetSound(CHIMES, 60, 255, 0);

  /* play the sound */
  CleO.SoundPlay(1);

  delay(2000);
}