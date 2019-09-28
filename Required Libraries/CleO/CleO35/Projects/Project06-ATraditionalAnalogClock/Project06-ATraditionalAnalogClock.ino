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
    Project06-ATraditionalAnalogClock.ino
  Purpose:              
    To desmonstrate how to draw a traditional analog clock using strings, needles and line segments
  External component:   
    PCF8523T RTC module
    CleO Speaker
  Hardware Connection:
    Connect CleO Speaker to CleO35 CN13 connector
    PCF8523     NerO      
    GND         GND      
    VDD         3.3V
    SDA         SDA
    SCL         SCL
    INT         Pin 7 of CN7
*/

#include <SPI.h>
#include <CleO.h>
#include <Wire.h>
#include <PCF8523.h>

#define SCREEN_WIDTH          320
#define SCREEN_HEIGHT         480
#define CENTRE_X              (0.5 * SCREEN_WIDTH)
#define CENTRE_Y              (0.5 * SCREEN_HEIGHT)
#define RADIUS                (SCREEN_WIDTH/2)

PCF8523 rtc;

bool software_timer = false;
unsigned long startTime, elapsedTime = 0; /* Used for calculating elapsed secs */
uint32_t t0 = 0; /* Holds epoch time for software timer */
int16_t icon_play[2], icon_setting;
int playing = 0;
int speak_hour, speak_min; /* Used while speaking time */
int16_t previousDur = 0, previousTag = 0;

void setup() {
  Serial.begin(115200);

  #ifdef _VARIANT_ARDUINO_DUE_X_
    Wire1.begin();  // Shield I2C pins connect to alt I2C bus on Arduino Due
  #else
    Wire.begin(); 
  #endif

  if (rtc.begin() == -1) // RTC is not detected
    software_timer = true;

  /* Initialize CleO */
  CleO.begin();

  int16_t s = 0, m = 0, h = 0, dayOfWeek = 0, dayOfMonth = 0, mon = 0, y = 0;
  if (software_timer == true) {
    CleO.SetDateTime("Date/Time", t0);
    CleO.EpochToDate(t0, y, mon, dayOfMonth, h, m, s, dayOfWeek);
  }
  else if (!rtc.isrunning() || rtc.checkOSF() == 1) {
    CleO.SetDateTime("Date/Time", t0);
    CleO.EpochToDate(t0, y, mon, dayOfMonth, h, m, s, dayOfWeek);
    rtc.set_time(y, mon + 1, dayOfMonth, h, m, s, dayOfWeek);
  }
  else { /* RTC is running fine */
    rtc.get_time(y, mon, dayOfMonth, h, m, s, dayOfWeek);
    CleO.DateToEpoch(t0, y, mon - 1, dayOfMonth, h, m, s);
  }

  startTime = millis();

  icon_play[0] = CleO.LoadIcon("@Icons/m48.ftico", ICON_PLAY_CIRCLE_OUTLINE);
  icon_play[1] = CleO.LoadIcon("@Icons/m48.ftico", ICON_PLAY_CIRCLE_FILLED);
  icon_setting = CleO.LoadIcon("@Icons/m48.ftico", ICON_SETTINGS);
}

void loop() {
  touchControl();
  updateTimer();
}

void updateTimer() {
  int16_t s, m, h, dayOfWeek, dayOfMonth, mon, y;

  if (software_timer == false)
      rtc.get_time(y, mon, dayOfMonth, h, m, s, dayOfWeek);
  else {
    elapsedTime = millis() - startTime;
    CleO.EpochToDate(t0 + elapsedTime / 1000L, y, mon, dayOfMonth, h, m, s, dayOfWeek);
    mon += 1; /* CleO.EpochToDate() gives January month as 0 */
    dayOfWeek += 1; /* CleO.EpochToDate() gives Sunday as 0 */

  }

  /* update speaking time */
  speak_hour = h, speak_min = m;

  /* Start drawing the screen shot */
  CleO.Start();

  /* Draw hour, min and sec with digits */
  drawDigitalClock(h, m, s);

  /* Analog clock will need 12 hours format */
  if (h > 12)
    h -= 12;

  /* Draw date with digits by passing day of week, day of month, month and
    year values */
  drawDate(dayOfWeek, dayOfMonth, mon, y);

  /* Draw analog watch with hour, min and second hands */
  drawClock(h, m, s);

  /* Draw play icon */
  CleO.Tag(100);
  CleO.Bitmap(icon_play[playing], 10, 360);

  /* Draw setting icon */
  CleO.Tag(101);
  CleO.Bitmap(icon_setting, SCREEN_WIDTH - 60, 360);

  /* Show the screenshot */
  CleO.Show();
}

void drawDigitalClock(int h, int m, int sec) {
  char buffer[10];
  sprintf(buffer, "%02d:%02d:%02d", h, m, sec);

  /* Draw a rectangle with same background as clock and MM as justification */
  CleO.RectangleExt(CENTRE_X, 450, SCREEN_WIDTH, 50, WHITE, MM, 0, 0);

  /* Draw time string */
  CleO.StringExt(FONT_MEDIUM, CENTRE_X, 450, BLACK, MM, 0, 0, buffer);
}

void drawDate(int dayOfWeek, int dayOfMonth, int mon, int y) {
  /* Convert bytes to strings */
  char strDayOfWeek[][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

  /* Prepare display string */
  char buffer[30];
  sprintf(buffer, "%s %02d/%02d/%02d", strDayOfWeek[dayOfWeek], dayOfMonth, mon, y);

  /* Draw a rectangle with MM justificaiton */
  CleO.RectangleExt(CENTRE_X, 20, SCREEN_WIDTH, 50, WHITE, MM, 0, 0);

  /* Draw date string */
  CleO.StringExt(FONT_MEDIUM, CENTRE_X, 20, BLACK, MM, 0, 0, buffer);
}

void drawClockDigits() {
  /* Draw clock digits at various angles and digits 6, 9, 12, 3 with bigger font */
  char digits[][3] = {"6", "7", "8", "9", "10", "11", "12", "1", "2", "3", "4", "5"};
  int angle = 0;
  for (int i = 0; i < 12; ++i) {
    int font = (i % 3) == 0 ? FONT_SANS_5 : FONT_MEDIUM;
    CleO.StringExt(font, CENTRE_X, CENTRE_Y, BLACK, MM, angle, 119, digits[i]);
    angle += 30;
  }
}

void drawClockMinuteLines() {
  CleO.LineColor(BLACK);
  CleO.LineWidth(2);

  int radius1 = 143, radius2 = 155;

  for (int deg = 0; deg < 360; deg += 6)
  {
    if (deg == 0 || deg == 90 || deg == 180 || deg == 270)
      radius1 = 135;
    else
      radius1 = 143;

    CleO.LineR1R2(CENTRE_X, CENTRE_Y, radius1, radius2, deg);
  }
}

void drawClockHands(int h, int m, int s) {
  uint32_t local_daytime = (3600L * h) + (60 * m) + s;
  int hour_degrees = 180 + (360L * local_daytime) / (12L * 3600);
  int min_degrees = 180 + 360 * (local_daytime % 3600) / 3600;
  int sec_degrees = 180 + (360L * (s * 1000L + (elapsedTime % 1000L))) / 60000L;

  CleO.NeedleWidth(16);
  CleO.NeedleExt(CENTRE_X, CENTRE_Y, 60, BLACK, hour_degrees, 0);

  CleO.NeedleWidth(12);
  CleO.NeedleExt(CENTRE_X, CENTRE_Y, 85, BLACK, min_degrees, 0);

  CleO.NeedleWidth(8);
  CleO.NeedleExt(CENTRE_X, CENTRE_Y, 80, DARK_RED, sec_degrees, 0);
}

void drawClock(int h, int m, int sec) {
  CleO.PointExt(CENTRE_X, CENTRE_Y, RADIUS, WHITE, 0, 0);
  drawClockDigits();
  drawClockMinuteLines();
  drawClockHands(h, m, sec);
}

/* updateTimer() routin is called to update UI while speaking time */
void processTags(int16_t tag) {
  char buf[40];
  if (tag == 100) {
    CleO.AudioPlay("@Music/Sounds/TheTimeIsNow.wav", PLAY_WAIT);
    updateTimer();

    if (speak_hour > 12)
      speak_hour -= 12;

    if (speak_hour == 0 || speak_hour == 12)
      CleO.AudioPlay("@Music/Sounds/12.wav", PLAY_WAIT);
    else {
      sprintf(buf, "@Music/Sounds/%d.wav", speak_hour);
      CleO.AudioPlay(buf, PLAY_WAIT);
    }

    updateTimer();
    CleO.AudioPlay("@Music/Sounds/oClock.wav", PLAY_WAIT);
    updateTimer();

    if (speak_min > 0) {
      if (speak_min < 21 || speak_min == 30 || speak_min == 40 || speak_min == 50) {
        sprintf(buf, "@Music/Sounds/%d.wav", speak_min);
        CleO.AudioPlay(buf, PLAY_WAIT);
        updateTimer();
      } else {
        if (speak_min < 30)
          CleO.AudioPlay("@Music/Sounds/20.wav", PLAY_WAIT);
        else if (speak_min < 40)
          CleO.AudioPlay("@Music/Sounds/30.wav", PLAY_WAIT);
        else if (speak_min < 50)
          CleO.AudioPlay("@Music/Sounds/40.wav", PLAY_WAIT);
        else
          CleO.AudioPlay("@Music/Sounds/50.wav", PLAY_WAIT);

        updateTimer();
        sprintf(buf, "@Music/Sounds/%d.wav", speak_min % 10);
        CleO.AudioPlay(buf, PLAY_WAIT);
        updateTimer();
      }

      CleO.AudioPlay("@Music/Sounds/minutes.wav", PLAY_WAIT);
      updateTimer();
    }
  } else if (tag == 101) { /* setting button is pressed */
    int16_t s = 0, m = 0, h = 0, dayOfWeek = 0, dayOfMonth = 0, mon = 0, y = 0;
    elapsedTime = (millis() - startTime) / 1000L;
    uint32_t t1 = t0 + elapsedTime;
    int16_t ret = CleO.SetDateTime("Date/Time", t1);
    if (0 == ret) {
      CleO.EpochToDate(t1, y, mon, dayOfMonth, h, m, s, dayOfWeek);
      rtc.set_time(y, mon + 1, dayOfMonth, h, m, s, dayOfWeek);
      startTime = millis();
      t0 = t1;
    }
  }
}

void touchControl() {
  int16_t x, y, dur, tag;

  CleO.TouchCoordinates(x, y, dur, tag);

  /* pen down */
  if (previousDur == 0 && dur != 0) {
    previousTag = tag;
    previousDur = 1;
    if (tag == 100)
      playing = 1;
  }
  /* pen up */
  else if (previousDur == 1 && dur == 0) {
    processTags(previousTag);
    previousDur = 0;
    if (previousTag == 100)
      playing = 0;
  }
}
