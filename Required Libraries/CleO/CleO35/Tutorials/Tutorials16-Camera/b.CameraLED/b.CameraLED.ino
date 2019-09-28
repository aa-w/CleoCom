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
    Tutorials16-CameraLED.ino
  Purpose:              
    To desmonstrate using command to control Camera LED
*/

#include <SPI.h>
#include <CleO.h>

#define SCREEN_WIDTH           480 //320
#define SCREEN_HEIGHT          800 //480
#define CENTER_X               (0.5 * SCREEN_WIDTH)

#define TURN_ON_TAG   1
#define TURN_OFF_TAG  2
#define PROGRESS_TAG  3

#define WIDTH	320
#define OFFSET 0


#define PROGRESS_WIDTH (WIDTH - 40)
  
int percent = 0;
void setup() {
  Serial.begin(115200);
  CleO.begin();
}

void loop() {
  drawGui();
  control();
}

void drawGui(void) {
  /* Start building a screen frame */
  CleO.Start();

  CleO.Tag(PROGRESS_TAG);
  CleO.StringExt(FONT_MEDIUM, OFFSET + 10, 100, WHITE, ML, 0, 0, "Dim brightness");
  CleO.eve_slider(OFFSET + 20, 150, PROGRESS_WIDTH, 20, 0, percent, 255);


  CleO.StringExt(FONT_MEDIUM, OFFSET +  10, 300, WHITE, ML, 0, 0, "Set On/Off");
  double button_width =  0.42 * WIDTH;
  double button_height = (0.08 * 480);
  
  CleO.Tag(TURN_ON_TAG);
  CleO.RectangleExt(OFFSET + WIDTH/2 - button_width/2 - 5, 380,
                    button_width, button_height, WHITE, MM, 0, 0);
                    
  CleO.StringExt(FONT_SANS_2, OFFSET + (0.30 * WIDTH),
                 380, BLACK, MM, 0, 0, "Light On");
        
  CleO.Tag(TURN_OFF_TAG);
  CleO.RectangleExt(OFFSET + WIDTH/2 + button_width/2 + 5, 380,
                    button_width, button_height, WHITE, MM, 0, 0);                  
  CleO.StringExt(FONT_SANS_2, OFFSET + (0.70 * WIDTH),
                 380, BLACK, MM, 0, 0, "Light Off");  
  
 
  /* Display completed screen frame */
  CleO.Show();
}



void control(void) {
  int16_t x, y, dur, tag;

  CleO.TouchCoordinates(x, y, dur, tag);
  
  if(dur)
  {
    if(tag == TURN_ON_TAG)
    {
      percent = 255;
    }
    else if(tag == TURN_OFF_TAG)
    {
      percent = 0;
    }
    else if(tag == PROGRESS_TAG)
    {
      if(x < OFFSET + 20) x = OFFSET + 20;
      if(x > OFFSET + PROGRESS_WIDTH) x = OFFSET + PROGRESS_WIDTH;      
      percent = map(x, OFFSET + 20, OFFSET + PROGRESS_WIDTH, 0, 255);
    }
    CleO.CameraLEDS(percent);
  }
  
}