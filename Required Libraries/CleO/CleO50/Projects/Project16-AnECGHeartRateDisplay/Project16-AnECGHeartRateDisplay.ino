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
    Project16-AnECGHeartRateDisplay.ino
  Purpose:              
    This project creates a Heart Rate Sensor using the MikroElectronica Heart Rate
    Click. CleO is used to display the current heart rate, to plot the real-time
    sensor data and play a wave file on each detected beat.
    The Heart Rate Click in turn uses the MAX30100 Pulse Oximeter sensor.
  External component:   
    Mikroe Heart Rate module
    CleO Speaker
    REFERENCES:
    1. MAX30100 Pulse Qximeter -
    https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/MAX30100.html
    2. MikroE Heart Rate Click - http://www.mikroe.com/click/heart-rate/
    3. Leaky Integrator - http://www.claysturner.com/dsp/Leaky%20Integrator.pdf
    4. MikroE click Arduino UNO shield - http://www.mikroe.com/click/arduino-uno-shield/
  Hardware Connection:
    Connect CleO Speaker to CleO50 CN2 connector
    HeartRate Click   Nero
    VCC               3.3V
    GND               GND 
    SDA               SDA
    SCL               SCL
*/

#include <SPI.h>
#include <CleO.h>
#include <Wire.h>
#include "click_hr.h"


#define DISP_MAX    (uint16_t)(480)
#define PLOT_SIZ    (byte) (64)
#define HEART_X     (800 - 200 / 2) /* Position of Heart bitmap */
#define HEART_Y     (150)
#define BPM_X       (800 - 200 / 2) /* Position of Heart Rate text */
#define BPM_Y       (330)
#define HR_MAX      (160)           /* Sanity limits for detected HR */
#define HR_MIN      (30)
#define MV_WINDOW_WIDTH     (12)
#define C_DECIMATE_N  (2)

#define DISP_MAX_VALUE      (uint16_t)(480)
#define GRAPH_BUFFER_SIZE   (byte) (64)
int16_t XTicks[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370, 380, 390, 400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530, 540, 550, 560, 570, 580, 590, 600, 610, 620, 630 };
uint8_t H;

/* Bitmap scaling factors for the animating heart */
uint8_t anims[] = { 6, 7, 8, 8, 9, 9, 8, 7, 7, 6, 6, 5, 5 };
#define LAST (sizeof(anims) - 1)
int ha = LAST; /* Counter to keep track of animating heart scaling factor index */
int16_t heart; /* Bitmap handle */
int16_t gVmax, gVmin; /* Max/Min values in the plot buffer */
uint16_t gVpp; /* peak-to-peak range of current plot */
byte sampleIdx; /* Index of samples in the plot buffer */
bool invalidate, Beat; /* Flags to indicate a refresh of display, beat detection */
int16_t samples[PLOT_SIZ], dispBuffer[PLOT_SIZ]; /* Buffers to store raw samples */
uint16_t SampleCnt, LastSampleCnt; /* Count the sample number to calculate HR */
int HRDisp; /* Currently displayed HR */
bool error; /* Set if HR Click errors are detected */

Avg SampleAvg(MV_WINDOW_WIDTH); /* Smoothes the raw data read from the sensor */
Avg DispAvg(8); /* Smoothes the data used for the display plot */
Avg HRAvg(6); /* Averages the calculated Heart Rate */
Filter Filt; /* Object to encapsulate DSP algorithms */
HRClick Click; /* Object to encapsulate HR click functions */


uint8_t DecimateCnt;

/* Initializes the CleO display and configures the HR Click for 100 SPS.
An image of a heart is loaded into CleO memory from the on-board flash. */
void setup() {
  CleO.begin();
  CleO.Start();
  CleO.Show();
	
		
  uint8_t handle = CleO.OpenI2CM(400000, I2C_ADR);		  
  H = handle;
				
	
	
  /* Load the heart bitmap */
  heart = CleO.LoadImageFile("@Pictures/heart.jpg", 0);
  Filt.DC(0, true);
  SampleAvg.Update(0, true);
  DispAvg.Update(0, true);
  Filt.Peak(0, 0, NULL, true);
  Filt.Decide(0, 0, true);
  plot_helper(0, true);

  error = (Click.Init(i2c_write, i2c_write_byte, i2c_read) == true)? false : true; /* Set error flag if init fails */
  invalidate = true; /* force update display once (even if sensor is not present) */
}

/* Polls the sensor for any new data. Each sample is filtered and passed
through a peak detector algorithm. The display is refreshed every new sample.
When a peak was detected, the heart image is animated and a wave file is played.
The average value of the Heart Rate is also shown on screen.
The size of the display buffer is 64 samples. */
void loop() {
  process_click();
  if (!invalidate) return;
  process_graph();
  if (Beat == true) CleO.AudioPlay("@Music/beat.wav", PLAY_ONCE);
  invalidate = false;
  CleO.Start(); /* Plot the graph, animate heart and display new HR */
  CleO.LineWidth(3);
  CleO.LineColor(RED);
  for (int i = 0; i < PLOT_SIZ - 1; i++) {
    /* Don't connect the new signal with the old values in the buffer! */
    if (i != (sampleIdx - 1))
      CleO.Line(XTicks[i], dispBuffer[i], XTicks[i + 1], dispBuffer[i + 1]);
  }
  int hr = HRAvg.GetAvg();
  if ((hr < HR_MAX) && (hr > HR_MIN)) HRDisp = hr;
  /* Animate the Heart */
  if ((Beat == true) && (ha == LAST)) ha = 0;
  else if (ha != LAST) ha++;
  Beat = false;
  /* Select a factor for this cycle */
  CleO.BitmapTransform(anims[ha] / 8., 0);
  /* Change the heart color to white for 2 cycles */
  CleO.BitmapColor((ha < 2) ? WHITE : 0xff2020);
  CleO.BitmapJustification(MM);
  /* Locate the image at (X,Y) = (480 - 162 / 2, 100)*/
  CleO.Bitmap(heart, HEART_X, HEART_Y);
  /* Display the numerical value of the Heart Rate */
  char buf[4];
  sprintf(buf, "%d", HRDisp);
  /* Use the largest font and display string at (X,Y) = (480-162/2, 230) */
  CleO.StringExt(FONT_HUGE, BPM_X, BPM_Y, 0xffffffUL, MM, 0, 0, buf);
  if(error)
    CleO.StringExt(FONT_TINY, 0, 320, RED, BL, 0, 0, "No Heart Rate Click detected");
  CleO.Show(); /* End display update */
}

void i2c_read(byte reg, byte* pData, byte len) {

  CleO.ReadI2CM(H, reg, len, pData);  
	
}

void i2c_write(byte reg, byte* pData, byte len) {

  CleO.WriteI2CM(H, reg, len, pData);  
	
}

void i2c_write_byte(byte reg, byte value) {

  CleO.WriteI2CM(H, reg, 1, &value);  

}
  

/* Processes the samples in the display buffer to make it ready for plotting.
If the peak-to-peak is within twice the display range, the data is scaled to fit.
If the peak-to-peak data is beyond twice display range, a flat line is shown. */
void process_graph(void) {
  plot_helper(0, true);
  for (byte i = 0; i < PLOT_SIZ; i++) plot_helper(samples[i], 0);
  gVpp = abs(gVmax - gVmin);
  if (gVpp <= (DISP_MAX * 2)) {
    for (byte i = 0; i < PLOT_SIZ; i++) 
      dispBuffer[i] = (samples[i] + DISP_MAX) >> 1;
  }
  else if (gVpp <= (DISP_MAX * 4)) {
    for (byte i = 0; i < PLOT_SIZ; i++) 
      dispBuffer[i] = (samples[i] + DISP_MAX) >> 2;
  }
  else {
    for (byte i = 0; i < PLOT_SIZ; i++) 
      dispBuffer[i] = DISP_MAX >> 1; /* flat line */
  }
}

/* Calculate the Vmax and Vmin of the samples to be plotted, to know if the
signal has gone out of range of the display */
void plot_helper(int16_t datum, bool init) {
  if (init) {
    gVmax = 0; gVmin = 0; return;
  }
  if (datum > gVmax) gVmax = datum;
  else if (datum < gVmin) gVmin = datum;
}

/*
1. MAX30100 is polled for new samples
2. Each sample is DC Filtered, rectified, averaged and passed to a peak-detector
3. If the peak-detector detects a new peak, then the new Heart Rate
is calculated and "Beat" flag variable is set to true.
4. If no beats have been detected for 5 seconds, the state is cleared
(the user may have removed the finger)
5. Samples for display are averaged and maintained in a separate display buffer
6. The "invalidate" flag is set whenever a new sample is added to
the display buffer, to trigger the display to refresh.
*/
void process_click(void) {
  byte data, sample_num;
  uint16_t u_datum, dly, pk, hr;
  int16_t i_datum, ir_buff[16];
  /* Read the available data from the sensor */
  sample_num = Click.ReadAvailable(ir_buff);
  for (int i = 0; i < sample_num; i++) { /* For each new sample */
    SampleCnt++;
    i_datum = Filt.DC(ir_buff[i] >> 2, false); /* Filter the DC */
    u_datum = SampleAvg.Update(abs(i_datum), false); /* Average over a moving window */
    pk = Filt.Peak(u_datum, i_datum, &dly, 0); /* Find the peaks */
    /* Check if it's a valid peak, and calculate the Heart Rate */
    if ((pk != 0) && (Filt.Decide(pk, SampleCnt, false) == true)) {
      hr = (uint16_t)((float)SR_HZ * 60.0 /
        ((SampleCnt - dly - LastSampleCnt) & 0xFFFF));
      if ((hr < HR_MAX) && (hr > HR_MIN)) {
        Beat = true; /* Set the beat flag to trigger a heart-beat animation */
        HRAvg.Update(hr, 0);
      }
      LastSampleCnt = SampleCnt - dly;
    }
    else if (((SampleCnt - LastSampleCnt) & 0xFFFF) > SAMPLE_RATE * 5) {
      clear_hr(); /* 5s with no beat - clear HR data */
    }
    /* Store the samples for display */
    DecimateCnt++;
    if (DecimateCnt >= C_DECIMATE_N) {
    if (sampleIdx == PLOT_SIZ) sampleIdx = 0;
    samples[sampleIdx++] = DispAvg.Update(i_datum, false);
    DecimateCnt = 0;
    }
    else {
    DispAvg.Update(i_datum, false);
    }
    invalidate = true; /* Mark the display for update */
  }
}

/* Clear HR average */
void clear_hr(void) {
  HRDisp = 0;
  HRAvg.Update(0, true);
}
