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
  Tutorial Name: Setting Gradient Screen Background
  Description: This tutorial shows how to gradient screen background.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
    int centerX = 800/2;

    /* Initialize CleO - needs to be done only once */
    CleO.begin();

    /* Start building a screen frame */
    CleO.Start();

    /* Select angle unit */
    CleO.SetAngleType(FURMANS);

    CleO.LineAngle(45);
    CleO.LineXY(400, 100, 60);

    /* Restore default context */
    CleO.SetDefaultContext();

    CleO.StringExt(FONT_MEDIUM, centerX, 300, WHITE, MM, 0, 0, "Default Context");

    /* 45 will be treated as degree instead of FURMANS */
    CleO.LineAngle(45);
    CleO.LineXY(400, 350, 60);

    /* Display completed screen frame */
    CleO.Show();
}

void loop() {}
