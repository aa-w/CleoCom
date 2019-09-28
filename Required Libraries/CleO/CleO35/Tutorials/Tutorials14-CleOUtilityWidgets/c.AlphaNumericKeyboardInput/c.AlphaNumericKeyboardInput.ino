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
  Tutorial Name: Alphanumeric Keypad
  Description: This tutorial shows how to use Keyboard() widget.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Max allowed input length is 16 */
  textfield AlphaNumeric = { 16, "" };

  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Draw Keypad */
  char buf[20];
  sprintf(buf, "Keyboard - %d chars", AlphaNumeric.size);
  int16_t retcode = CleO.Keyboard(buf, AlphaNumeric);
  if (retcode == 1) // Cancel pressed
    strcpy(AlphaNumeric.text, ""); // erase text

  /* Start building a screen frame */
  CleO.Start();

  CleO.StringExt(FONT_MEDIUM, 160, 240, LIGHT_GREEN, MM, 0, 0,
  AlphaNumeric.text);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}
