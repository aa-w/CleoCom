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
    Project17-ADigitalFMRadio.ino
  Purpose:              
    To demonstrate an FM radio using the radio component
  External component:   
    RDA5870M module on CleOIO board
  Hardware Connection:
    Connect CleOIO board to NerO
    
*/

#include <SPI.h>
#include <CleO.h>
#include <RDA5807M.h>

#define SCREEN_WIDTH     320

#define FIX_BAND     RADIO_BAND_FM   ///< The band that will be tuned by this sketch is FM.

int16_t spk1;     // Speaker icon
int channel;  // FM channel in tenths, 880 to 1080
int volume;   // Headphone volume 0-15
bool hardwarePresent = true;

RDA5807M radio;    // Create an instance of Class for RDA5807M Chip

void setup() {
  Serial.begin(115200);

  /* Initialize CleO */
  CleO.begin();
  spk1 = CleO.LoadIcon("@Icons/m48.ftico", ICON_VOLUME_UP);
  CleO.DisplayRotate(2, 0);
  inform("Connecting to radio...");
  // Initialize the Radio
  if (radio.init() == false)
    hardwarePresent = false;

  // Enable information to the Serial port
  radio.debugEnable();

  volume = 5;
  channel = 897;

  if (hardwarePresent == true) {
    // Set all radio setting to the fixed values.
    radio.setBandFrequency(FIX_BAND, channel * 10);
    radio.setVolume(volume);
    radio.setMono(false);
    radio.setMute(false);
  }
  inform("ready");
}

void loop() {
  displayRadio();
  control();

  //  char s[12];
  //  radio.formatFrequency(s, sizeof(s));
  //  Serial.print("Station:");
  //  Serial.println(s);
  //
  //  Serial.print("Radio:");
  //  radio.debugRadioInfo();
  //
  //  Serial.print("Audio:");
  //  radio.debugAudioInfo();
  //
  //  delay(3000);
}

void inform(const char *msg) {
  CleO.Start();
  CleO.StringExt(FONT_MEDIUM, 240, SCREEN_WIDTH/2, WHITE, MM, 0, 0, msg);
  CleO.Show();
}

// Rescale x from the range (x0, x1) to the range (y0, y1)
float rescale(int x, int x0, int x1, int y0, int y1) {
  x = max(x0, min(x1, x));
  return y0 + ((x - x0) * long(y1 - y0) / float(x1 - x0));
}

float dial(int ch) {
  // The tuner supports the FM band 87.5 to 108.0 MHz
  // So channel has range 875 to 1080
  return rescale(ch, 875, 1080, 10, 480 - 10);
}

void displayRadio() {
  /* Start drawing the screen shot */
  CleO.Start();

  CleO.LineWidth(1);
  for (int f = 880; f < 1080; f += 20) {
    char buf[4];
    sprintf(buf, "%d", f / 10);
    CleO.StringExt(FONT_SANS_2, dial(f), 120, LIGHT_GREEN, MM, 0, 0, buf);
  }
  CleO.Tag(1);
  CleO.LineColor(DARK_GREEN);
  for (int f = 880; f < 1080; f += 2) {
    int major = (f % 10) == 0;
    CleO.LineWidth(major ? 1.2 : 0.8);
    CleO.Line(dial(f), 60, dial(f), 90 + 10 * major);
  }

  CleO.NeedleColor(DARK_RED);
  CleO.NeedleWidth(10);
  CleO.NeedleXY(dial(channel), 20, 60, 0);

  CleO.Tag(2);

  /* Draw drak orange volume line with 3 thickness along with circle which
    will be used to touch and drag */
  CleO.LineWidth(4);
  CleO.LineColor(DARK_ORANGE);
  CleO.Line(300, 280, 440, 280);
  CleO.CircleExt(rescale(volume, 0, 15, 300, 440), 280, 0, 20, DARK_ORANGE,
                 MM, 0, 0);

  CleO.Tag(3);
  CleO.StringExt(FONT_MEDIUM, 10, 150, WHITE, TL, 0, 0, "Previous");
  CleO.Tag(4);
  CleO.StringExt(FONT_MEDIUM, 445, 150, WHITE, TR, 0, 0, "Next");

  CleO.Tag(5);
  CleO.BitmapJustification(MM);
  CleO.Bitmap(spk1, 240, 280);

  /* Draw channel frequency using string */
  CleO.Tag(6);
  char buf[8];
  sprintf(buf, "%d.%d", channel / 10, channel % 10);
  CleO.StringExt(FONT_SANS_6, 10, 280, WHITE, ML, 0, 0, buf);

  if (hardwarePresent == false)
    CleO.StringExt(FONT_SANS_2, 10, SCREEN_WIDTH - 10, RED, BL, 0, 0, "No RDA5807M Detected");
  /* Show the screenshot */
  CleO.Show();
}

/* set selected channel, volume, seek up and down channels based on touch input */
void control() {
  int16_t x, y, dur, tag, newval;
  CleO.TouchCoordinates(x, y, dur, tag);
  if(!dur) {    
    return;     //not handle invalid inputs
  }
  if (tag == 1) {
    newval = int(rescale(x, 10, 480 - 10, 875, 1080));
    if (channel != newval) {
      channel = newval;
      if (hardwarePresent == true)
        radio.setBandFrequency(FIX_BAND, channel * 10);
    }
  }
  else if (tag == 2) {
    newval = int(rescale(x, 300, 440, 0, 15));
    if (volume != newval) {
      volume = newval;
      if (hardwarePresent == true) {
        radio.setVolume(volume);
      }
    }
  }
  else if (tag == 3 && hardwarePresent == true) {
    radio.seekDown(false);
    delay(50);      //wait for radio chip to switch to new freq
    RADIO_FREQ f = radio.getFrequency();
    channel = f / 10;
//    Serial.println(f);
  }
  else if (tag == 4 && hardwarePresent == true) {
    radio.seekUp(false);
    delay(50);      //wait for radio chip to switch to new freq
    RADIO_FREQ f = radio.getFrequency();
    channel = f / 10;
//    Serial.println(f);
  }
}
