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

#define SCREEN_WIDTH          800
#define SCREEN_HEIGHT         480
#define CENTRE_X              (0.5 * SCREEN_WIDTH)
#define CENTRE_Y              (0.5 * SCREEN_HEIGHT)
#define BUTTON_X              (CENTRE_X + 225)
#define SKETCH_X              10
#define SKETCH_Y              10
#define SKETCH_WIDTH          450
#define SKETCH_HEIGHT         (SCREEN_HEIGHT - 50)
#define CLEAR_X               (BUTTON_X - 80)
#define SAVE_X                (BUTTON_X + 80)

void setup() {
  /* Initialize CleO - needs to be done only once */
  CleO.begin();
}

void loop() {
  /* Start building a screen frame */
  CleO.Start();

  /* Title */
  CleO.StringExt(FONT_SANS_5, BUTTON_X, CENTRE_Y - 100, LIGHT_GREEN, TM, 0, 0, "SKETCH PAD");

  /* Draw a rectangle area for sketch */
  CleO.RectangleExt(SKETCH_X, SKETCH_Y, SKETCH_WIDTH, SKETCH_HEIGHT, LIGHTYELLOW1, TL, 0, 0);
  CleO.BitmapColor(BLUE);
  CleO.Sketch(SKETCH_X, SKETCH_Y, SKETCH_WIDTH, SKETCH_HEIGHT);

  CleO.LineWidth(30);

  /* draw CLEAR button */
  CleO.Tag(1);
  CleO.LineColor(WHITE);
  CleO.Line(CLEAR_X - 45 + 1, CENTRE_Y + 100 + 1, CLEAR_X + 45 + 1, CENTRE_Y + 100 + 1);
  CleO.LineColor(RED);
  CleO.Line(CLEAR_X - 45, CENTRE_Y + 100, CLEAR_X + 45, CENTRE_Y + 100);
  CleO.StringExt(FONT_MEDIUM, CLEAR_X, CENTRE_Y + 100, WHITE, MM, 0, 0, "CLEAR");

  /* draw SAVE button */
  CleO.Tag(2);
  CleO.LineColor(WHITE);
  CleO.Line(SAVE_X - 45 + 1, CENTRE_Y + 100 + 1, SAVE_X + 45 + 1, CENTRE_Y + 100 + 1);
  CleO.LineColor(RED);
  CleO.Line(SAVE_X - 45, CENTRE_Y + 100, SAVE_X + 45, CENTRE_Y + 100);
  CleO.StringExt(FONT_MEDIUM, SAVE_X, CENTRE_Y + 100, WHITE, MM, 0, 0, "SAVE");

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
    CleO.CaptureScreen("@Pictures/sketch.png", SKETCH_X, SKETCH_Y, SKETCH_WIDTH, SKETCH_HEIGHT);
}
