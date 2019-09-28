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
    Project20-AnLEDDisplay.ino
  Purpose:              
    Code for running 4 demo animations on the MikroE 8x8 Click
    The display on the CleO uses 4 built-in button widgets
    User selects the animation to run by pressing the buttons on CleO
  External component:   
    Mikroe 8X8 LED Click
    REFERENCES:
    1. Alien Animation - https://learn.adafruit.com/3d-printed-led-animation-bmo/led-animations
    2. Pacman Video - https://www.youtube.com/watch?v=4R3psup26F4
    3. MikroE 8x8 Click - http://www.mikroe.com/click/8x8-r/
    4. MikroE click Arduino UNO shield - http://www.mikroe.com/click/arduino-uno-shield/
  Hardware Connection:
    LED Click         Nero
    5V                5V
    GND               GND
    SDI               ~11
    SDO               12
    SCK               13
    CS                8
*/

#include <SPI.h>
#include <CleO.h>
#include <setjmp.h>

#include "click_8x8.h"


#define WDTH 300
#define HGHT 150
#define X0 ((800 - 2*WDTH)/2)
#define Y0 ((480 - 2*HGHT)/2)

uint8_t H;


/* Arduino Digital pin used to drive the Chip Select on the Click */
#define CLICK_CS_PIN 8
#define sel()     digitalWrite(CLICK_CS_PIN, LOW)
#define unsel()   digitalWrite(CLICK_CS_PIN, HIGH)



#define TAG_PACMAN    200 /* Tags to ID each button */
#define TAG_SPIRAL    201
#define TAG_ALIEN     202
#define TAG_STRING    203
#define OPT_FLAT  ((uint16_t)256)

LedClick Click; /* An object to encapsulate the 8x8 Click */
uint8_t image_buffer[8]; /* Buffer to store the image while scroling */
uint8_t State; /* Selected animation */
jmp_buf jump_buffer; /* Buffer to save setjump() context */
/* Coordinates for the buttons */
int16_t btn_xy[4*2] = { X0, Y0, X0, Y0 + HGHT,X0 + WDTH, Y0 + HGHT,X0 + WDTH, Y0 };
/* Strings to display for each button */
const char* btn_txt[4] = { "Pacman", "Spiral","Alien","String" };
/* Pre-computed row and column indices to generate a spiral */
byte spiral_idx[64] = { 0x0,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x71,0x72,0x73,
0x74,0x75,0x76,0x77,0x67,0x57,0x47,0x37,0x27,0x17,0x7,0x6,0x5,0x4,0x3,0x2,0x1,
0x11,0x21,0x31,0x41,0x51,0x61,0x62,0x63,0x64,0x65,0x66,0x56,0x46,0x36,0x26,
0x16,0x15,0x14,0x13,0x12,0x22,0x32,0x42,0x52,0x53,0x54,0x55,0x45,0x35,0x25,
0x24,0x23,0x33,0x43,0x44,0x34 };


 /* Initializes CleO and the 8x8 Matrix (MAX7219) to "normal mode" */
void setup(){

  
  pinMode(CLICK_CS_PIN, OUTPUT);
  unsel();
  CleO.begin();
  Click.Initialize(WRITE, false);
  

  /* Initialize the CleO display */
  CleO.Start();
  CleO.Show();

  State = TAG_PACMAN;
}

/* Each animation is an independent state which loops continuously until the user
selects a new animation. The currently selected button is rendered as a "flat"
button. setjump() and longjump() are used to return back to the loop() once a
touch is detected in any of the states. */
void loop(){
  setjmp(jump_buffer);  /* Jump here on detecting a new touch */
  clear_matrix();       /* Start from a clear state */
  update_cleo(State);   /* Update CleO display based on current state */
  /* Run animcations */
  if(State == TAG_PACMAN) run_pacman();
  else if (State == TAG_SPIRAL) run_spiral();
  else if(State == TAG_ALIEN) run_aliens();
  else if(State == TAG_STRING) run_string();
  else get_touch();
}

/* Updates the CleO display based on the currently selected button */
void update_cleo(int16_t tag) {
  int cx = 400;
  int cy = 240;
  CleO.Start();
  for (int i = 0; i < 4; i++) {
    CleO.EnableTag(true);
    CleO.Tag(TAG_PACMAN + i);
    int16_t opt  = ((tag > 0) && ((tag - TAG_PACMAN) == i)) ? OPT_FLAT : 0;
    CleO.eve_button(btn_xy[i * 2], btn_xy[i * 2 + 1], WDTH, HGHT, FONT_MEDIUM, opt, btn_txt[i]);
    CleO.EnableTag(false);
  }
  CleO.SetBackgroundGradient(0, 0, cx * 2, cy * 2, DARK_BLUE, DARK_RED);
  CleO.Show();
}

/*  All the bitmaps/keyframes are stored in click_8x8_font.h */
void run_pacman(void) {
  keyframe_scroll_left(chase_fow, sizeof(chase_fow));
  keyframe_play(look_out, sizeof (look_out), TAG_PACMAN);
  keyframe_scroll_left(chase_fow + 8, 8);
  keyframe_scroll_right(chase_rev, sizeof(chase_rev));
}

/* To generate the spiral, (row,column) indices for the spiral are precomputed
and stored in spiral_idx[]  */
void run_spiral(void) {
  for (int i = 0; i < 64; i++) {
    Click.LightPixel(spiral_idx[i] >> 4, spiral_idx[i] & 0x0F);
    wait_for(50, TAG_SPIRAL);
  }
  for (int i = 63; i >= 0; i--) {
    Click.ClearPixel(spiral_idx[i] >> 4, spiral_idx[i] & 0x0F);
    wait_for(50, TAG_SPIRAL);
  }
}

/*  Runs the animation for the Display String  */
void run_string(void) {
  keyframe_play(&display_str[0][0], sizeof(display_str), TAG_STRING);
}

/* Based on the adafruit demo -
https://learn.adafruit.com/3d-printed-led-animation-bmo/led-animations  */
void run_aliens(void) {
  keyframe_play(aliens, sizeof(aliens), TAG_ALIEN);
}

/* Wait either for the delay OR a touch event. */
void wait_for(uint16_t delay_ms, int16_t tag ) {
  uint16_t cnt = 0, t;
  while (cnt < delay_ms) {
    delay(1); cnt++;
    t = get_touch();
    if ( (t != 0) && (t != tag)) {
      longjmp(jump_buffer, 1); /* Exit state */
      return;
    }
  }
}

/* Returns the tag of the selected button, 0 if no button is selected. */
byte get_touch(void) {
  int16_t x, y, dur, tag;
  CleO.TouchCoordinates(x, y, dur, tag);
  if (dur && (tag != 0xFFFF)) {
  State = tag;
    return tag;
  }
  else return 0;
}

/* Clears the 8x8 Matrix screen and sets the CleO display to default state. */
void clear_matrix(void) {
  uint8_t clr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  Click.ShowImage(clr);
}

/* Scroll the keyframe right column by column */
void keyframe_scroll_right(const uint8_t PROGMEM * frame, uint16_t len) {
  for (int i = 0; i < len ; i++) {
    for (int j = 0; j < 7; j++) image_buffer[j] = image_buffer[j + 1];
    image_buffer[7] = pgm_read_byte(&frame[i]);
    Click.ShowImage(image_buffer);
    wait_for(50, TAG_PACMAN);
  }
}

/* Scroll the keyframe left column by column */
void keyframe_scroll_left(const uint8_t PROGMEM * frame, uint16_t len) {
  for (int i = 0; i < len; i++) {
    for (int j = 7; j >= 0; j--) image_buffer[j] = image_buffer[j - 1];
    image_buffer[0] = pgm_read_byte(&frame[i]);
    Click.ShowImage(image_buffer);
    wait_for(50, TAG_PACMAN);
  }
}

/* Display each image in the keyframe, and wait for the configured delay */
void keyframe_play(const uint8_t PROGMEM * frame, uint16_t len, uint8_t tag) {
  for (int i = 0; i < len / 9; i++) {
    Click.ShowImage(&frame[i*9]);
    wait_for(pgm_read_byte(&frame[i*9 + 8]) * 10, tag);
  }
}

/* Writes a bunch of data to the SPI slave */
void WRITE(uint8_t len, uint8_t* pData) {


  sel();
  for (int i = 0; i < len; i++) {
    SPI.transfer(pData[i]);
  }
  unsel();

}
