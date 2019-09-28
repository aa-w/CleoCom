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
    Project07-AModermStyleAnalogClock.ino
  Purpose:              
    To desmonstrate how to draw a modern style analog clock using the CleO primitives
  External component:   
    PCF8523T RTC module
    CleO Speaker
  Hardware Connection:
    Connect CleO Speaker to CleO50 CN2 connector
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

#define SCREEN_WIDTH           480
#define SCREEN_HEIGHT          800
#define CENTRE_X              (0.5 * SCREEN_WIDTH)
#define CENTRE_Y              (0.5 * SCREEN_HEIGHT)
#define RADIUS                (SCREEN_WIDTH/2 - 10)
#define CLOCK_X               (CENTRE_X)
#define CLOCK_Y               (CENTRE_Y)
#define DATE_TIME_X           (CENTRE_X)

PCF8523 rtc;

uint32_t t0 = 0; /* Holds epoch time for software timer */
#define BGHEAVY 0x237ead
#define BGLIGHT 0x7bb2ce

/* Used for smooth sec hand movement */
unsigned long currentElapsedTime = 0;
int16_t previousDur = 0, previousTag = 0;
bool software_timer = false;
unsigned long startTime, elapsedTime = 0; /* Used for calculating elapsed secs */
int speak_hour, speak_min; /* Used while speaking time */
int16_t icon_play[2], icon_setting;
int playing = 0;

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

  CleO.SetBackgroundcolor(BGHEAVY);

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

  /* Set screen coordinate in portrait mode */
  CleO.DisplayRotate(2, 0);
}

void loop() {
  touchControl();
  updateTimer();
}

void updateTimer() {
  int f = FONT_SANS_3;       // Font for numbers
  int cx = CLOCK_X, cy = CLOCK_Y;    // Clock center
  int r0 = RADIUS - 30, r1 = RADIUS - 15, r2 = RADIUS;
  int16_t s, m, h, dayOfWeek, dayOfMonth, mon, y;

  if (software_timer == false)
    rtc.get_time(y, mon, dayOfMonth, h, m, s, dayOfWeek);
  else {
    elapsedTime = millis() - startTime;
    CleO.EpochToDate(t0 + elapsedTime / 1000L, y, mon, dayOfMonth, h, m, s, dayOfWeek);
  }

  /* update speaking time */
  speak_hour = h, speak_min = m;

  currentElapsedTime = millis();

  int hour_degrees = (360L * ((h + 6) % 12)) / 12;
  int min_degrees = 180 + (360L * m) / 60;
  int sec_degrees = 180 + (360L * (s * 1000L + (elapsedTime % 1000L))) / 60000L;

  CleO.Start();

  // Top menu
  CleO.Tag(2);
  CleO.StringExt(FONT_SANS_5, DATE_TIME_X, 80, WHITE, MM, 0, 0, "Clock");

  // Outer ring
  CleO.CircleExt(cx, cy, r2, r2 + 0.5, WHITE, MM, 0, 0);

  // gaps for the four numbers
  for (int a = 0; a < 360; a += 90)
    CleO.CircleExt(cx, cy, 0, 10, BGHEAVY, MM, a, r2);

  CleO.StringExt(f, cx, cy, WHITE, MM, 270, r2, "3");
  CleO.StringExt(f, cx, cy, WHITE, MM, 0, r2, "6");
  CleO.StringExt(f, cx, cy, WHITE, MM, 90, r2, "9");
  CleO.StringExt(f, cx, cy, WHITE, MM, 180, r2, "12");

  // Outside rim
  for (int deg = 0; deg < 360; deg += 2) {
    int age = (360 + sec_degrees - deg) % 360;
    float fade = 1.2 - (age / 64.);
    CleO.LineWidth(max(0.25, min(1.0, fade)));
    CleO.LineR1R2(cx, cy, r0, r1, deg);
  }

  // Draw the seconds hand as a small triangle
  CleO.NeedleWidth(12);
  CleO.NeedleR1R2(cx, cy, r0 - 15, r0 - 2, sec_degrees);
  CleO.CircleExt(cx, cy, 0, r0 - 15, BGHEAVY, MM, 0, 0);

  // Hour and minute hands
  CleO.LineWidth(2.0);
  CleO.LineR1R2(cx, cy, 6, 2 * r0 / 3, hour_degrees);
  CleO.LineWidth(1.5);
  CleO.LineR1R2(cx, cy, 6, 4 * r0 / 5, min_degrees);

  // Dial center knob
  CleO.CircleExt(cx, cy, 0, 6, WHITE, MM, 0, 0);
  CleO.CircleExt(cx, cy, 0, 3, BGHEAVY, MM, 0, 0);

  /* Draw play icon */
  CleO.Tag(100);
  CleO.Bitmap(icon_play[playing], DATE_TIME_X - 170, SCREEN_HEIGHT - 130);

  /* Draw setting icon */
  CleO.Tag(101);
  CleO.Bitmap(icon_setting, DATE_TIME_X + 130, SCREEN_HEIGHT - 130);

  CleO.Show();
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
    CleO.DisplayRotate(1, 0);   //set to landscape mode to display SetDateTime correctly
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
    CleO.DisplayRotate(2, 0);   //restore to portrait mode to display the clock
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
