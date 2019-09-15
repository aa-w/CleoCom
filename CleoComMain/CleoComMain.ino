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

#define Mode1 1 //Serial
#define Mode2 2 //SPI
#define Mode3 3 //I2C
#define Mode4 4 //UART

#define Settings 5 //Settings
#define SerialBaud 6 //Serial Baud rate tag
#define SerialBaud1 7
#define SerialBaud2 8
#define SerialBaud3 9
#define SerialBaud4 10

byte MenuMode = 1;
byte tag;

////////////// SETTINGS VAIRABLES //////////////

bool ShowSettings = false;
long SettingsDebounceTimer;

////////////// SERIAL SETTINGS //////////////

int16_t SerialBaudArray [4] = {4800, 9600, 19200, 38400};
byte SerialBaudRate = 2;
////////////// SPI SETTINGS //////////////

////////////// I2C SETTINGS //////////////

////////////// UART SETTINGS //////////////
void setup()
{
  CleO.begin();
  CleO.DisplayRotate(2, 0);
  CleO.SetBackgroundcolor(WHITE);

  Intro();

  ////////////// SERIAL SETUP //////////////
  Serial.begin(SerialBaudArray[2]);

}

void loop()
{
  //Draw Stage
  CleO.Start();
  MainMenu();

  //Touch Handling
  TouchControl();

  if (ShowSettings == true)
  {
    SettingsPage();
  }
  else
  {
    //Page Switch Case
    switch (MenuMode)
    {
      case 1:
        SerialPage();
        break;
      case 2:
        SPIPage();
        break;
      case 3:
        I2CPage();
        break;
      case 4:
        UARTPage();
        break;
      default:
        break;
    }

  }

  CleO.Show();
}

void MainMenu() //Draws the Main Menu with touch tags
{

  //Draws each of the buttons in Grey
  CleO.RectangleColor(GREY);

  CleO.Tag(Mode1);
  CleO.RectangleXY((0.01 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);

  CleO.Tag(Mode2);
  CleO.RectangleXY((0.251 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);

  CleO.Tag(Mode3);
  CleO.RectangleXY((0.491 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);

  CleO.Tag(Mode4);
  CleO.RectangleXY((0.731 * ScreenWidth), (0.01 * ScreenHeight), 122, 50);

  //Sets the selected button colour
  if (ShowSettings == false)
  {
    CleO.RectangleColor(LIGHT_BLUE);
  }
  else
  {
    CleO.RectangleColor(INDIAN_RED);
  }

  //Outlines the selected mode
  switch (MenuMode)
  {
    case 1:
      CleO.Tag(Mode1);
      CleO.RectangleXY((0.01 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);
      break;
    case 2:
      CleO.Tag(Mode2);
      CleO.RectangleXY((0.251 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);
      break;
    case 3:
      CleO.Tag(Mode3);
      CleO.RectangleXY((0.491 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);
      break;
    case 4:
      CleO.Tag(Mode4);
      CleO.RectangleXY((0.731 * ScreenWidth), (0.01 * ScreenHeight), 122, 50);
      break;
  }

  //Outputs the button names with the assigned tags
  CleO.Tag(Mode1);
  CleO.StringExt(FONT_SANS_4, (0.125 * ScreenWidth), (0.1 * ScreenHeight), WHITE, MM, 0, 0, "SERIAL");

  CleO.Tag(Mode2);

  CleO.StringExt(FONT_SANS_4, (0.375 * ScreenWidth), (0.1 * ScreenHeight), WHITE, MM, 0, 0, "SPI");

  CleO.Tag(Mode3);

  CleO.StringExt(FONT_SANS_4, (0.625 * ScreenWidth), (0.1 * ScreenHeight), WHITE, MM, 0, 0, "I2C");

  CleO.Tag(Mode4);

  CleO.StringExt(FONT_SANS_4, (0.87 * ScreenWidth), (0.1 * ScreenHeight), WHITE, MM, 0, 0, "UART");

  //Draws the settings option for each page - this is whole new system modified by the Menu int

  CleO.Tag(Settings);
  CleO.RectangleXY((0.90 * ScreenWidth), (0.18 * ScreenHeight), 40, 40);
  if (ShowSettings == false)
  {
    CleO.StringExt(FONT_SANS_5, (0.945 * ScreenWidth), (0.25 * ScreenHeight), WHITE, MM, 0, 0, "*");
  }
  else
  {
    CleO.StringExt(FONT_SANS_5, (0.945 * ScreenWidth), (0.245 * ScreenHeight), WHITE, MM, 0, 0, "X");
  }

  CleO.RectangleColor(GREY);
  CleO.Tag(0);
}

void SerialPage() //Serial Log of data
{
  //CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, "SERIAL");

  int SerialByte = 0;
  char SerialData [20];
  String SerialString;
  while (Serial.available() > 0)
  {
    SerialString = Serial.readString();
  }
  SerialString.toCharArray(SerialData, SerialString.length());
  CleO.StringExt(FONT_SANS_4, (0.50 * ScreenWidth), (0.90 * ScreenHeight), BLACK, MM, 0, 0, SerialData);
}

void SPIPage()
{
  CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, "SPI");
}

void I2CPage()
{
  CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, "I2C");
}

void UARTPage()
{
  CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, "UART");
}

void SettingsPage() //Takes the Menu Tag and selects the settings based on the touch
{
  //CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, "SETTINGS");

  switch (MenuMode)
  {
    case 1:
      SerialSettings();
      break;
    case 2:
      SPISettings();
      break;
    case 3:
      I2CSettings();
      break;
    case 4:
      UARTSettings();
      break;
  }
}

void SerialSettings()
{
  char CharOutput [12];

  CleO.StringExt(FONT_SANS_4, (0.10 * ScreenWidth), (0.4 * ScreenHeight), BLACK, MM, 0, 0, "BAUD");

  itoa (SerialBaudArray[SerialBaudRate], CharOutput, 10);
  CleO.Tag(SerialBaud);
  CleO.StringExt(FONT_SANS_4, (0.35 * ScreenWidth), (0.4 * ScreenHeight), RED, MM, 0, 0, CharOutput);

  if (tag == SerialBaud)
  {
    CleO.Show();

    bool Selected = false;
    Serial.end();
    while (Selected == false)
    {
      CleO.Start();
      CleO.Tag(SerialBaud1);
      itoa (SerialBaudArray[0], CharOutput, 10);
      CleO.StringExt(FONT_SANS_4, (0.55 * ScreenWidth), (0.4 * ScreenHeight), RED, MM, 0, 0, CharOutput);

      CleO.Tag(SerialBaud2);
      itoa (SerialBaudArray[1], CharOutput, 10);
      CleO.StringExt(FONT_SANS_4, (0.55 * ScreenWidth), (0.5 * ScreenHeight), RED, MM, 0, 0, CharOutput);

      CleO.Tag(SerialBaud3);
      itoa (SerialBaudArray[2], CharOutput, 10);
      CleO.StringExt(FONT_SANS_4, (0.55 * ScreenWidth), (0.6 * ScreenHeight), RED, MM, 0, 0, CharOutput);

      CleO.Tag(SerialBaud4);
      itoa (SerialBaudArray[3], CharOutput, 10);
      CleO.StringExt(FONT_SANS_4, (0.55 * ScreenWidth), (0.7 * ScreenHeight), RED, MM, 0, 0, CharOutput);

      CleO.Show();
      TouchControl();
      //Sets the position of the Serial Baud Rate in the Array when the baud rate tag is selected
      switch (tag)
      {
        case 7:
          SerialBaudRate = 0;
          Selected = true;
          break;
        case 8:
          SerialBaudRate = 1;
          Selected = true;
          break;
        case 9:
          SerialBaudRate = 2;
          Selected = true;
          break;
        case 10:
          SerialBaudRate = 3;
          Selected = true;
          break;
        default:
          break;
      }
    }

    //starts a new serial with the new data rate
    Serial.begin(SerialBaudArray[SerialBaudRate]);

    CleO.Start();

  }

  CleO.Tag(255);
}

void SPISettings()
{
  CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.70 * ScreenHeight), BLACK, MM, 0, 0, "SPI");

}

void I2CSettings()
{
  CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.70 * ScreenHeight), BLACK, MM, 0, 0, "I2C");

}

void UARTSettings()
{
  CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.70 * ScreenHeight), BLACK, MM, 0, 0, "UART");

}

void Intro()  //Draws DS Style Intro Animation Isolated from the rest of the program
{
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
    CleO.StringExt(FONT_SANS_7, (0.62 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, "COM");

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

  delay(1500);
}

void TouchControl()//Touch Control Handler
{
  int16_t x, y, dur, TouchedTag;
  CleO.TouchCoordinates(x, y, dur, TouchedTag);
  tag = TouchedTag;
  if (tag == 1 || tag == 2 || tag == 3 || tag == 4)
  {
    MenuMode = tag;
  }

  //Debounce Delay prevents fast cycling
  if (tag == 5 && millis() > SettingsDebounceTimer)
  {
    //This toggles the settings function
    if (ShowSettings == false)
    {
      ShowSettings = true;
    }
    else
    {
      ShowSettings = false;
    }
    SettingsDebounceTimer = millis() + 255;
  }

}
