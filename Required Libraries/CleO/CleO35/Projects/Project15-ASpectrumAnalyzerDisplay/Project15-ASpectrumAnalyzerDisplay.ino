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
    Project15-ASpectrumAnalyzerDisplay.ino
  Purpose:              
    To desmonstrate how to draw a spectrum analyzer using the CleO Graphics Primitive bitmaps and FFT Arduino library.
  External component:   
    Microphone IO
  Hardware Connection:
    MicrophoneIO    NerO
    VCC             IOREF
    GND             GND
    OUT             A0
*/

#include <SPI.h>
#include <CleO.h>

#define LOG_OUT 1 // use the log output function
#define FFT_N 128

#include <FFT.h> // include the library

int16_t h[16];

void setup() {
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0

  CleO.begin();
  CleO.DisplayRotate(2, 0);

  /* Load 16 bitmaps to form spectrum */
  h[0] = CleO.LoadImageFile("@Pictures/Spectrum/0.bmp", 0);
  h[1] = CleO.LoadImageFile("@Pictures/Spectrum/1.bmp", 0);
  h[2] = CleO.LoadImageFile("@Pictures/Spectrum/2.bmp", 0);
  h[3] = CleO.LoadImageFile("@Pictures/Spectrum/3.bmp", 0);
  h[4] = CleO.LoadImageFile("@Pictures/Spectrum/4.bmp", 0);
  h[5] = CleO.LoadImageFile("@Pictures/Spectrum/5.bmp", 0);
  h[6] = CleO.LoadImageFile("@Pictures/Spectrum/6.bmp", 0);
  h[7] = CleO.LoadImageFile("@Pictures/Spectrum/7.bmp", 0);
  h[8] = CleO.LoadImageFile("@Pictures/Spectrum/8.bmp", 0);
  h[9] = CleO.LoadImageFile("@Pictures/Spectrum/9.bmp", 0);
  h[10] = CleO.LoadImageFile("@Pictures/Spectrum/10.bmp", 0);
  h[11] = CleO.LoadImageFile("@Pictures/Spectrum/11.bmp", 0);
  h[12] = CleO.LoadImageFile("@Pictures/Spectrum/12.bmp", 0);
  h[13] = CleO.LoadImageFile("@Pictures/Spectrum/13.bmp", 0);
  h[14] = CleO.LoadImageFile("@Pictures/Spectrum/14.bmp", 0);
  h[15] = CleO.LoadImageFile("@Pictures/Spectrum/15.bmp", 0);
}

/* Read analog input from A0 and perform FFT of 128 samples */
void loop() {
  while (1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0; i < (FFT_N * 2); i += 2) { // save 256 samples
      while (!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i + 1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();
    draw();
  }
}

/* Draw 13 bitmaps in landscape mode with scaled frequency values */
void draw() {
  CleO.Start();
  for (int i = 0; i < 13; i++) {
    int lg = fft_log_out[2 + 1 * i];
    int sz = max(0, min(15, (lg - 0x40) / 8));
    CleO.Bitmap(h[sz], 6 + (36 * i), 0);
  }
  CleO.Show();
}