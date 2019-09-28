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
    Project10-ADigitalVoltmeter.ino
  Purpose:              
    To desmonstrate how to display a Rotary Pot IO reading using the seven segment font.
  External component:   
    Rotary Pot IO
  Hardware Connection:
    Rotary Pot      NerO
    VCC             IOREF
    GND             GND
    OUT             A0
*/

#include <SPI.h>
#include <CleO.h>

#define LED_RED 0xff4040UL

int16_t font;

void setup() {
  CleO.begin();
  font = CleO.LoadFont("@Fonts/DSEG7ClassicMini-BoldItalic.ftfont");
}

void loop() {
  int analog = analogRead(0);     // sample analog pin A0
  int mv = 5000L * analog / 1023; // rescale read value in range 0-1023 to millivolts in range 0-5000
  char buf[9];

  sprintf(buf, "%d. %03d", mv / 1000, mv % 1000);
  CleO.Start();
  CleO.StringExt(font, 400, 240 , LED_RED, MM, 0, 0, buf);
  CleO.Show();
}
