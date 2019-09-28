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
    Project04-RetroDigitalClockNIXIEType.ino
  Purpose:              
    To desmonstrate how to draw a retro digital clock with nixie font (bitmap files)
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

PCF8523 rtc;

int16_t digit[10]; /* Holds image handles */
uint32_t t0 = 0; /* Holds epoch time for software timer */
int16_t previousDur = 0, previousTag = 0;
bool software_timer = false;
unsigned long startTime, elapsedTime; /* Used for calculating elapsed secs */
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

  CleO.Start();
  CleO.Show();

  /* Get each digit image handle */
  digit[0] = CleO.LoadImageFile("@Pictures/nixie/0.png", 0);
  digit[1] = CleO.LoadImageFile("@Pictures/nixie/1.png", 0);
  digit[2] = CleO.LoadImageFile("@Pictures/nixie/2.png", 0);
  digit[3] = CleO.LoadImageFile("@Pictures/nixie/3.png", 0);
  digit[4] = CleO.LoadImageFile("@Pictures/nixie/4.png", 0);
  digit[5] = CleO.LoadImageFile("@Pictures/nixie/5.png", 0);
  digit[6] = CleO.LoadImageFile("@Pictures/nixie/6.png", 0);
  digit[7] = CleO.LoadImageFile("@Pictures/nixie/7.png", 0);
  digit[8] = CleO.LoadImageFile("@Pictures/nixie/8.png", 0);
  digit[9] = CleO.LoadImageFile("@Pictures/nixie/9.png", 0);

  icon_play[0] = CleO.LoadIcon("@Icons/m48.ftico", ICON_PLAY_CIRCLE_OUTLINE);
  icon_play[1] = CleO.LoadIcon("@Icons/m48.ftico", ICON_PLAY_CIRCLE_FILLED);
  icon_setting = CleO.LoadIcon("@Icons/m48.ftico", ICON_SETTINGS);

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

  CleO.Start();
  CleO.Show();

  /* Set screen coordinate in landscape mode */
  CleO.DisplayRotate(2, 0);
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
    elapsedTime = (millis() - startTime) / 1000L;
    CleO.EpochToDate(t0 + elapsedTime, y, mon, dayOfMonth, h, m, s, dayOfWeek);
    mon += 1; /* CleO.EpochToDate() gives January month as 0 */
    dayOfWeek += 1; /* CleO.EpochToDate() gives Sunday as 0 */
  }

  /* update speaking time */
  speak_hour = h, speak_min = m;

  /* Start drawing the screen shot */
  CleO.Start();

  /* Draw bitmap digits for hour, minute and second */
  show2(0, 0, h);
  show2(170, 0, m);
  show2(340, 0, s);

  /* Draw bitmap digits for month and day of month */
  show2(274, 112, mon);
  show2(70, 112, dayOfMonth);

  /* Draw bitmap digits for year */
  show2(104, 224, y / 100);
  show2(240, 224, y % 100);

  /* Draw play icon */
  CleO.Tag(100);
  CleO.Bitmap(icon_play[playing], 20, 250);

  /* Draw setting icon */
  CleO.Tag(101);
  CleO.Bitmap(icon_setting, 410, 250);

  /* Show the screenshot */
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
    CleO.Start();
    CleO.Show();

    /* Set screen coordinate in portrait mode */
    CleO.DisplayRotate(1, 0);
    int16_t s = 0, m = 0, h = 0, dayOfWeek = 0, dayOfMonth = 0, mon = 0, y = 0;
    elapsedTime = (millis() - startTime) / 1000L;
    uint32_t t1 = t0 + elapsedTime;
    int16_t ret = CleO.SetDateTime("Date/Time", t1);

    CleO.Start();
    CleO.Show();
    /* Set screen coordinate in landscape mode */
    CleO.DisplayRotate(2, 0);

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

// Show a 2-digit number at (x, y)
void show2(int x, int y, int nn) {
  /* Draw first digit bitmap at (x, y) by using digit handle */
  CleO.Bitmap(digit[nn / 10], x, y);

  /* Draw second digit bitmap at (x, y) by using digit handle */
  CleO.Bitmap(digit[nn % 10], x + 68, y);
}
