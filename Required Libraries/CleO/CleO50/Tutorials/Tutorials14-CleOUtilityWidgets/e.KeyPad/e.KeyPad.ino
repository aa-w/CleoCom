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
  Tutorial Name: Keypad
  Description: This tutorial shows how to use KeyPad() widget.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  int32_t numeric = 1234;

  /* Draw Keypad */
  int16_t retcode = CleO.KeyPad("Enter PIN", numeric);

  String displayText = String(retcode);
  char buffer[10];
  displayText.toCharArray(buffer, 10);

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 400, 240, LIGHT_GREEN, MM, 0, 0, buffer);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
