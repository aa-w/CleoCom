
#ifndef _CLICK8x8_H_
#define _CLICK8x8_H_

#ifdef ARDUINO // Include in the arduino sketch, not on FT900
#include "click_8x8_font.h"
#endif

#define NO_OP  0
#define DIGIT_0 1
#define DIGIT_1 2
#define DIGIT_2 3
#define DIGIT_3 4
#define DIGIT_4 5
#define DIGIT_5 6
#define DIGIT_6 7
#define DIGIT_7 8
#define DECODE_MODE 9
#define INTENSITY 0xA
#define SCAN_LIMIT 0xB
#define SHUTDOWN 0xC
#define DISPLAY_TEST 0xF

#define SHDN_NORMAL_OPERATION 1
#define SHDN_SHUTDOWN 0
#define NO_DECODE_MODE  0
#define INTENSITY_MAX   0x0F
#define INTENSITY_MID 0x04
#define INTENSITY_LOW 0x01
#define SCAN_ALL    0x7 // enable all 8 digit lines, segments are controlled by the values in the digit register
#define NORMAL_MODE 0x00

typedef  void (*WriteHook) (uint8_t len, uint8_t* buf);
typedef uint16_t (*ReadHook) (uint8_t len, uint8_t* buf);
static unsigned char lookup[16] = {
  0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
  0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf,
};

class LedClick {
private:
  uint8_t matrix[8];
  WriteHook WRITE;
  bool rotate; // set true for cleo50 to align led display with CleO silk screen image
public:
  void Initialize(WriteHook w, bool rot) {
    WRITE = w;
    rotate = rot;
    Serial.begin(115200);
    delay(1000); /* MAX7219 seems to require a delay after power up */
    uint8_t d88[2] = { 0, 0 };
    /* Set up MAX7219 for no-decode operation */
    d88[0] = DISPLAY_TEST; d88[1] = NORMAL_MODE; WRITE(2, d88);
    d88[0] = INTENSITY; d88[1] = INTENSITY_LOW; WRITE(2, d88);
    d88[0] = DECODE_MODE; d88[1] = NO_DECODE_MODE; WRITE(2, d88);
    d88[0] = SCAN_LIMIT;  d88[1] = SCAN_ALL; WRITE(2, d88);
    d88[0] = SHUTDOWN; d88[1] = SHDN_NORMAL_OPERATION; WRITE(2, d88);
    unveil();
  }

  /*  Light a pixel on the 8x8 Matrix */
  void LightPixel(uint8_t row, uint8_t col) {
    uint8_t d88[2] = { 0, 0 };
    d88[0] = col + 1;
    matrix[col] = matrix[col] | (0x01 << row);
    d88[1] = matrix[col];
    WRITE(2, d88);
  }

  /* Clear a pixel on the 8x8 Matrix */
  void ClearPixel(uint8_t row, uint8_t col) {
    uint8_t d88[2] = { 0, 0 };
    d88[0] = col + 1;
    matrix[col] = matrix[col] & (~(0x01 << row));
    d88[1] = matrix[col];
    WRITE(2, d88);
  }

  void ClearState(void) {
    for (int i = 0; i < 8; i++) matrix[i] = 0;
  }

  /* Write a column to 8x8 matrix. */
  void ShowColumn(char col, char col_num) {
    uint8_t d88[2] = { 0, 0 };
    d88[0] = col_num + 1;   d88[1] = col;
    WRITE(2, d88);
  }

  /*  display image on LED Martix from 8 byte array in SRAM. */
  void ShowImage(uint8_t *image) {
    for (int i = 7; i >= 0; i--) {
      if(rotate)
        ShowColumn(reverse(image[i]), i);
      else
        ShowColumn((image[i]), i);
    }
  }

  /*  display image on LED Martix from 8 byte array in PROGMEM. */
  void ShowImage(const uint8_t PROGMEM *image) {
    for (int i = 7; i >= 0; i--) {
      if(rotate)
        ShowColumn(reverse(pgm_read_byte(&image[i])), i);
      else
        ShowColumn((pgm_read_byte(&image[i])), i);
    }
  }

private:

uint8_t reverse(uint8_t n) {
  // Reverse the top and bottom nibble then swap them.
  return (lookup[n & 0b1111] << 4) | lookup[n >> 4];
}
    
  /* Lights and clears every pixel on the 8x8 LED Matrix */
  void unveil(void) {
    for (int i = 7; i >= 0; i--) {
      for (int j = 7; j >= 0; j--) {
        LightPixel(i, j);
        delay(10);
      }
    }
    for (int i = 7; i >= 0; i--) {
      for (int j = 7; j >= 0; j--) {
        ClearPixel(i, j);
        delay(10);
      }
    }
  }
};

#endif