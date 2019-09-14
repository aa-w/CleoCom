/**
    CleoCom ----------------
    Alex.W - September - 2019
    This is a little sideproject, please don't blame me if it doesn't work properly.
    ------------------------
*/

#include <SPI.h>
#include <CleO.h>

#define ScreenWidth  480
#define ScreenHeight 320

#define Button1 1
#define Button2 2
#define Button3 3
#define Button4 4

int16_t previousDur = 0, previousTag = 0;

void setup()
{
  CleO.begin();
  CleO.DisplayRotate(2, 0);
  CleO.SetBackgroundcolor(WHITE);

  Intro();

}

void loop()
{
  Intro();
  IntroInvert();

}

void MainMenu()
{
  //Draws the Main Menu with touch tags

  CleO.Start();

  CleO.RectangleColor(GREY);

  CleO.Tag(Button1);
  CleO.RectangleXY((0.01 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);
  CleO.StringExt(FONT_SANS_4, (0.125 * ScreenWidth), (0.1 * ScreenHeight), WHITE, MM, 0, 0, "SERIAL");

  CleO.Tag(Button2);
  CleO.RectangleXY((0.251 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);
  CleO.StringExt(FONT_SANS_4, (0.375 * ScreenWidth), (0.1 * ScreenHeight), WHITE, MM, 0, 0, "SPI");

  CleO.Tag(Button3);
  CleO.RectangleXY((0.491 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);
  CleO.StringExt(FONT_SANS_4, (0.625 * ScreenWidth), (0.1 * ScreenHeight), WHITE, MM, 0, 0, "I2C");

  CleO.Tag(Button4);
  CleO.RectangleXY((0.731 * ScreenWidth), (0.01 * ScreenHeight), 122, 50);
  CleO.StringExt(FONT_SANS_4, (0.87 * ScreenWidth), (0.1 * ScreenHeight), WHITE, MM, 0, 0, "UART");

  CleO.Show();
}

void Intro()
{
  CleO.SetBackgroundcolor(WHITE);
  double MoveDownPer = 0;
  double MoveUpPer = 1;
  int AnimationTime = millis() + 1500;
  int AnimationInc = millis() + 200;

  //Shapes Move Together
  while (millis() < AnimationTime && MoveDownPer < 0.42)
  {
    if (millis() > AnimationInc)
    {
      MoveDownPer = MoveDownPer + 0.05;
      MoveUpPer = MoveUpPer - 0.05;
      AnimationInc = millis() + 30;
    }

    CleO.Start();
    CleO.StringExt(FONT_SANS_4, (0.32 * ScreenWidth), (0.46 * ScreenHeight), BLACK, MM, 0, 0, "CLE");
    double randval = (random(0,100) / 100);
    CleO.StringExt(FONT_SANS_7, (randval * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, "COM");

    CleO.RectangleJustification(MM);
    CleO.RectangleCorner(6);
    CleO.RectangleColor(BLACK);
    CleO.RectangleXY((0.42 * ScreenWidth), (MoveDownPer * ScreenHeight), 30, 30);

    CleO.RectangleCorner(5);
    CleO.RectangleColor(WHITE);
    CleO.RectangleXY((0.42 * ScreenWidth), (MoveDownPer * ScreenHeight), 20, 20);

    CleO.RectangleCorner(6);
    CleO.RectangleColor(GREY);
    CleO.RectangleXY((0.42 * ScreenWidth), (MoveUpPer * ScreenHeight), 30, 30);

    CleO.RectangleCorner(5);
    CleO.RectangleColor(WHITE);
    CleO.RectangleXY((0.42 * ScreenWidth), (MoveUpPer * ScreenHeight), 20, 20);
    CleO.Show();
  }

  CleO.Start();

  CleO.StringExt(FONT_SANS_4, (0.32 * ScreenWidth), (0.46 * ScreenHeight), BLACK, MM, 0, 0, "CLE");
  CleO.StringExt(FONT_SANS_7, (0.62 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, "COM");

  CleO.RectangleJustification(MM);
  CleO.RectangleCorner(6);
  CleO.RectangleColor(BLACK);
  CleO.RectangleXY((0.42 * ScreenWidth), (0.46 * ScreenHeight), 30, 30);

  CleO.RectangleColor(GREY);
  CleO.RectangleXY((0.42 * ScreenWidth), (0.56 * ScreenHeight), 30, 30);

  CleO.RectangleCorner(5);
  CleO.RectangleColor(WHITE);
  CleO.RectangleXY((0.42 * ScreenWidth), (0.56 * ScreenHeight), 20, 20);

  CleO.RectangleColor(WHITE);
  CleO.RectangleXY((0.42 * ScreenWidth), (0.46 * ScreenHeight), 20, 20);

  CleO.CircleExt((0.40 * ScreenWidth), (0.85 * ScreenHeight), 10, 14, GREY, MM, 0, 0);
  CleO.CircleExt((0.60 * ScreenWidth), (0.85 * ScreenHeight), 10, 14, GREY, MM, 0, 0);

  CleO.RectangleCorner(1);
  CleO.RectangleColor(GREY);
  CleO.RectangleXY((0.50 * ScreenWidth), (0.85 * ScreenHeight), 100, 28);
  CleO.RectangleColor(WHITE);
  CleO.RectangleXY((0.50 * ScreenWidth), (0.85 * ScreenHeight), 100, 23);

  CleO.StringExt(FONT_SANS_3, (0.50 * ScreenWidth), (0.85 * ScreenHeight), GREY, MM, 0, 0, "ALEX.W");

  CleO.Show();

  //delay(2000);
}

void IntroInvert()
{
  CleO.SetBackgroundcolor(BLACK);
  double MoveDownPer = 0;
  double MoveUpPer = 1;
  int AnimationTime = millis() + 1500;
  int AnimationInc = millis() + 200;

  //Shapes Move Together
  while (millis() < AnimationTime && MoveDownPer < 0.42)
  {
    if (millis() > AnimationInc)
    {
      MoveDownPer = MoveDownPer + 0.05;
      MoveUpPer = MoveUpPer - 0.05;
      AnimationInc = millis() + 30;
    }

    CleO.Start();
    CleO.StringExt(FONT_SANS_4, (0.32 * ScreenWidth), (0.46 * ScreenHeight), WHITE, MM, 0, 0, "CLE");
    CleO.StringExt(FONT_SANS_7, (0.62 * ScreenWidth), (0.50 * ScreenHeight), WHITE, MM, 0, 0, "COM");

    CleO.RectangleJustification(MM);
    CleO.RectangleCorner(6);
    CleO.RectangleColor(WHITE);
    CleO.RectangleXY((0.42 * ScreenWidth), (MoveDownPer * ScreenHeight), 30, 30);

    CleO.RectangleCorner(5);
    CleO.RectangleColor(BLACK);
    CleO.RectangleXY((0.42 * ScreenWidth), (MoveDownPer * ScreenHeight), 20, 20);

    CleO.RectangleCorner(6);
    CleO.RectangleColor(BLUE);
    CleO.RectangleXY((0.42 * ScreenWidth), (MoveUpPer * ScreenHeight), 30, 30);

    CleO.RectangleCorner(5);
    CleO.RectangleColor(BLACK);
    CleO.RectangleXY((0.42 * ScreenWidth), (MoveUpPer * ScreenHeight), 20, 20);
    CleO.Show();
  }

  CleO.Start();

  CleO.StringExt(FONT_SANS_4, (0.32 * ScreenWidth), (0.46 * ScreenHeight), WHITE, MM, 0, 0, "CLE");
  CleO.StringExt(FONT_SANS_7, (0.62 * ScreenWidth), (0.50 * ScreenHeight), WHITE, MM, 0, 0, "COM");

  CleO.RectangleJustification(MM);
  CleO.RectangleCorner(6);
  CleO.RectangleColor(WHITE);
  CleO.RectangleXY((0.42 * ScreenWidth), (0.46 * ScreenHeight), 30, 30);

  CleO.RectangleColor(RED);
  CleO.RectangleXY((0.42 * ScreenWidth), (0.56 * ScreenHeight), 30, 30);

  CleO.RectangleCorner(5);
  CleO.RectangleColor(BLACK);
  CleO.RectangleXY((0.42 * ScreenWidth), (0.56 * ScreenHeight), 20, 20);

  CleO.RectangleColor(BLACK);
  CleO.RectangleXY((0.42 * ScreenWidth), (0.46 * ScreenHeight), 20, 20);

  CleO.CircleExt((0.40 * ScreenWidth), (0.85 * ScreenHeight), 10, 14, GREY, MM, 0, 0);
  CleO.CircleExt((0.60 * ScreenWidth), (0.85 * ScreenHeight), 10, 14, GREY, MM, 0, 0);

  CleO.RectangleCorner(1);
  CleO.RectangleColor(PINK);
  CleO.RectangleXY((0.50 * ScreenWidth), (0.85 * ScreenHeight), 100, 28);
  CleO.RectangleColor(BLACK);
  CleO.RectangleXY((0.50 * ScreenWidth), (0.85 * ScreenHeight), 100, 23);

  CleO.StringExt(FONT_SANS_3, (0.50 * ScreenWidth), (0.85 * ScreenHeight), GREY, MM, 0, 0, "ALEX.W");

  CleO.Show();

  //delay(2000);
}
void TouchControl()   //Touch Control Handler
{
  int16_t x, y, dur, tag;
  CleO.TouchCoordinates(x, y, dur, tag);

  /* pen down */
  if (previousDur == 0 && dur != 0)
  {
    previousTag = tag; previousDur = 1;
  }
  /* pen up */
  else if (previousDur == 1 && dur == 0)
  {
    ProcessTags(previousTag); previousDur = 0;
  }
}

void ProcessTags(int tag)
{
  CleO.Start();
  CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, "TOUCHY");
  CleO.Show();
}
