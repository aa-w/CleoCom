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
    Project21-ARotaryDisplay.ino
  Purpose:              
    This project MikroElectronica Rotary Click to implement a Rotary Switch.
    CleO is used to display the current selection of the switch.
  External component:   
    Mikroe Rotary Click
    REFERENCES:
    1. MikroE Rotary Click - http://www.mikroe.com/click/rotary-r/
    2. Rotary switch data sheet - http://www.alps.com/prod/info/E/PDF/Switch/Encoder/EC12E/EC12E.PDF
    3. 74HC595 Shift register data sheet - https://www.nxp.com/documents/data_sheet/74HC_HCT595.pdf
    4. MikroE click Arduino UNO shield - http://www.mikroe.com/click/arduino-uno-shield/
  Hardware Connection:
    Rotary Click      Nero
    ECA               ~9
    SW                2
    5V                5V
    GND               GND
    3.3V              3.3V
    SDI               ~11
    SDO               12
    SCK               13
    RST               4
    ECB               A0
*/

#include <SPI.h>
#include <CleO.h>


  
/* PIN mapping for the click and arduino */
#define PIN_CS    8
#define PIN_ENCA  9
#define PIN_ENCB  A0
#define PIN_SW    2

#define sel()                   digitalWrite(PIN_CS, LOW)
#define unsel()                 digitalWrite(PIN_CS, HIGH)
  



#define DETENTS 16 /* Number of detents on the encoder */
#define BDR   5    /* 5 pixels for margin */
#define DIA   40   /* Diameter of small circles */
#define WDTH  20   /* Width of big circle */

uint16_t Led_State, Prev_LED_State; /* Current and previous state of LEDs on the click */
uint8_t ENCA_bit, ENCB_bit, SW_bit, Switch_state; /* Cached state of pins */
uint16_t Led_Init[] = { 0x0001, 0x0101, 0x1111, 0x5555 }; /* Initial state of LEDs */

#define Z_BitIsSet(val, mask)   (((val) & (mask)) == (mask))

/* Initializes the CleO display and configures the GPIO pins used by the Rotary
Click. ENCA, ENCB and SW are inputs used for detecting the rotation and switch
press. Aditionally, a Chip Select output pin is required for driving the
74HC595 shift register over SPI.*/
void setup() {
  CleO.begin();

  
  pinMode(PIN_ENCA, INPUT);
  pinMode(PIN_ENCB, INPUT);
  pinMode(PIN_SW, INPUT);
  pinMode(PIN_CS, OUTPUT);
  unsel();
  
  Led_State = Led_Init[0];
  light_leds(0x0001);
}

/* Polls the ENCA, ENCB and SW pins for any level changes.
The relative phase between ENCA and ENCB indicates the direction of rotation.
The CleO display is updated if there is a change in status of the LEDs.*/
void loop() {
  uint8_t ENCA, ENCB, SW;
  /* Encoder processing */
  
    
  ENCA = digitalRead(PIN_ENCA);
  ENCB = digitalRead(PIN_ENCB);
      
  /* If logic state on pin A or pin B change, rotate the LEDs */
  if ((ENCA_bit != ENCA) || (ENCB_bit != ENCB)) {
    if (ENCB_bit == 1 || ENCA == 1) {
        Led_State = (Led_State << 1) | (Led_State >> 15);
    }
    if (ENCA_bit == 1 || ENCB == 1) {
        Led_State = (Led_State >> 1) | (Led_State << 15);
    }
    ENCA_bit = ENCA; /* Save state */
    ENCB_bit = ENCB;
    light_leds(Led_State);
  }

  /* Switch processing */
  
    
  SW = digitalRead(PIN_SW);
    
  if (SW && (SW != SW_bit)) {
    Switch_state++;
    if (Switch_state == 4) Switch_state = 0;
    Led_State = Led_Init[Switch_state];
    light_leds(Led_State);
  }
  SW_bit = SW;
  update_cleo(Led_State); /* Finally update the CleO display */
}

/* Updates the CleO display based on the bits in the led_state variable */
void update_cleo(uint16_t led_state) {
  if (led_state == Prev_LED_State) return; /* Only update if there's a change */

  Prev_LED_State = led_state;
  int cx = 400;
  int cy = 240;
  CleO.Start();

  CleO.CircleExt(cx, cy, (cx - (BDR + DIA + WDTH * 2)) / 2 , (cx - (BDR + DIA)) / 2, DARK_ORANGE, MM, 0, 0);
  byte i = 0;
  for (float a = 0; i < DETENTS; a -= 360.0 / DETENTS, i++) {
    // If bit is set, update the LED else dont
    if (Z_BitIsSet(led_state, (0x0001 << i) ) ) {
      CleO.CircleExt(cx, cy, 0, DIA / 2, FORESTGREEN, MM, a, (cx - (BDR + DIA)) / 2 + DIA / 2);
    }
    else {
      CleO.CircleExt(cx, cy, 0, DIA / 2, LIGHT_BLUE, MM, a, (cx - (BDR + DIA)) / 2 + DIA / 2);
    }
  }

  CleO.SetBackgroundGradient(0, 0, cx * 2, cy * 2, DARK_BLUE, DARK_RED);
  CleO.Show();
}

/* Update the 74HC595 shift register over SPI */
void light_leds(uint16_t value) {

  
  sel();
  SPI.transfer(value & 0xFF); /* lsb */
  SPI.transfer(value >> 8);   /* msb */
  unsel();
  
}
