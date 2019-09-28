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
    Project11-AnalogVoltmeter.ino
  Purpose:              
    To desmonstrate how to draw an analog voltmeter using the primitives - Needle, String, and Bitmap.
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

int16_t voltm, voltmc;

void setup() {
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  CleO.DisplayRotate(2, 0);
  voltm = CleO.LoadImageFile("@Pictures/voltm35.jpg", 0);
  voltmc = CleO.LoadImageFile("@Pictures/voltmc35.jpg", 0);
  CleO.SetBackgroundGradient(0, 0, 480, 320, LIGHT_BLUE, WHITE);
}

void loop() {
  int f = FONT_MEDIUM;            // Font for numbers
  int analog = analogRead(0);     // sample analog pin A0
                                  // CleO IO - Rotary Pot connects to CN11
  int angle = gauge_angle(analog);
  int cx = 240;                   // X center
  int cy = 344;                   // Y Center
  int r = 265;               // Gauge radius
  int lineLength;

  /* Start building a screen frame */
  CleO.Start();

  CleO.LineColor(BLACK);

  /* Draw arcs with 30 lines */
  for (int i = 0; i <= 30; i++)
  {
    if (i % 5 != 0) {
      lineLength = 20;
      CleO.LineWidth(1);
    } else {
      lineLength = 40;
      CleO.LineWidth(2);
    }

    CleO.LineR1R2(cx, cy, r + lineLength, r, 135 + i * 90.00 / 30);
  }

  /* Draw V logo with two lines below it */
  CleO.StringExt(FONT_HUGE, cx, cy - 175, BLACK, MM, 0, 0, "V");
  CleO.LineExt(cx - 28, cy - 130, 60, BLACK, 270, 0);
  CleO.LineExt(cx - 28, cy - 118, 12, BLACK, 270, 0);
  CleO.LineExt(cx - 5, cy - 118, 12, BLACK, 270, 0);
  CleO.LineExt(cx + 20, cy - 118, 12, BLACK, 270, 0);

  // 0v and 5v markers
  CleO.StringExt(f, cx, cy, BLACK, MM, gauge_angle(0), r + 60, "0");
  CleO.StringExt(f, cx, cy, BLACK, MM, gauge_angle(1023), r + 60, "5");

  /* Select the scissor area */
  CleO.Scissor(0, 250, 480, 320);

  // Gauge background plate
  CleO.Bitmap(voltm, 0, 0);

  /* Select the scissor area */
  CleO.Scissor(0, 0, 480, 320);

  // Shadow
  CleO.NeedleWidth(15);
  CleO.NeedleColor(GRAY70);
  CleO.NeedleXY(cx, cy, r, angle);

  // Red needle
  CleO.NeedleWidth(12);
  CleO.NeedleColor(ORANGERED);
  CleO.NeedleXY(cx, cy, r, angle);

  // Pale red highlight
  CleO.NeedleWidth(5);
  CleO.NeedleColor(0xffc0c0);
  CleO.NeedleXY(cx, cy, r, angle);

  // Gauge cover plate
  CleO.BitmapJustification(BL);
  CleO.Bitmap(voltmc, 0, 320);

  /* Display completed screen frame */
  CleO.Show();
}

// Rescale x from the range (x0, x1) to the range (y0, y1)
float rescale(int x, int x0, int x1, int y0, int y1) {
  x = max(x0, min(x1, x));
  return y0 + ((x - x0) * long(y1 - y0) / float(x1 - x0));
}

float gauge_angle(int x) {
  return rescale(x, 0, 1023, 180 - 45, 180 + 44);
}
