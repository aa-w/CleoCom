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



#include <CleO.h>
#include <SPI.h>

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 480


#define TARGET_CLEO 35
#define OFFSET 200
#define WIDTH  480



void inform(const char *msg)
{
  CleO.Start();
  CleO.StringExt(FONT_TINY, 160, 240, WHITE, MM, 0, 0, msg);
  CleO.Show();
}

void setup()
{
  Serial.begin(115200);
  CleO.begin();

  char buf[80];
  uint16_t version = CleO.Version();
  uint32_t id[2];
  CleO.UnitIdentifier(id[0], id[1]);
#if defined(CLEO_MOCK)
  sprintf(buf, "Version %d.%d.%d, UnitId %08" PRIX32 " %08" PRIX32,
#else
  sprintf(buf, "Version %d.%d.%d, UnitId %08lX %08lX",
#endif
      (version >> 8) & 0xf,
      (version >> 4) & 0xf,
      (version >> 0) & 0xf,
      id[0], id[1]);
  Serial.println(buf);
  int16_t module = CleO.ModuleIdentifier();
  if(module == TARGET_CLEO) {
    int h = CleO.FOpen("System/flash", FILE_READ);
    CleO.InternalEFlashUpdate(h, 0x1d7f);
    CleO.FClose(h);
    CleO.Calibration();
  } else {
    CleO.Start();
    CleO.StringExt(FONT_MEDIUM, OFFSET, WIDTH/2, RED, ML, 0, 0, "Wrong CleO board!");
    CleO.Show();
    while(1);    
  }
}

void loop()
{
  int16_t x, y, dur, tag;
  CleO.TouchCoordinates(x, y, dur, tag);
  CleO.Start();
  CleO.RectangleJustification(TL);
  CleO.RectangleColor(BLACK);
  CleO.RectangleXY(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);   
  CleO.StringExt(FONT_TINY, 160, 240, WHITE, MM, 0, 0, "Load complete");
  if (dur)
    CleO.CircleExt(x, y, 10, 20, DARK_ORANGE, MM, 0, 0);
  CleO.Show();
}
