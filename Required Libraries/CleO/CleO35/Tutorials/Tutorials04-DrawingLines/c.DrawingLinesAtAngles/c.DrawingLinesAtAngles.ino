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
Tutorial Name: Line Drawing with angles
Description: This tutorial shows how to draw a line with angle.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
    /* Initialize CleO - needs to be done only once */
    CleO.begin();

    /* Start building a screen frame */
    CleO.Start();

    /* select different line angles and draw line at (150, 240) with 150 length */
    CleO.LineAngle(60);
    CleO.LineXY(150, 240, 150);
    CleO.LineAngle(120);
    CleO.LineXY(150, 240, 150);
    CleO.LineAngle(180);
    CleO.LineXY(150, 240, 150);
    CleO.LineAngle(240);
    CleO.LineXY(150, 240, 150);
    CleO.LineAngle(300);
    CleO.LineXY(150, 240, 150);
    CleO.LineAngle(360);
    CleO.LineXY(150, 240, 150);

    /* Display completed screen frame */
    CleO.Show();
}

void loop()
{
}