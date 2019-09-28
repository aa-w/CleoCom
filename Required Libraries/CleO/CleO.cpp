#include <string.h>
#include <Arduino.h>
#include "SPI.h"
#include "CleO.h"

CleOClass CleO;

uint8_t *addstr(uint8_t *pc, const char *s)
{
  if(strlen(s) > MAX_STR_LEN) {
    strncpy((char*)pc, s, MAX_STR_LEN);
    pc[MAX_STR_LEN] = '\0';
  }
  else
    strcpy((char*)pc, s);

  return pc + strlen(s) + 1;
}
