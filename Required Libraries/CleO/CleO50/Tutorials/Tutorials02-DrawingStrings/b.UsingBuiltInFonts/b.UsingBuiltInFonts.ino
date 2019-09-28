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
Tutorial Name: Using Built-in Fonts
Description:
This tutorial shows how to use built-in fonts.
*/
#include <SPI.h>
#include <CleO.h>

void setup()
{
    int screenWidth = 800, screenheight = 480;

    /* Initialize CleO */
    CleO.begin();

    /* Start building a screen frame */
    CleO.Start();

    /* Draw string (digits) using built-in font */
    CleO.StringExt(FONT_TINY, screenWidth / 2, 60,
    0xff4040UL, MM, 0, 0, "Tiny");

    CleO.StringExt(FONT_SANS_2, screenWidth / 2, 150,
    0xff4040UL, MM, 0, 0, "Small");

    CleO.StringExt(FONT_MEDIUM, screenWidth / 2, screenheight / 2,
    0xff4040UL, MM, 0, 0, "Medium");

    CleO.StringExt(FONT_SANS_6, screenWidth / 2, 320,
    0xff4040UL, MM, 0, 0, "Large");

    CleO.StringExt(FONT_HUGE, screenWidth / 2, 425,
    0xff4040UL, MM, 0, 0, "Huge");

    /* Display completed screen frame */
    CleO.Show();
}

void loop() {}