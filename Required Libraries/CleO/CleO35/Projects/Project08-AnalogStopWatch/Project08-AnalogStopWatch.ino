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
    Project08-AnalogStopWatch.ino
  Purpose:              
    To desmonstrate how to use the CleO primitives to create an analog stopwatch
  External component:   
    None
  Hardware Connection:
    None
*/

#include <SPI.h>
#include <CleO.h>

#define SCREEN_WIDTH           320
#define SCREEN_HEIGHT          480
#define CENTER_X               (0.5 * SCREEN_WIDTH)

#define TIMER_RADIUS           100
#define TIMER_CENTER_Y         (0.30 * SCREEN_HEIGHT)

#define LAST_LAP_LINE_HEIGHT   30
#define TOTAL_LAST_LAPS        5

/* Holds current timer, saved current timers and laps */
uint32_t currentTimer = 0, lastCurrentTimers[TOTAL_LAST_LAPS], lastLaps[TOTAL_LAST_LAPS];
int recordedLaps = 0;
int16_t previousTag = 0, previousDur = 0;
char timerString[15], buffer[15];
uint32_t first_start_time = 0, pauseTime = 0, start_pause_time = 0, end_pause_time = 0;

/* Initial button text and their status */
uint32_t startButtonTextColor = GREEN, lapButtonTextColor = BLACK;
char startButtonText[6] = "Start", lapButtonText[6] = "Lap";
boolean startPressed = false, lapPressed = false, firstStart = false;

void setup() {
  /* Initialize CleO */
  CleO.begin();
  Serial.begin(112500);
}

void loop() {
  touchControl();
  drawWatchScreen();
  updateTimers();
}

void onLapButtonPress() {
  lapPressed = true;
  updateLastLapTime();
  drawLastLaps();
}

void onResetButtonPress() {
  lapPressed = false;
  lapButtonTextColor = LIGHT_GREY;
  sprintf(lapButtonText, "%s", "Lap");

  resetTimers();
}

void onStopButtonPress() {
  startButtonTextColor = GREEN;
  sprintf(startButtonText, "%s", "Start");
  sprintf(lapButtonText, "%s", "Reset");

  start_pause_time = millis();
}

void onStartButtonPress() {
  startButtonTextColor = RED;
  lapButtonTextColor = BLACK;
  sprintf(startButtonText, "%s", "Stop");

  if (strcmp(lapButtonText, "Reset") == 0)
    sprintf(lapButtonText, "%s", "Lap");

  if (firstStart == false) {
    firstStart = true;
    first_start_time = millis();
  }
  else {
    end_pause_time = millis();
    if (start_pause_time != 0)
      pauseTime += end_pause_time - start_pause_time;
  }
}

void processTags(int16_t tag) {
  if (tag == 1) {
    if (startPressed == false)
      onStartButtonPress();
    else
      onStopButtonPress();

    startPressed = !startPressed;
  } else if (tag == 2) {
    if (startPressed == true)
      onLapButtonPress();
    else
      onResetButtonPress();
  }
}

void touchControl() {
  int16_t x, y, dur, tag;
  CleO.TouchCoordinates(x, y, dur, tag);

  /* pen down */
  if (previousDur == 0 && dur != 0) {
    previousTag = tag;
    previousDur = 1;
  }
  /* pen up */
  else if (previousDur == 1 && dur == 0) {
    processTags(previousTag);
    previousDur = 0;
  }
}

void drawWatchScreen() {
  /* Start drawing the screen shot */
  CleO.Start();

  drawTimer();

  /* Draw lap area using lines and rectangle */
  CleO.RectangleExt(0, (0.60 * SCREEN_HEIGHT), SCREEN_WIDTH, SCREEN_HEIGHT, WHITE, TL, 0, 0);
  drawLastLapsLines();

  /* Draws button arean using rectangle */
  drawButtonsArea();

  /* Selects curves at the corner for the rectangle */
  CleO.RectangleCorner((0.08 * SCREEN_HEIGHT) / 2);
  
  double button_width =  0.42 * SCREEN_WIDTH;
  double button_height = (0.08 * SCREEN_HEIGHT);
  CleO.Tag(1);
  CleO.RectangleExt(CENTER_X - button_width/2 - 5, (0.95 * SCREEN_HEIGHT),
                    button_width, button_height, WHITE, MM, 0, 0);
    /* Draw start/stop button using rectangles with corner and string */
  CleO.StringExt(FONT_SANS_2, (0.30 * SCREEN_WIDTH),
                 (0.95 * SCREEN_HEIGHT), startButtonTextColor, MM, 0, 0, startButtonText);
                                   
  CleO.Tag(2);
  CleO.RectangleExt(CENTER_X + button_width/2 + 5, (0.95 * SCREEN_HEIGHT),
                    button_width, button_height, WHITE, MM, 0, 0);                  
  CleO.StringExt(FONT_SANS_2, (0.70 * SCREEN_WIDTH),
                 (0.95 * SCREEN_HEIGHT), lapButtonTextColor, MM, 0, 0, lapButtonText);

  CleO.Tag(3);
  /* Draw last laps only if lap button was pressed */
  if (lapPressed == true)
    drawLastLaps();

  /* Show the screenshot */
  CleO.Show();
}

void drawButtonsArea() {
  CleO.RectangleExt(0, (0.90 * SCREEN_HEIGHT), SCREEN_WIDTH, (0.10 * SCREEN_HEIGHT), LIGHT_GREY, TL, 0, 0);

  /* Selects curves at the corner for the rectangle */
  CleO.RectangleCorner((0.08 * SCREEN_HEIGHT) / 2);
  CleO.RectangleExt(CENTER_X, (0.95 * SCREEN_HEIGHT),
                    (0.90 * SCREEN_WIDTH), (0.08 * SCREEN_HEIGHT), WHITE, MM, 0, 0);
}

void drawTimer() {
  CleO.RectangleExt(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x237ead, TL, 0, 0);
  CleO.StringExt(FONT_SANS_2, CENTER_X, 10, WHITE, MM, 0, 0, "Stopwatch");

  /* Draw outter circle */
  CleO.CircleExt(CENTER_X, TIMER_CENTER_Y, TIMER_RADIUS - 5, TIMER_RADIUS, WHITE, MM, 0, 0);

  /* Draw inner circle */
  CleO.CircleExt(CENTER_X, TIMER_CENTER_Y + (TIMER_RADIUS / 2) + 10, 17, 20, WHITE, MM, 0, 0);

  /* Draw inner most smaller circle */
  CleO.CircleExt(CENTER_X, TIMER_CENTER_Y + (TIMER_RADIUS / 2) + 10, 2, 5, WHITE, MM, 0, 0);

  int16_t msecAngle = 180 + (currentTimer * 360L) / 1000;
  int16_t secAngle = 180 + (currentTimer * 360L) / (60 * 1000L);

  /* Draw needle based on the angle calculated from milliseconds */
  CleO.NeedleColor(BLACK);
  CleO.NeedleXY(CENTER_X, TIMER_CENTER_Y + (TIMER_RADIUS / 2) + 10, 20, msecAngle);

  CleO.LineColor(WHITE);
  CleO.LineR1R2(CENTER_X, TIMER_CENTER_Y, TIMER_RADIUS - 15, TIMER_RADIUS - 5, secAngle);

  CleO.PointExt(CENTER_X, TIMER_CENTER_Y, 4, WHITE, secAngle, TIMER_RADIUS - 15);
  CleO.PointExt(CENTER_X, TIMER_CENTER_Y, 2, BLACK, secAngle, TIMER_RADIUS - 15);

  CleO.StringExt(FONT_SANS_4, CENTER_X, TIMER_CENTER_Y, WHITE, MM, 0, 0, timerString);
}

void drawLastLapsLines() {
  CleO.LineColor(GREY);
  for (int i = 1; i <= 4; i++)
    CleO.Line(5, (0.60 * SCREEN_HEIGHT) + (i * LAST_LAP_LINE_HEIGHT), 0.98 * SCREEN_WIDTH,
              (0.60 * SCREEN_HEIGHT) + (i * LAST_LAP_LINE_HEIGHT));
}

void drawLastLaps() {
  int lapPosition = TOTAL_LAST_LAPS - 1;
  int totalStoredLaps = recordedLaps;
  int startY = (0.60 * SCREEN_HEIGHT) + (LAST_LAP_LINE_HEIGHT / 2);

  if (totalStoredLaps > 99)
    recordedLaps = 1;

  /* For every lap to be drawn */
  for (int lapCount = 0; lapCount < TOTAL_LAST_LAPS; lapCount++) {
    if (totalStoredLaps == 0)
      break;

    /* Draw lap count and "Lap" text */
    if (totalStoredLaps < 10)
      sprintf(buffer, "0%d", totalStoredLaps);
    else
      sprintf(buffer, "%d", totalStoredLaps);

    CleO.StringExt(FONT_SANS_2, 5, startY, BLACK, ML, 0, 0, buffer);
    CleO.StringExt(FONT_SANS_0, 5 + 30, startY, BLACK, BL, 0, 0, "Lap");

    /* Draw current time string */
    updateTimeString(lastCurrentTimers[lapPosition]);
    CleO.StringExt(FONT_SANS_2, CENTER_X, startY, BLACK, MM, 0, 0, buffer);

    /* Draw last lap time string */
    updateTimeString(lastLaps[lapPosition]);
    CleO.StringExt(FONT_SANS_0, 0.98 * SCREEN_WIDTH, startY, BLACK, MR, 0, 0, buffer);

    totalStoredLaps--;
    lapPosition--;

    /* Update y coordinate to used by next lap line */
    startY += LAST_LAP_LINE_HEIGHT;
  }
}

/* Store latest timer reading into last index */
void updateLastLapTime() {
  int i;
  for (i = 0; i < TOTAL_LAST_LAPS - 1; i++)
    lastCurrentTimers[i] = lastCurrentTimers[i + 1];

  lastCurrentTimers[i] = currentTimer;

  for (i = 0; i < TOTAL_LAST_LAPS - 1; i++)
    lastLaps[i] = lastLaps[i + 1];

  lastLaps[i] = lastCurrentTimers[TOTAL_LAST_LAPS - 1] - lastCurrentTimers[TOTAL_LAST_LAPS - 2];
  recordedLaps++;
}

void resetTimers() {
  firstStart = false;
  pauseTime = 0;
  currentTimer = 0;
  recordedLaps = 0;

  for (int i = 0; i < TOTAL_LAST_LAPS; i++) {
    lastLaps[i] = 0;
    lastCurrentTimers[i] = 0;
  }
}

void updateTimers() {
  /* Calculate elapsed time and also consider pause time */
  if (startPressed)
    currentTimer = millis() - first_start_time - pauseTime;

  int32_t sec = currentTimer / 1000;
  int32_t minute = sec / 60;
  int32_t hour = minute / 60;

  /* Prepare timer string to be displayed */
  sprintf(timerString, "%02d:%02d:%02d.%02d", (int)hour,
          (int)(minute % 60L), (int)(sec % 60L), (int)((currentTimer % 1000L) / 10));
}

void updateTimeString(int32_t msec) {
  int32_t sec = msec / 1000;
  int32_t minute = sec / 60;
  int32_t hour = minute / 60;
  sprintf(buffer, "%02d:%02d:%02d", (int)(hour), (int)(minute % 60L), (int)(sec % 60L));
}
