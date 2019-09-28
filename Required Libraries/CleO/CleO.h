#include <SPI.h>
#include <string.h>

#include "common.h"
#include "Bridge.h"
#define REPORT(VAR) (Serial.print(#VAR "="), Serial.println(VAR, HEX))

#define CLEO_FILE   0xfc
#define CLEO_DIR    0xfd

typedef struct
{
  char filename[244];
  uint16_t type;
  uint32_t length;
}file_info_t;

#if defined (_VARIANT_AMEL_SMARTEVERYTHING_) 
#define Serial                      SerialUSB
#endif

/* Default IO configurations */
#define _CLEO_CHIPSELECTPIN_ (10)
#define _CLEO_INTSELECTPIN_ (3)

#if !defined(__AVR_ATmega328P__)
#define MOSI  11
#define MISO  12
#define SCK   13    // B.27

class ASPI_t {
public:
  void begin(void) {
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, INPUT_PULLUP);
    pinMode(SCK, OUTPUT);

    digitalWrite(SCK, 0);
    digitalWrite(MOSI, 0);
  }
  uint8_t transfer(uint8_t x ) {
    uint8_t r = 0;

    for (uint8_t i = 0; i< 8; i++) {
       digitalWrite(MOSI, (x >> 7) & 1);
      r <<= 1;
      r |= digitalRead(MISO);

       digitalWrite(SCK, 1);
      //r |= digitalRead(MISO);
       digitalWrite(SCK, 0);

      x <<= 1;
    }

    return r;
  }
};
static class ASPI_t ASPI;
#define SPI ASPI

#endif

static int16_t __lnum;

extern uint8_t *addstr(uint8_t *pc, const char *s);

class CleOClass : public Bridge
{
public:
    CleOClass(uint8_t CSPin = _CLEO_CHIPSELECTPIN_ , uint8_t IntPin = _CLEO_INTSELECTPIN_)
    {
      _Cleo_chipSelectPin = CSPin;
      _Cleo_intSelectPin = IntPin;
    }

  int begin(void) {
    pinMode(_Cleo_chipSelectPin, OUTPUT);
    pinMode(_Cleo_intSelectPin, INPUT);
    digitalWrite(_Cleo_chipSelectPin, 1);
#if !defined(__AVR_ATmega328P__)
    Serial.println("SPI Bitbang enabled");
    ASPI.begin();
#else
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
#endif

    // initalize the  data ready and chip select pins:
    pinMode(_Cleo_chipSelectPin, OUTPUT);
    pinMode(_Cleo_intSelectPin, INPUT);
    unsel();
    delay(1);

    uint16_t init = RX2();
    if (init != 0x3412) {
      return -1;
    }
    uint16_t r0, r1;
    do {
      r0 = Noop();
      r1 = Echo();
      Serial.print("r0=");
      Serial.println(r0, HEX);
      Serial.print("r1=");
      Serial.println(r1, HEX);
      Serial.println();
    } while ((r0 != 0x0000) || (r1 != 0x1d7f));
    Serial.println("synced");
    return 0;
  }

#include "encoder.i"

private:
  uint8_t _Cleo_chipSelectPin;
  uint8_t _Cleo_intSelectPin;

  void sel(void) {
    digitalWrite(_Cleo_chipSelectPin, 0);
  }

  void unsel(void) {
    digitalWrite(_Cleo_chipSelectPin, 1);
  }

  union {
    uint8_t c2nbuf[256];
    uint16_t c2nbuf_w[128];
  };

  void TX(uint8_t *a, size_t b) {
    size_t i;
    sel();
    SPI.transfer(b);

    SPI.transfer(b >> 8);
    for (i = 0; i < b; i++) {
      if ((i & 63) == 0) {
        // wait for a high signal - slave ready
        while (digitalRead(_Cleo_intSelectPin) == 0)
          ;
      }
      SPI.transfer(a[i]);
    }
    unsel();
  }

  uint8_t collect(void) {
    uint8_t r = SPI.transfer(0xff);
    return r;
  }

  void RX(int n) {
    sel();
    for (int i = 0; i < n; i++) {
      if ((i & 63) == 0) {
        while (digitalRead(_Cleo_intSelectPin) == 0) // wait for a high signal - slave ready
          ;
      }
      c2nbuf[i] = collect();
    }
    unsel();

    while (digitalRead(_Cleo_intSelectPin) == 1) // Wait for slave to complete MISO transaction before we move on
      ;
  }

  int RX2() {
    RX(2);
    return c2nbuf_w[0];
  }
  
  uint16_t RX3(uint8_t* pData)
  {
    sel();
    while (digitalRead(_Cleo_intSelectPin) == 0) // wait for a high signal - slave ready
    ;
    // first 2 bytes gives you the payload length
    uint16_t n = collect() | ((uint16_t)collect() << 8);
    //Serial.println("read:");
    //Serial.print(n, HEX);
    for (uint16_t i = 2; i < n + 2; i++) {
      if ((i & 63) == 0) {
        while (digitalRead(_Cleo_intSelectPin) == 0) // wait for a high signal - slave ready
          ;
      }
      *pData++ = collect();
      //Serial.print(*(pData-1), HEX);
    }
    unsel();

    while (digitalRead(_Cleo_intSelectPin) == 1){ // Wait for slave to complete MISO transaction before we move on
      //Serial.print('w');
      ;
    }
#if 0
    // first 2 bytes gives you the length of data to read
    uint16_t rx_len = collect() | ((uint16_t)collect() << 8);
    for(b = 0; b < rx_len; b++){
        *pData++ = collect();
    }
    unsel();
#endif
    return n;
  }
};

extern CleOClass CleO;
