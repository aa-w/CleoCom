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
    Project09-AnalogTimer.ino
  Purpose:              
    To desmonstrate how to create an analog stopwatch using the CleO primitives.
  External component:   
    None
  Hardware Connection:
    None
*/

#include <SPI.h>
#include <CleO.h>

#define START_PAUSE_BUTTON_TAG                1
#define CANCEL_BUTTON_TAG                     2
#define TIMER_TAG                             3

#define SCREEN_WIDTH           480
#define SCREEN_HEIGHT          800
#define CENTRE_X              (0.5 * SCREEN_WIDTH)
#define CENTRE_Y              (0.5 * SCREEN_HEIGHT)
#define RADIUS                (SCREEN_WIDTH/2 - 30)
#define CLOCK_X               (CENTRE_X)
#define CLOCK_Y               (CENTRE_Y)
#define BUTTON_X              (CENTRE_X)

/* Holds current timer value */
uint32_t currentTimer;

/* Variables to hold button press state, text, color and
  display string for the current timer */
uint32_t startButtonTextColor = GREEN, CancelButtonTextColor = BLACK;
char startButtonText[10] = "Set Timer", CancelButtonText[10] = "Cancel";
boolean startPressed = false, CancelPressed = false;
int16_t previousDur = 0, previousTag = 0;
uint32_t target, start_pause_time = 0, end_pause_time = 0, pauseTime = 0;
char timerString[15];

void setup() {
  Serial.begin(112500);
  CleO.begin();
  /* Set screen coordinate in portrait mode */
  CleO.DisplayRotate(2, 0);
}

void loop() {
  touchControl();
  drawWatchScreen();
  updateTimer();
}

void set_timer() {
  uint32_t t0 = 0;

  /* Set screen coordinate in landscape mode */
  CleO.DisplayRotate(1, 0);

  CleO.SetDateTime("DateTime", t0);

  /* Set screen coordinate in portrait mode */
  CleO.DisplayRotate(2, 0);

  /* Convert epoch time to year, month, day, hour, min and sec */
  int16_t year = 0, month = 0, day = 0, h = 0, m = 0, s = 0, wday = 0;
  CleO.EpochToDate(t0, year, month, day, h, m, s, wday);

  /* Set 'target' to the moment in the future when the timer should expire */
  target = millis() + 1000 * (h * 3600L + m * 60L + s * 1L);
}

void onCancelButtonPress() {
  /* Change the color and text of the start and cancel button */
  CancelPressed = true;
  CancelButtonTextColor = LIGHT_GREY;
  startPressed = false;
  sprintf(startButtonText, "%s", "Set Timer");
  startButtonTextColor = GREEN;

  /* Reset timer and  target */
  target = 0;
  currentTimer = 0;
}

void onPauseButtonPress() {
  startButtonTextColor = GREEN;
  sprintf(startButtonText, "%s", "Continue");
  start_pause_time = millis();
}

void onSetTimerButtonPress() {
  set_timer();
  startButtonTextColor = RED;
  CancelButtonTextColor = BLACK;
  sprintf(startButtonText, "%s", "Pause");
}

void onStartButtonPress() {
  /* Changes the color and text of the start button */
  startButtonTextColor = RED;
  CancelButtonTextColor = BLACK;
  sprintf(startButtonText, "%s", "Pause");

  end_pause_time = millis();
  pauseTime += end_pause_time - start_pause_time;
}

void processTags(int16_t tag) {
  /* On start/pause button press */
  if (tag == START_PAUSE_BUTTON_TAG) {
    if (isTimeOut() == false) {
      /* Start button was not press earlier */
      if (startPressed == false)
        onStartButtonPress();
      else
        onPauseButtonPress();

      /* Alternate button press event */
      startPressed = !startPressed;
    } else {
      onSetTimerButtonPress();
      startPressed = true;
    }
  } else if (tag == CANCEL_BUTTON_TAG) {
    if (startPressed == true || isTimeOut() == false)
      onCancelButtonPress();
  }
}

void touchControl() {
  int16_t x, y, dur, tag;
  CleO.TouchCoordinates(x, y, dur, tag);

  /* pen down */
  if (previousDur == 0 && dur != 0) {
    previousTag = tag; previousDur = 1;
  }
  /* pen up */
  else if (previousDur == 1 && dur == 0) {
    processTags(previousTag); previousDur = 0;
  }
}

void drawWatchScreen() {
  CleO.Start();

  /* Draw timer using circle, needle, line, point */
  drawTimer();

  /* Draws rectangle button area at the bottom of the screen */
  CleO.RectangleCorner((0.08 * SCREEN_HEIGHT) / 2);

  double button_width =  0.42 * SCREEN_WIDTH;
  double button_height = (0.08 * SCREEN_HEIGHT);
  
  /* Assign touch tag for stat/stop button */
  CleO.Tag(START_PAUSE_BUTTON_TAG);
  CleO.RectangleExt(CENTRE_X - button_width/2 - 5, (0.95 * SCREEN_HEIGHT),
                    button_width, button_height, WHITE, MM, 0, 0);
  CleO.StringExt(FONT_SANS_4, BUTTON_X - 100,
                 SCREEN_HEIGHT - 40, startButtonTextColor, MM, 0, 0, startButtonText);

  /* Assign touch tag for cancel/reset button */
  CleO.Tag(CANCEL_BUTTON_TAG);
  CleO.RectangleExt(CENTRE_X + button_width/2 + 5, (0.95 * SCREEN_HEIGHT),
                    button_width, button_height, WHITE, MM, 0, 0);
  CleO.StringExt(FONT_SANS_4, BUTTON_X + 100,
                 SCREEN_HEIGHT - 40, CancelButtonTextColor, MM, 0, 0, CancelButtonText);

  CleO.Show();
}

void drawTimer() {
  /* Display rectangle button with round corner */
  CleO.RectangleExt(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x237ead, TL, 0, 0);
  CleO.StringExt(FONT_SANS_6, BUTTON_X, 80, WHITE, MM, 0, 0, "Timer");

  int16_t msecAngle = 180 + (currentTimer * 360L) / 1000;
  int16_t minAngle = 180 + (currentTimer * 360L) / (60 * 1000L);

  /* Display main timer circle and minute ticking hand */
  CleO.CircleExt(CLOCK_X, CLOCK_Y, RADIUS + 19, RADIUS + 20, WHITE, MM, 0, 0);
  CleO.CircleExt(CLOCK_X, CLOCK_Y, RADIUS - 5, RADIUS, WHITE, MM, 0, 0);
  CleO.LineColor(WHITE);
  CleO.LineR1R2(CLOCK_X, CLOCK_Y, RADIUS - 15, RADIUS - 5, minAngle);
  CleO.PointExt(CLOCK_X, CLOCK_Y, 4, WHITE, minAngle, RADIUS - 15);
  CleO.PointExt(CLOCK_X, CLOCK_Y, 2, BLACK, minAngle, RADIUS - 15);

  /* Display inner circle and needle */
  CleO.CircleExt(CLOCK_X, CLOCK_Y + (RADIUS / 2) + 10, 17, 20, WHITE, MM, 0, 0);
  CleO.CircleExt(CLOCK_X, CLOCK_Y + (RADIUS / 2) + 10, 2, 5, WHITE, MM, 0, 0);
  CleO.NeedleColor(BLACK);
  CleO.NeedleXY(CLOCK_X, CLOCK_Y + (RADIUS / 2) + 10, 20, msecAngle);

  /* Display current timer */
  CleO.StringExt(FONT_SANS_6, CLOCK_X, CLOCK_Y, WHITE, MM, 0, 0, timerString);
}

boolean isTimeOut() {
  return (target < millis());
}

void updateTimer() {
  /* Calculate elapsed time and also consider pause time */
  if (startPressed)
    currentTimer = max(0, uint32_t(target - millis() + pauseTime));

  int32_t sec = currentTimer / 1000;
  int32_t minute = sec / 60;
  int32_t hour = minute / 60;

  /* Prepare timer string to be displayed */
  sprintf(timerString, "%02d:%02d:%02d.%02d", (int)hour,
          (int)(minute % 60L), (int)(sec % 60L),
          (int)((currentTimer % 1000L) / 10));

  if (isTimeOut() == true && startPressed == true)
    onCancelButtonPress();
}
