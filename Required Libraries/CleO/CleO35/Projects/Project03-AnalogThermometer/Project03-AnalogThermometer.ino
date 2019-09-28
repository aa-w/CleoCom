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
    Project03-AnalogThermometer.ino
  Purpose:              
    To desmonstrate how to create analog thermometer using the CleO primitives
  External component:   
    HDC1080 Humidity and Temperature sensor
  Hardware Connection:  
    HDC1080     NerO      CleOIO 
    Pin 1       GND       Connect to connector CN13 and set JP13 to 5V
    Pin 2       5V
    Pin 3       SDA
    Pin 4       SCL
*/

#include <SPI.h>
#include <CleO.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"

#define SCREEN_WIDTH     320
#define SCREEN_HEIGHT    480

#define UNIT_TAG         1
#define ALERT_TAG        2
#define DIGITS_MIN       (-10)
#define DIGITS_MAX       (80)

#define CENTRE_X         (0.5 * SCREEN_WIDTH)
#define CENTRE_Y         (0.5 * SCREEN_HEIGHT)

#define HDC1080_ADDR     0x40
#define MANUFACTURER_ID  0x5449
#define DEVICE_ID        0x1050

boolean showCelsius = true;
char buffer[10];
int16_t previousTag = 0, previousDur = 0;

ClosedCube_HDC1080 hdc1080;

void setup() {
  Serial.begin(115200);

  /* Initialize HDC1080 */
  hdc1080.begin(HDC1080_ADDR);

  /* Initialize CleO */
  CleO.begin();
}

void loop() {
  touchControl();

  //  Serial.print("Manufacturer ID=0x");
  //  Serial.println(hdc1080.readManufacturerId()); // 0x5449 ID of Texas Instruments
  //  Serial.print("Device ID=0x");
  //  Serial.println(hdc1080.readDeviceId()); // 0x1050 ID of the device

  int ManufacturerId = hdc1080.readManufacturerId();
  int DeviceId = hdc1080.readDeviceId();

  //  Serial.print("T=");
  //  Serial.print(hdc1080.readTemperature());
  //  Serial.print("C, RH=");
  //  Serial.print(hdc1080.readHumidity());
  //  Serial.println("%");

  /* Read temperature from DHT22 */
  float tempC = hdc1080.readTemperature();
  float humidity = hdc1080.readHumidity();

  /* Start drawing thermometer */
  CleO.Start();

  /* Draw rectangle cover whole screen */
  CleO.RectangleExt(CENTRE_X, CENTRE_Y, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE, MM, 0, 0);

  /* Draws toggle switch to set display unit of temperature */
  drawReading(tempC, humidity);
  drawReadingToggle();

  /* Draws measurement, units and shows tempC as an indicator */
  drawThermometer(tempC);

  if (!(ManufacturerId == MANUFACTURER_ID && DeviceId == DEVICE_ID))
    CleO.StringExt(FONT_SANS_2, 10, SCREEN_HEIGHT - 10, RED, BL, 0, 0, "No HDC1080 Detected");

  CleO.Show();

  /* slow down the rate of reading from thermometer */
  delay(500);
}

void touchControl() {
  int16_t x, y, dur, tag;
  CleO.TouchCoordinates(x, y, dur, tag);

  /* pen down */
  if (previousDur == 0 && dur != 0) {
    previousTag = tag;
    previousDur = 1;
  }
  /* pen up */
  else if (previousDur == 1 && dur == 0) {
    /* If unit toggle is touched, alter display unit for temperature */
    if (previousTag == UNIT_TAG)
      showCelsius = !showCelsius;

    previousDur = 0;
  }
}

/* Displays current temperature and humidity readings */
void drawReading(float tempC, float humidity) {
  float reading = tempC;

  if (showCelsius == false)
    reading = (1.8 * tempC) + 32;

  String displayText = String(reading);
  displayText.toCharArray(buffer, 10);
  CleO.StringExt(FONT_SANS_4, 225, (SCREEN_HEIGHT - 90), BLACK, TL, 0, 0, buffer);

  displayText = String(humidity);
  displayText += " %";
  displayText.toCharArray(buffer, 10);
  CleO.StringExt(FONT_SANS_3, 215, (SCREEN_HEIGHT - 200), BLACK, TL, 0, 0, "Humidity");
  CleO.StringExt(FONT_SANS_3, 225, (SCREEN_HEIGHT - 170), BLACK, TL, 0, 0, buffer);
}

/* Displays toggle button to select Celsius or Fahrenheit unit */
void drawReadingToggle() {
  CleO.eve_bgcolor(YELLOW);

  /* Assign tag and based on current unit status, displays toggle switch */
  CleO.Tag(UNIT_TAG);
  if (showCelsius == true)
    CleO.eve_toggle(235, (SCREEN_HEIGHT - 45), 50,
                    FONT_MEDIUM, OPT_FLAT, 0, "Cel" "\xff" "Fer");
  else
    CleO.eve_toggle(235, (SCREEN_HEIGHT - 45), 50,
                    FONT_MEDIUM, OPT_FLAT, 65535, "Cel" "\xff" "Fer");
}

void drawReading(int tempC) {
  CleO.PointExt(110, 445, 25, YELLOW, 0, 0);

  int negativeReadingY = 0, positiveReadingY = 0;
  if (tempC >= 0) {
    positiveReadingY = 25 - (4 * (DIGITS_MIN)) + (4 * tempC);
    CleO.RectangleExt(110, 445, 12, positiveReadingY, YELLOW, BM, 0, 0);
  } else {
    negativeReadingY = 25 + ((tempC - DIGITS_MIN) * 4);
    CleO.RectangleExt(110, 445, 12, negativeReadingY, YELLOW, BM, 0, 0);
  }
}

void drawMeasurement() {
  int lineLength, lineY = 420;
  CleO.LineColor(BLACK);
  CleO.LineWidth(1);

  /* For each temperature reading on thermometer line */
  for (int startTemp = DIGITS_MIN;
       startTemp <= DIGITS_MAX;
       startTemp += 1) {
    if (startTemp % 10 == 0)
      lineLength = 30;
    else if (startTemp % 5 == 0)
      lineLength = 20;
    else
      lineLength = 10;

    int temFLineX1 = 130 - lineLength - (2 * 20);;

    /* Draw line for celsius and fahrenheit unit */
    CleO.Line(130, lineY, 130 + lineLength, lineY);
    CleO.Line(temFLineX1, lineY, 90, lineY);

    if (startTemp % 10 == 0) {
      String displayText = String(startTemp);
      displayText.toCharArray(buffer, 6);

      /* Draw celsius unit text */
      CleO.StringExt(FONT_SANS_1, 170, lineY, BLACK, ML, 0, 0, buffer);

      displayText = String((1.8 * startTemp) + 32);
      displayText.toCharArray(buffer, 6);

      /* Draw fahrenheit unit text */
      CleO.StringExt(FONT_SANS_1, temFLineX1 - 10, lineY, BLACK, MR, 0, 0, buffer);
    }

    lineY -= 4;
  }
}

void drawThermometer(float tempC) {
  /* Draws base circle of thermometer using point */
  CleO.PointExt(110, 445, 25, DEEPSKYBLUE, 0, 0);

  /* Draws thermometer vertical line which diplays reading level */
  CleO.RectangleExt(110, 445, 12, 430, DEEPSKYBLUE, BM, 0, 0);
  CleO.PointExt(110, 415, 6, DEEPSKYBLUE, 0, 0);

  /* Draws unit lines on vertical bar of thermometer */
  drawMeasurement();

  /* Draws units (text) on vertical bar of thermometer */
  CleO.StringExt(FONT_SANS_3, 90, 15, BLACK, TR, 0, 0, "F");
  CleO.StringExt(FONT_SANS_3, 130, 15, BLACK, TL, 0, 0, "C");

  /* Draws reading level on measurement line */
  drawReading(tempC);
}
