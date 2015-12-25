#ifndef Baro_h
#define Baro_h

#include "Arduino.h"

class Baro
{
  public:
    Baro(uint8_t SSpin, uint8_t MOSIPin, uint8_t MISOPin, uint8_t SCKPin);
    int32_t readAdc(uint8_t CMD_);
    void reset_Baro();
    float getPres();
    void readCoefficients();

  private:
    uint8_t cmdReset_   = 0x1E;
    uint8_t cmdAdcRead_ = 0x00;
    uint8_t cmdAdcConv_ = 0x40;
    uint8_t cmdAdcD1_   = 0x40;
    uint8_t cmdAdcD2_   = 0x50;
    uint8_t cmdAdc256_  = 0x00;
    uint8_t cmdAdc512_  = 0x02;
    uint8_t cmdAdc1024_ = 0x04;
    uint8_t cmdAdc2048_ = 0x06;
    uint8_t cmdAdc4096_ = 0x08;
    uint8_t cmdPromRd_  = 0xA0;
    unsigned int coefficients_[6];

    uint8_t _SSPin;
    uint8_t _MOSIPin;
    uint8_t _MISOPin;
    uint8_t _SCKPin;

    int32_t pascalToCent(int32_t pressurePa);
    uint32_t ConvertPressureTemperature(uint32_t pressure, uint32_t temperature);
    uint16_t  readCoefficient(uint8_t coefNum);

};
#endif
