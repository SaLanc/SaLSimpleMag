

#include "Arduino.h"
#include "Baro.h"

Baro::Baro(uint8_t SSPin, uint8_t MOSIPin, uint8_t MISOPin, uint8_t SCKPin)
{
  pinMode(SSPin, OUTPUT);
  pinMode(SCKPin, OUTPUT);
  pinMode(MISOPin, INPUT);
  pinMode(MOSIPin, OUTPUT);
  _SSPin = SSPin;
  _MISOPin = MISOPin;
  _MOSIPin = MOSIPin;
  _SCKPin = SCKPin;
    digitalWrite(_SSPin, HIGH);


}

void Baro::readCoefficients() {
  for (uint8_t i = 0; i < 6; ++i) {
    coefficients_[i] = readCoefficient(i + 1);
  }

/*
  for (uint8_t i = 0; i < 6; ++i)
  {
    Serial.print("Coefficient ");
    Serial.print(i + 1, DEC);
    Serial.print(" : ");
    Serial.println(coefficients_[i], DEC);
  }
*/
  
  }

uint16_t Baro::readCoefficient(const uint8_t coefNum) {

  uint8_t _cmd = cmdPromRd_ + (coefNum * 2);
  digitalWrite(_SSPin, LOW);
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, _cmd);
  delayMicroseconds(20);
  const uint8_t _byte1 = shiftIn(_MISOPin, _SCKPin, MSBFIRST);
  const uint8_t _byte2 = shiftIn(_MISOPin, _SCKPin, MSBFIRST);
  digitalWrite(_SSPin, HIGH);
  uint16_t rC = _byte1;
  rC = (rC * 256) + _byte2;
  return rC;
}

int32_t Baro::readAdc(const uint8_t CMD_)
{
  digitalWrite(_SSPin, LOW); 
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, CMD_);
  digitalWrite(_SSPin, HIGH);
  delay(8);
  digitalWrite(_SSPin, LOW);
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, cmdAdcRead_);
  const uint8_t _byte1 = shiftIn(_MISOPin, _SCKPin, MSBFIRST);
  const uint8_t _byte2 = shiftIn(_MISOPin, _SCKPin, MSBFIRST);
  const uint8_t _byte3 = shiftIn(_MISOPin, _SCKPin, MSBFIRST);
  digitalWrite(_SSPin, HIGH);

  int32_t _receive = _byte1;
  _receive = (_receive * 256) + _byte2;
  _receive = (_receive * 256) + _byte3;

  return _receive;
}

void Baro::reset_Baro()
{
  digitalWrite(_SSPin, LOW);
  shiftOut(_MISOPin, _SCKPin, MSBFIRST, cmdReset_);
  digitalWrite(_SSPin, HIGH);
  delay(30);
}

int32_t Baro::getPres()
{


    const uint32_t temperature = readAdc(cmdAdcD2_);
    const uint32_t pressure    = readAdc(cmdAdcD1_);
    const uint32_t pressConv   = ConvertPressureTemperature(pressure, temperature);



  const int32_t AltCm = pascalToCent(pressConv);

  return AltCm;
}

int32_t Baro::pascalToCent(const int32_t pressurePa) {

  static const int32_t PZLUT_ENTRIES = 77;
  static const int32_t PA_INIT       = 104908;
  static const int32_t PA_DELTA      = 1024;

  static const int32_t lookupTable[PZLUT_ENTRIES] = {
    -29408, -21087, -12700,  -4244,   4279,
    12874,  21541,  30281,  39095,  47986,
    56953,  66000,  75126,  84335,  93628,
    103006, 112472, 122026, 131672, 141410,
    151244, 161174, 171204, 181335, 191570,
    201911, 212361, 222922, 233597, 244388,
    255300, 266334, 277494, 288782, 300204,
    311761, 323457, 335297, 347285, 359424,
    371719, 384174, 396795, 409586, 422552,
    435700, 449033, 462560, 476285, 490216,
    504360, 518724, 533316, 548144, 563216,
    578543, 594134, 609999, 626149, 642595,
    659352, 676431, 693847, 711615, 729752,
    748275, 767202, 786555, 806356, 826627,
    847395, 868688, 890537, 912974, 936037,
    959766, 984206
  };

  if (pressurePa > PA_INIT)
    return lookupTable[0];
  else
  {
    const int32_t inx = (PA_INIT - pressurePa) >> 10;
    if (inx >= PZLUT_ENTRIES - 1)
      return lookupTable[PZLUT_ENTRIES - 1];
    else
    {
      const int32_t pa1 = PA_INIT - (inx << 10);
      const int32_t z1 = lookupTable[inx];
      const int32_t z2 = lookupTable[inx + 1];
      return (z1 + (((pa1 - pressurePa) * (z2 - z1)) >> 10));
    }
  }
}

uint32_t Baro::ConvertPressureTemperature(uint32_t pressure, uint32_t temperature)
{
  const uint64_t C1 = static_cast<uint64_t>(coefficients_[0]);
  const uint64_t C2 = static_cast<uint64_t>(coefficients_[1]);
  const uint64_t C3 = static_cast<uint64_t>(coefficients_[2]);
  const uint64_t C4 = static_cast<uint64_t>(coefficients_[3]);
  const uint64_t C5 = static_cast<uint64_t>(coefficients_[4]);
  const uint64_t C6 = static_cast<uint64_t>(coefficients_[5]);

  // calcualte 1st order pressure and temperature (MS5607 1st order algorithm)
  const int32_t dT    = temperature - C5 * 256;
  const int32_t temp  = 2000 + (dT * C6) / pow(2, 23) ;

  const int64_t OFF   = (C2 * pow(2, 17)) + ((C4 * dT) / pow(2, 6));
  const int64_t SENS  = (C1 * pow(2, 16)) + ((C3 * dT) / pow(2, 7));
  const int32_t press = ((pressure * SENS / pow(2, 21) - OFF) / pow(2, 15));
  return press;
}
