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
  Tutorial Name: Scissor Function
  Description:
  This tutorial shows how to use CleO.Scissor()
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Get bitmap handle */
  int16_t handle = CleO.LoadImageFile("@Pictures/ctg.jpg", 0);

  /* Start building a screen frame */
  CleO.Start();

  /* Display original bitmap */
  CleO.Bitmap(handle, 0, 0);

  /* Select the scissor area */
  CleO.Scissor(200, 290, 100, 130);

  /* Display bitmap - only area under scissor will get displayed */
  CleO.Bitmap(handle, 130, 250);

  /* Display completed screen frame */
  CleO.Show();
}

void loop() {}