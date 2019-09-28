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
    Project12-ASpeedometerDisplay.ino
  Purpose:              
    To desmonstrate how to draw a speedometer using the CleO Graphics Primitive commands Bitmap() and Needle().
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

int16_t dial;

void setup() {
  CleO.begin();
  CleO.DisplayRotate(2, 0);
  dial = CleO.LoadImageFile("@Pictures/speedo.png", 0);
}

void loop() {
  int f = FONT_MEDIUM;            // Font for numbers
  int analog = analogRead(0);     // sample analog pin A0
                                  // CleO IO - Rotary Pot connects to CN11
  int angle = gauge_angle(analog);
  int cx = 240;                   // X center
  int cy = 160;                   // Y Center

  CleO.Start();
  CleO.Bitmap(dial, 0, 0);

  // Red needle
  CleO.NeedleWidth(10);
  CleO.NeedleColor(RED);
  CleO.NeedleXY(cx, cy, 153, angle);

  CleO.Show();
}

// Rescale x from the range (x0, x1) to the range (y0, y1)
float rescale(int x, int x0, int x1, int y0, int y1) {
  x = max(x0, min(x1, x));
  return y0 + ((x - x0) * long(y1 - y0) / float(x1 - x0));
}

float gauge_angle(int x) {
  return rescale(x, 0, 1023, -2, 263);
}
