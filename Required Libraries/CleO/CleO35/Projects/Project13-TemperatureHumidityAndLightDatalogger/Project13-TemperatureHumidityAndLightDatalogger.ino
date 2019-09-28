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
    Project13-TemperatureHumidityAndLightDatalogger.ino
  Purpose:              
    To desmonstrate how to implement the three-channel data logger using the CleO primitives.
  External component:   
    HDC1080 Humidity and Temperature Sensor
    CleO Light Sensor
  Hardware Connection:
    HDC1080     NerO      CleOIO 
    Pin 1       GND       Connect to connector CN13 and set JP13 to 5V
    Pin 2       5V
    Pin 3       SDA
    Pin 4       SCL
    LightSensor NerO
    VCC         5V
    GND         GND
    OUT         A0
*/

#include <SPI.h>
#include <CleO.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"

#define SCREEN_WIDTH     320

#define PHOTORESISTOR1_PIN A0

#define HDC1080_ADDR     0x40
#define MANUFACTURER_ID  0x5449
#define DEVICE_ID        0x1050

int nsamples;
static float samples[3][32] = {0.0};
static uint32_t colors[3] = {DEEP_SKY_BLUE, GOLD, WHITE};
char text[][20] = {"Light Sensitivity", "Humidity", "Temperature"};
static float temperature = 0.0;
static float humidity = 0.0;
bool HDC1080_detected = true;
ClosedCube_HDC1080 hdc1080;

void setup() {
  /* Initialize CleO */
  CleO.begin();

  /* Initialize HDC1080 */
  hdc1080.begin(HDC1080_ADDR);

  CleO.DisplayRotate(2, 0);
}

void loop() {
  /*
    Stores all recording samples collected from 3 channels in an array
    Latest samples will be stored at highest index in an array
  */
  CollectTemperatureHumidity();
  addsamples(CollectFromPR1(), humidity, temperature);

  /* Start drawing the screen shot */
  CleO.Start();

  float mn, mx, scale;
  CleO.LineWidth(2);

  /* For each channel */
  for (int chan = 0; chan < 3; chan++) {
    int ybase = 107 + 107 * chan;

    /* Draw a rectangle section in which reading will be drawn */
    CleO.RectangleExt(0, ybase, 400, 90, 0x303030, BL, 0, 0);
    CleO.StringExt(FONT_SANS_1, 400, ybase - 105, colors[chan], TR, 0, 0, text[chan]);

    if (HDC1080_detected == false)
      CleO.StringExt(FONT_SANS_2, 10, SCREEN_WIDTH - 10, RED, BL, 0, 0, "No HDC1080 Detected");

    mn = mx = samples[chan][31];
    float *s = samples[chan];

    /* from all samples in a channel, get min and max */
    for (int i = (32 - nsamples); i < 32; i++) {
      mn = min(mn, s[i]);
      mx = max(mx, s[i]);
    }

    /* Get the scaling value */
    if (mx != mn)
      scale = 90 / (mx - mn);
    else
      scale = 0;

    /* Select differnet color for each channel */
    CleO.LineColor(colors[chan]);

    /* Connect all samples for a channel with a line */
    for (int i = (32 - nsamples); i < 31; i++) {
      float x0 = i * (400. / 31);
      float x1 = (i + 1) * (400. / 31);
      float y0 = ybase - scale * (s[i] - mn);
      float y1 = ybase - scale * (s[i + 1] - mn);
      CleO.Line(x0, y0, x1, y1);
    }

    /* Get the y coordinate for the last sample and use it for drawing reading
      as text */
    char val[9];
    sprintf(val, "%d", int(s[31]));
    float y = ybase - scale * (s[31] - mn);
    CleO.StringExt(FONT_SANS_3, 408, y, colors[chan], BL, 0, 0, val);
  }

  /* Show the screenshot */
  CleO.Show();
  delay(400);
}

void addsamples(float s0, float s1, float s2) {
  for (int i = 0; i < 31; i++) {
    samples[0][i] = samples[0][i + 1];
    samples[1][i] = samples[1][i + 1];
    samples[2][i] = samples[2][i + 1];
  }
  samples[0][31] = s0;
  samples[1][31] = s1;
  samples[2][31] = s2;
  if (nsamples < 32)
    nsamples += 1;
}

float CollectFromPR1() {
  return analogRead(PHOTORESISTOR1_PIN);
}

void CollectTemperatureHumidity() {
  /* Collect temperature and humidity */
  int ManufacturerId = hdc1080.readManufacturerId();
  int DeviceId = hdc1080.readDeviceId();
  temperature = hdc1080.readTemperature();
  humidity= hdc1080.readHumidity();

  HDC1080_detected = true;

  if (!(ManufacturerId == MANUFACTURER_ID && DeviceId == DEVICE_ID)) {
    HDC1080_detected = false;
    temperature = 0.0;
    humidity = 0.0;
  }
}
