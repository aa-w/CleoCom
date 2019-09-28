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
  Tutorial Name: Playing Midi Sounds
  Description: This tutorial shows how to create sound from Midi
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
  /* select Sound, pitch and notes string */
  CleO.MPlay(PIANO, 400,
  "E5A5E5B5E5G5A5E5C6E5D6E5B5C6E5A5E5B5E5G5A5E5C6E5D6E5B5C6E5B5");

  delay(2000);
}
