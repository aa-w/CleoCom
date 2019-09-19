
  //Serial Data Read
  int SerialByte = 0;
  String SerialString;
  while (Serial.available() > 0)
  {
    SerialString = Serial.readString();
    //stores the serial read in the circular array
    SerialString.toCharArray(SerialData[SerialPointer], SerialString.length());
  }

  //The data store is circular replacing the oldest value
  if (SerialPointer >= (SerialArraySize - 1))
  {
    SerialPointer++;
  }
  else
  {
    SerialPointer = 0;
  }

  //Serial Data Output
  //CleO.RectangleColor(GREY77);
  //CleO.RectangleXY((0.01 * ScreenWidth), (0.18 * ScreenHeight), 425, 255);

  double charpos = 0.90;
  char SubBuffer [SerialBufferSize];

  for(int i = SerialArraySize; i >= 0; i--)
  {
    //SubBuffer[] = SerialData[i];
    strncpy(SubBuffer,SerialData[i],SerialBufferSize);
    CleO.StringExt(FONT_SANS_4, (0.50 * ScreenWidth), (charpos * ScreenHeight), BLACK, MM, 0, 0, SubBuffer);
    charpos = charpos - 0.20;
  }

  /*
    byte ShiftPos = (SerialArraySize - 2);

    while (ShiftPos > -1)
    {
    //SerialData[ShiftPos] = SerialData[ShiftPos + 1];
    strncpy(SerialData[ShiftPos], SerialData[ShiftPos + 1], SerialBufferSize);
    ShiftPos--;
    }

    strncpy(SerialData[3], SerialData[4], SerialBufferSize);

    strncpy(SerialData[2], SerialData[3], SerialBufferSize);

    strncpy(SerialData[1], SerialData[2], SerialBufferSize);

    strncpy(SerialData[0], SerialData[1], SerialBufferSize);

  */


  //SerialData[0] = "hello 0";
  //SerialData[1] = "hello 1";
  //SerialData[2] = "hello 2";
  //SerialData[3] = "hello 3";
  //SerialData[4][0] = 'h';
