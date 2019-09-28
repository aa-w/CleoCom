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
    Project01-BasicAudioSoundsDemo.ino
  Purpose:              
    To desmonstrate how to play various built-in sounds in CleO
  External component:   
    CleO Speaker
  Hardware Connection:  
    Connect CleO Speaker to CleO35 CN13 connector
*/

#include <SPI.h>
#include <CleO.h>

int16_t icon_speaker, icon_note, icon_play[2];

int playing = 0;
int volume = 25;
int pitch = 60;    // MIDI middle C
int sound = SQUAREWAVE;

struct {
  const char *name;
  int code;
} instruments[] = {
  {"Square",      SQUAREWAVE},
  {"Sine",        SINEWAVE},
  {"Sawtooth",    SAWTOOTH},
  {"Triangle",    TRIANGLE},
  {"Beeping",     BEEPING},
  {"Alarm",       ALARM},
  {"Warble",      WARBLE},
  {"Carousel",    CAROUSEL},
  {"Harp",        HARP},
  {"Xylophone",   XYLOPHONE},
  {"Tuba",        TUBA},
  {"Glockenspiel", GLOCKENSPIEL},
  {"Organ",       ORGAN},
  {"Trumpet",     TRUMPET},
  {"Piano",       PIANO},
  {"Chimes",      CHIMES},
  {"Musicbox",    MUSICBOX},
  {"Bell",        BELL},
  {"Click",       CLICK},
  {"Switch",      SWITCH},
  {"Cowbell",     COWBELL},
  {"Notch",       NOTCH},
  {"Hihat",       HIHAT},
  {"Kickdrum",    KICKDRUM},
  {"Pop",         POP},
  {"Clack",       CLACK},
  {"Chack",       CHACK},
};

void setup() {
  CleO.begin();
  icon_speaker = CleO.LoadIcon("@Icons/m48.ftico", ICON_VOLUME_UP);
  icon_note = CleO.LoadIcon("@Icons/m48.ftico", ICON_MUSIC_NOTE);
  icon_play[0] = CleO.LoadIcon("@Icons/m48.ftico", ICON_PLAY_CIRCLE_OUTLINE);
  icon_play[1] = CleO.LoadIcon("@Icons/m48.ftico", ICON_PLAY_CIRCLE_FILLED);
}

void loop() {
  CleO.Start();

  for (int i = 0; i < 27; i++) {
    int x = (i % 3) * 107 + 53;
    int y = (i / 3) * 30;
    uint32_t color = (sound == instruments[i].code) ? WHITE : GRAY70;
    CleO.Tag(instruments[i].code);
    CleO.StringExt(FONT_SANS_1, x, y, color, TM, 0, 0, instruments[i].name);
  }
  CleO.BitmapJustification(MM);

  CleO.Tag(202);
  CleO.Bitmap(icon_play[playing], 160, 310);
  CleO.Tag(0);
  CleO.Bitmap(icon_note, 30, 380);
  CleO.Tag(200);
  CleO.LineWidth(4);
  CleO.LineColor(LIGHT_BLUE);
  CleO.Line(60, 380, 290, 380);
  CleO.CircleExt(rescale(pitch, 21, 109, 60, 290),
                 380, 0, 15, LIGHT_BLUE, MM, 0, 0);

  CleO.Tag(0);
  CleO.Bitmap(icon_speaker, 30, 460);
  CleO.Tag(201);
  CleO.LineWidth(4);
  CleO.LineColor(DARK_ORANGE);
  CleO.Line(60, 460, 290, 460);
  CleO.CircleExt(rescale(volume, 0, 255, 60, 290),
                 460, 0, 15, DARK_ORANGE, MM, 0, 0);

  CleO.Show();

  control();
}

// Rescale x from the range (x0, x1) to the range (y0, y1)
float rescale(int x, int x0, int x1, int y0, int y1) {
  x = max(x0, min(x1, x));
  return y0 + ((x - x0) * long(y1 - y0) / float(x1 - x0));
}

void control() {
  static int prevdur;
  int16_t x, y, dur, tag;

  CleO.TouchCoordinates(x, y, dur, tag);
  if (dur) {
    if ((SQUAREWAVE <= tag) && (tag <= CHACK))
      sound = tag;
    if (tag == 200)
      pitch = int(rescale(x, 60, 290, 21, 109));
    if (tag == 201)
      volume = int(rescale(x, 60, 290, 0, 255));
  }

  if (dur && !prevdur && (tag == 202)) {
    CleO.SetSound(sound, pitch, volume, 0);
    CleO.SoundPlay(1);
    playing = 1;
  }
  if (!dur) {
    CleO.SoundPlay(0);
    playing = 0;
  }

  prevdur = dur;
}