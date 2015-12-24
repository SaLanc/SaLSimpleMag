#include "Arduino.h"
#include "Filter.h"

Filter::Filter(float peram1)
{
  for (uint8_t i = 0; i < 10; ++i) {
    AltStorage[i] = 0;
  }
  position = 0;

}

float Filter::filterAlt(float altInput)
{
  position++;
  if (position == 10) {
    position = 0;
  }
  AltStorage[position] = altInput;

  float movingSum = 0;

  for (uint8_t i = 0; i < 10; ++i) {
    movingSum += AltStorage[i];
  }

  float altOutput = movingSum / 10;

  return altOutput;
}

