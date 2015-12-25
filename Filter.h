#ifndef Filter_h
#define Filter_h

#include "Arduino.h"

class Filter
{
  public:
    Filter(uint32_t peram1);
    float filterAlt(float altInput);

  private:
    float AltStorage[10];
    uint8_t position;

};
#endif
