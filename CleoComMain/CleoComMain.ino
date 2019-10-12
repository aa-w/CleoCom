/**
    CleoCom ----------------
    Alex.W - September - 2019

    This is a little sideproject, please don't blame me if it doesn't work properly.
    ------------------------
*/

#include <SPI.h>
#include <CleO.h>
#include <Wire.h>

//CleO Screen Setup
#define ScreenWidth  480
#define ScreenHeight 320

//Buffer Define values
#define ArraySize 8
#define BufferSize 10

//CleO Button Tag Values
#define SerialButton 1 //Serial
#define SPIButton 2 //SPI
#define I2CButton 3 //I2C
#define UARTButton 4 //UART

#define Settings 5 //Settings
#define SerialBaud 6 //Serial Baud rate tag
#define SerialBaud1 7
#define SerialBaud2 8
#define SerialBaud3 9
#define SerialBaud4 10
#define ClearMemory 11
#define SetI2CAddress 12
#define SetI2CByteSize 13
#define IncrementButton 14
#define DecrementButton 15
#define DoneButton 16
#define I2CScan 17


byte MenuMode = 1; //Top level menu mode
byte tag; //Defines UI Events based on touches

////////////// SETTINGS VAIRABLES //////////////

bool ShowSettings = false;
long SettingsDebounceTimer;

////////////// SERIAL GLOBALS //////////////
char SerialData [ArraySize][BufferSize];
byte SerialPointer = 0;

int16_t SerialBaudArray [4] = {4800, 9600, 19200, 38400};
byte SerialBaudRate = 2;

////////////// SPI GLOBALS //////////////
char SPIData [ArraySize][BufferSize];

////////////// I2C GLOBALS //////////////
char I2CData [ArraySize][BufferSize];
byte I2CPointer = 0;

int I2CAddress = 1;
byte I2CByteSize = 1;

#define SDAPin 4
#define SCLPin 5

////////////// UART GLOBALS //////////////
char UARTData [ArraySize][BufferSize];

void setup()
{
  ////////////// CLEO ANIMATION SETUP //////////////
  CleO.begin();
  CleO.DisplayRotate(2, 0);
  CleO.SetBackgroundcolor(WHITE);

  // IDLE ANIMATION //
  CleO.LineWidth(5);

  ////////////// SERIAL SETUP //////////////
  SerialData[0][0] = ' ';
  SerialData[1][0] = ' ';
  SerialData[2][0] = ' ';
  SerialData[3][0] = ' ';
  SerialData[4][0] = ' ';
  SerialData[5][0] = ' ';
  SerialData[6][0] = ' ';
  SerialData[7][0] = ' ';

  Serial.begin(SerialBaudArray[2]);

  ////////////// SERIAL SETUP //////////////
  I2CData[0][0] = ' ';
  I2CData[1][0] = ' ';
  I2CData[2][0] = ' ';
  I2CData[3][0] = ' ';
  I2CData[4][0] = ' ';
  I2CData[5][0] = ' ';
  I2CData[6][0] = ' ';
  I2CData[7][0] = ' ';

  Wire.begin();

  ///////////// INTRO ANIMATION //////////////
  Intro();

}

void loop()
{

  //BACKEND
  BackendHandler();

  //UI LOOP
  UIHandler();

  //Touch Handling
  TouchHandler();

}

void UIHandler() //Handles User Interfaces
{
  //Draw Stage
  CleO.Start();

  MainMenu();

  //Page Switch Case
  switch (MenuMode)
  {
    case 1:
      CleO.StringExt(FONT_SANS_7, (0.50 * ScreenWidth), (0.55 * ScreenHeight), GREY85, MM, 0, 0, "SERIAL");

      if (ShowSettings == true)
      {
        SerialSettings();
      }
      else
      {
        //Runs the spooler function with Serial Pointer
        Spooler(SerialPointer);
      }
      break;
    case 2:
      CleO.StringExt(FONT_SANS_7, (0.50 * ScreenWidth), (0.55 * ScreenHeight), GREY85, MM, 0, 0, "SPI");
      if (ShowSettings == true)
      {
        SPISettings();
      }
      else
      {
        SPIPage();
      }
      break;
    case 3:
      CleO.StringExt(FONT_SANS_7, (0.50 * ScreenWidth), (0.55 * ScreenHeight), GREY85, MM, 0, 0, "I2C");
      if (ShowSettings == true)
      {
        I2CSettings();
      }
      else
      {
        Spooler(I2CPointer);
      }
      break;
    case 4:
      CleO.StringExt(FONT_SANS_7, (0.50 * ScreenWidth), (0.55 * ScreenHeight), GREY85, MM, 0, 0, "UART");
      if (ShowSettings == true)
      {
        UARTSettings();
      }
      else
      {
        UARTPage();
      }
      break;
    default:
      break;
  }

  AnimationLoop();

  CleO.Show();
}

void BackendHandler() //Handlers the Communication and operation
{
  switch (MenuMode)
  {
    case 1:
      SerialHandler();
      break;
    case 2:
      SPIHandler();
      break;
    case 3:
      I2CHandler();
      break;
    case 4:
      UARTHandler();
      break;
    default:
      break;
  }

}

// ------------------------ BACKEND ----------------------------------------

void SerialHandler() //Serial Backend
{
  String SerialString;

  if (Serial.available() > 0)
  {
    SerialString = Serial.readString();
    SerialString.toCharArray(SerialData[SerialPointer], SerialString.length());

    if (SerialPointer != (ArraySize - 1))
    {
      SerialPointer++;
    }
    else
    {
      SerialPointer = 0;
    }
  }
}

void SPIHandler()
{

}

void I2CHandler()
{

  Wire.requestFrom(I2CAddress, I2CByteSize);
  Serial.println("Request Sent");
  if (Wire.available())
  {
    //I2CString = Wire.read();
    Serial.println(Wire.available());
    //I2CString.toCharArray(SerialData[I2CPointer], I2CString.length());

    for (int index = 0; index < I2CByteSize; index++)
    {
      Serial.println(Wire.available());
      I2CData[I2CPointer][index] = Wire.read();
      Serial.println(I2CData[I2CPointer][index]);
    }

    if (I2CPointer != (ArraySize - 1))
    {
      I2CPointer++;
    }
    else
    {
      I2CPointer = 0;
    }
  }

}

void UARTHandler()
{

}

// ------------------------ UI ----------------------------------------

void MainMenu() //Draws the Main Menu with touch tags
{
  //Draws each of the buttons in Grey
  CleO.RectangleColor(GREY);

  CleO.Tag(SerialButton);
  CleO.RectangleXY((0.01 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);

  CleO.Tag(SPIButton);
  CleO.RectangleXY((0.251 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);

  CleO.Tag(I2CButton);
  CleO.RectangleXY((0.491 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);

  CleO.Tag(UARTButton);
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

  //Outlines in BLUE the selected mode
  switch (MenuMode)
  {
    case 1:
      CleO.Tag(SerialButton);
      CleO.RectangleXY((0.01 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);
      break;
    case 2:
      CleO.Tag(SPIButton);
      CleO.RectangleXY((0.251 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);
      break;
    case 3:
      CleO.Tag(I2CButton);
      CleO.RectangleXY((0.491 * ScreenWidth), (0.01 * ScreenHeight), 112, 50);
      break;
    case 4:
      CleO.Tag(UARTButton);
      CleO.RectangleXY((0.731 * ScreenWidth), (0.01 * ScreenHeight), 122, 50);
      break;
  }

  //Outputs the button names with the assigned tags
  CleO.Tag(SerialButton);
  CleO.StringExt(FONT_SANS_4, (0.125 * ScreenWidth), (0.1 * ScreenHeight), WHITE, MM, 0, 0, "SERIAL");

  CleO.Tag(SPIButton);
  CleO.StringExt(FONT_SANS_4, (0.375 * ScreenWidth), (0.1 * ScreenHeight), WHITE, MM, 0, 0, "SPI");

  CleO.Tag(I2CButton);
  CleO.StringExt(FONT_SANS_4, (0.625 * ScreenWidth), (0.1 * ScreenHeight), WHITE, MM, 0, 0, "I2C");

  CleO.Tag(UARTButton);
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

/*
  void SerialPage() //Serial Page Interactions
  {
  char SubBuffer [BufferSize];
  byte positionvalue = SerialPointer;
  double ScreenPos = 0.90;

  for (byte index = 0; index != ArraySize; index++)
  {
    if (positionvalue == 0)
    {
      positionvalue = (ArraySize - 1);
    }

    strncpy(SubBuffer, SerialData[positionvalue], BufferSize);
    CleO.StringExt(FONT_SANS_4, (0.20 * ScreenWidth), (ScreenPos * ScreenHeight), BLACK, ML, 0, 0, SubBuffer);
    ScreenPos = (ScreenPos - 0.10);
    positionvalue--;
  }
  }
*/

void SPIPage()
{
  //CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, "SPI");
}

void Spooler(byte positionvalue) //Universal output Spooler for the data arrays collected
{
  double ScreenPos = 0.95;
  char SubBuffer [BufferSize];

  for (byte index = 0; index != ArraySize; index++)
  {
    if (positionvalue == 0)
    {
      positionvalue = (ArraySize - 1);
    }

    switch (MenuMode)
    {
      case 1:
        strncpy(SubBuffer, SerialData[positionvalue], BufferSize);
        break;
      case 2:
        break;
      case 3:
        strncpy(SubBuffer, I2CData[positionvalue], BufferSize);
        break;
      case 4:
        break;
      default:
        CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.50 * ScreenHeight), RED, MM, 0, 0, "SPOOLER ERROR");
        break;
    }

    CleO.StringExt(FONT_SANS_4, (0.20 * ScreenWidth), (ScreenPos * ScreenHeight), BLACK, ML, 0, 0, SubBuffer);
    ScreenPos = (ScreenPos - 0.10);
    positionvalue--;

  }
}

void UARTPage()
{
  //CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, "UART");
}

void SerialSettings()
{
  char CharOutput [12];

  CleO.StringExt(FONT_SANS_4, (0.05 * ScreenWidth), (0.30 * ScreenHeight), BLACK, ML, 0, 0, "BAUD:");

  itoa (SerialBaudArray[SerialBaudRate], CharOutput, 10);
  CleO.Tag(SerialBaud);
  CleO.StringExt(FONT_SANS_4, (0.30 * ScreenWidth), (0.30 * ScreenHeight), RED, ML, 0, 0, CharOutput);

  CleO.Tag(ClearMemory);
  CleO.StringExt(FONT_SANS_4, (0.05 * ScreenWidth), (0.40 * ScreenHeight), RED, ML, 0, 0, "CLEAR LOG");

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
      TouchHandler();
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

  if (tag == ClearMemory)
  {
    /*
      byte positionvalue = SerialPointer;
      for (byte index = 0; index != ArraySize; index++)
      {
      SerialData[index][0] = (char)0;
      //memset(SerialData[index], 0, sizeof(BufferSize));

      }
    */

    SerialData[0][0] = ' ';
    SerialData[1][0] = ' ';
    SerialData[2][0] = ' ';
    SerialData[3][0] = ' ';
    SerialData[4][0] = ' ';
    SerialData[5][0] = ' ';
    SerialData[6][0] = ' ';
    SerialData[7][0] = ' ';

    Serial.end();
    Serial.begin(SerialBaudArray[SerialBaudRate]);

    CleO.StringExt(FONT_SANS_4, (0.05 * ScreenWidth), (0.40 * ScreenHeight), BLACK, ML, 0, 0, "CLEAR LOG");
    tag = 255;
  }

  CleO.Tag(255);
}

void SPISettings()
{
  CleO.StringExt(FONT_SANS_5, (0.50 * ScreenWidth), (0.70 * ScreenHeight), BLACK, MM, 0, 0, "SPI");

}

void I2CSettings()
{
  char CharOutput [12];

  CleO.StringExt(FONT_SANS_4, (0.05 * ScreenWidth), (0.30 * ScreenHeight), BLACK, ML, 0, 0, "Pin Out: SDA - 4 SCL - 5");
  CleO.StringExt(FONT_SANS_4, (0.05 * ScreenWidth), (0.40 * ScreenHeight), BLACK, ML, 0, 0, "Address:");

  CleO.Tag(SetI2CAddress);
  itoa (I2CAddress, CharOutput, 10);
  CleO.StringExt(FONT_SANS_4, (0.40 * ScreenWidth), (0.40 * ScreenHeight), RED, ML, 0, 0, CharOutput);

  CleO.StringExt(FONT_SANS_4, (0.05 * ScreenWidth), (0.50 * ScreenHeight), BLACK, ML, 0, 0, "No. Bytes:");

  CleO.Tag(SetI2CByteSize);
  itoa (I2CByteSize, CharOutput, 10);
  CleO.StringExt(FONT_SANS_4, (0.40 * ScreenWidth), (0.50 * ScreenHeight), RED, ML, 0, 0, CharOutput);

  CleO.Tag(I2CScan);
  CleO.StringExt(FONT_SANS_4, (0.50 * ScreenWidth), (0.40 * ScreenHeight), RED, ML, 0, 0, "Scan");

  //Set Address Value
  if (tag == SetI2CAddress)
  {
    CleO.Show();

    bool Selected = false;
    long DebounceTimer = 0;
    while (Selected == false)
    {
      CleO.Start();
      CleO.StringExt(FONT_SANS_4, (0.50 * ScreenWidth), (0.05 * ScreenHeight), BLACK, MM, 0, 0, "Set Address:");

      itoa (I2CAddress, CharOutput, 10);
      CleO.StringExt(FONT_SANS_7, (0.50 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, CharOutput);

      DrawSettingsButtons();

      CleO.Show();

      TouchHandler(); //Updates tag with latest touch values

      if (tag == IncrementButton && millis() > DebounceTimer)
      {
        I2CAddress++;
        DebounceTimer = millis() + 200;
      }
      else if (tag == DecrementButton && millis() > DebounceTimer)
      {
        I2CAddress--;
        DebounceTimer = millis() + 200;
      }
      else if (tag == DoneButton)
      {
        Selected = true;
        CleO.Start();
      }
      else
      {
      }

    }
  }

  //Set Byte Size
  if (tag == SetI2CByteSize)
  {
    CleO.Show();

    bool Selected = false;
    long DebounceTimer = 0;
    while (Selected == false)
    {
      CleO.Start();
      CleO.StringExt(FONT_SANS_4, (0.50 * ScreenWidth), (0.05 * ScreenHeight), BLACK, MM, 0, 0, "Set Byte Size:");

      itoa (I2CByteSize, CharOutput, 10);
      CleO.StringExt(FONT_SANS_7, (0.50 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, CharOutput);

      DrawSettingsButtons();

      CleO.Show();

      TouchHandler(); //Updates tag with latest touch values

      if (tag == IncrementButton && millis() > DebounceTimer)
      {
        I2CByteSize++;
        DebounceTimer = millis() + 200;
      }
      else if (tag == DecrementButton && millis() > DebounceTimer)
      {
        I2CByteSize--;
        DebounceTimer = millis() + 200;
      }
      else if (tag == DoneButton)
      {
        Selected = true;

      }
      else
      {
      }
    }
    CleO.Start();
  }

  if (tag == I2CScan)
  {
    CleO.Show();

    bool Selected = false;
    byte ScanI2CAddress = 1;
    long ScanTimer = millis() + 300;
    while (Selected == false)
    {
      TouchHandler();

      CleO.Start();

      AnimationLoop();

      CleO.StringExt(FONT_SANS_4, (0.50 * ScreenWidth), (0.05 * ScreenHeight), BLACK, MM, 0, 0, "Scanning I2C Address:");

      itoa (ScanI2CAddress, CharOutput, 10);
      CleO.StringExt(FONT_SANS_7, (0.50 * ScreenWidth), (0.50 * ScreenHeight), BLACK, MM, 0, 0, CharOutput);

      CleO.RectangleColor(GREY);
      CleO.Tag(DoneButton);
      CleO.RectangleXY((0.75 * ScreenWidth), (0.43 * ScreenHeight), 50, 50);
      CleO.StringExt(FONT_SANS_6, (0.80 * ScreenWidth), (0.50 * ScreenHeight), WHITE, MM, 0, 0, "X");

      CleO.Show();

      if (millis() > ScanTimer)
      {
        Wire.requestFrom(ScanI2CAddress, I2CByteSize);
        if (Wire.available() > 0)
        {
          I2CAddress = ScanI2CAddress;
          CleO.Start();
          Selected = true;
        }
        else
        {
          if (ScanI2CAddress == 127)
          {
            ScanI2CAddress = 0;
          }
          else
          {
            ScanI2CAddress++;
          }
          ScanTimer = millis() + 200;
        }
      }

      if (tag == DoneButton)
      {
        CleO.Start();
        Selected = true;
      }
    }

  }
}

void DrawSettingsButtons() //Draws increment and decrement buttons for settings menus
{

  CleO.RectangleColor(GREY);

  CleO.Tag(IncrementButton);
  CleO.RectangleXY((0.40 * ScreenWidth), (0.13 * ScreenHeight), 100, 50);
  CleO.StringExt(FONT_SANS_6, (0.50 * ScreenWidth), (0.20 * ScreenHeight), WHITE, MM, 0, 0, "+");

  CleO.Tag(DecrementButton);
  CleO.RectangleXY((0.40 * ScreenWidth), (0.72 * ScreenHeight), 100, 50);
  CleO.StringExt(FONT_SANS_6, (0.50 * ScreenWidth), (0.80 * ScreenHeight), WHITE, MM, 0, 0, "-");

  CleO.Tag(DoneButton);
  CleO.RectangleXY((0.75 * ScreenWidth), (0.43 * ScreenHeight), 50, 50);
  CleO.StringExt(FONT_SANS_6, (0.80 * ScreenWidth), (0.50 * ScreenHeight), WHITE, MM, 0, 0, "X");

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

void AnimationLoop() //Displays the frame refresh rate
{

  CleO.LineColor(GREY);

  double CurrentSecond = ((millis() / 1000U) % 10); //Gets the seconds in millis
  double CurrentMiliSecond = ((millis() / 100U) % 10);

  CleO.Line((0 * ScreenWidth), (0.985 * ScreenHeight), (((CurrentSecond / 10) + (CurrentMiliSecond / 100)) * ScreenWidth), (0.985 * ScreenHeight));
}

void TouchHandler() //Handles Touch Interactions
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
