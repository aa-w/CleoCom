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



/*
  Tutorial Name: Animated Bitmaps
  Description: This tutorial shows how to draw animation with bouncing ball.
*/
#include <SPI.h>
#include <CleO.h>

int screenWidth = 320, screenHeight = 480;
int ballMaxX = screenWidth - 50, ballMaxY = screenHeight - 50;
int ballX = ballMaxX / 2, ballY = 0;
bool flagX = false, flagY = false;
int deltaX = 1, deltaY = 5;
int16_t handle;

void setup()
{
  /* Initialize CleO - needs to be done only once */
  CleO.begin();

  /* Get the handle by loading bitmap */
  handle = CleO.LoadImageFile("@Pictures/ball.png", 0);;

  CleO.SetBackgroundcolor(WHITE);
}

void loop()
{
  /* Start building a screen frame */
  CleO.Start();

  /* draw a bitmap at (x, y) coordinate */
  CleO.Bitmap(handle, ballX, ballY);

  /* When ball hits the height boundaries */
  if (ballY >= ballMaxY || ballY <= 0)
    flagY = !flagY;

  /* When ball hits the side boundaries */
  if (ballX >= ballMaxX || ballX <= 0)
    flagX = !flagX;

  /* Update ball x-coordinate */
  if (flagX)
    ballX += deltaX;
  else
    ballX -= deltaX;

  /* Update ball y-coordinate */
  if (flagY)
    ballY += deltaY;
  else
    ballY -= deltaY;

  /* Display completed screen frame */
  CleO.Show();
}