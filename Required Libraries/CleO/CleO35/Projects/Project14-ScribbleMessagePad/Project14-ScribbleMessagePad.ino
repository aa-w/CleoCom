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
    Project14-ScribbleMessagePad.ino
  Purpose:              
    To desmonstrate how to create a message pad with clear/save functionality.
  External component:   
    None
  Hardware Connection:
    None
*/

#include <SPI.h>
#include <CleO.h>

void setup() {
  /* Initialize CleO - needs to be done only once */
  CleO.begin();
}

void loop() {
  /* Start building a screen frame */
  CleO.Start();

  /* Title */
  CleO.StringExt(FONT_MEDIUM, 160, 0, LIGHT_GREEN, TM, 0, 0, "SKETCH PAD");

  /* Draw a rectangle area for sketch */
  CleO.RectangleExt(0, 50, 320, 350, LIGHTYELLOW1, TL, 0, 0);
  CleO.BitmapColor(BLUE);
  CleO.Sketch(0, 50, 320, 350);

  CleO.LineWidth(30);

  /* draw CLEAR button */
  CleO.Tag(1);
  CleO.LineColor(WHITE);
  CleO.Line(50 + 1, 440 + 1, 125 + 1, 440 + 1);
  CleO.LineColor(RED);
  CleO.Line(50, 440, 125, 440);
  CleO.StringExt(FONT_MEDIUM, 88, 440, WHITE, MM, 0, 0, "CLEAR");

  /* draw SAVE button */
  CleO.Tag(2);
  CleO.LineColor(WHITE);
  CleO.Line(200 + 1, 440 + 1, 275 + 1, 440 + 1);
  CleO.LineColor(RED);
  CleO.Line(200, 440, 275, 440);
  CleO.StringExt(FONT_MEDIUM, 238, 440, WHITE, MM, 0, 0, "SAVE");

  /* Display completed screen frame */
  CleO.Show();

  /* wait until clear or save button is pressed */
  int16_t x, y, dur, tag, newval;
  do {
    CleO.TouchCoordinates(x, y, dur, tag);
    if (tag == 1 || tag == 2)
      break;
  } while (true);

  /* Do not perform any action as new frame will erase the message pad */
  if (tag == 1) {}


  /* Capture the message pad screen area */
  if (tag == 2)
    CleO.CaptureScreen("@Pictures/sketch.png", 0, 50, 320, 350);
}