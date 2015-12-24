/*
  data structure
  0x000000 - 0x007FFF
  0x008000 - 0x00FFFF
  0x010000 - 0x017FFF
  0x018000 - 0x01FFFF
  0x020000 - 0x027FFF
  0x028000 - 0x02FFFF
  0x030000 - 0x037FFF
  0x038000 - 0x03FFFF
  0x040000 - 0x047FFF
  0x048000 - 0x04FFFF
  0x050000 - 0x057FFF
  0x058000 - 0x05FFFF
  0x060000 - 0x067FFF
  0x068000 - 0x06FFFF
  0x070000 - 0x077FFF
  0x078000 - 0x07FFFF
  0x080000 - 0x087FFF
  0x087000 - 0x08FFFF
  0x090000 - 0x097FFF
  0x098000 - 0x09FFFF
  0x0A0000 - 0x0A7FFF
  0x0A8000 - 0x0AFFFF
  0x0B0000 - 0x0B7FFF
  0x0B8000 - 0x0BFFFF
  0x0C0000 - 0x0C7FFF
  0x0C8000 - 0x0CFFFF
  0x0D0000 - 0x0D7FFF
  0x0D8000 - 0x0DFFFF
  0x0E0000 - 0x0E7FFF
  0x0E8000 - 0x0EFFFF
  0x0F0000 - 0x0F7FFF
  0x0F8000 - 0x0FFFFF


*/

#include "Arduino.h"
#include "Memory.h"

Memory::Memory(uint8_t SSPin, uint8_t MOSIPin, uint8_t MISOPin, uint8_t SCKPin)
{
  pinMode(SSPin, OUTPUT);
  pinMode(SCKPin, OUTPUT);
  pinMode(MISOPin, INPUT);
  pinMode(MOSIPin, OUTPUT);

  _SSPin1 = SSPin;
  _MISOPin = MISOPin;
  _MOSIPin = MOSIPin;
  _SCKPin = SCKPin;

}

uint8_t Memory::getByte(uint8_t address)
{

  digitalWrite(_SSPin1, LOW);
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, OPCODE_SLOWREAD_ARRAY);
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((address & 0xFF0000) >> 16)); // Byte address - MSB Sig Byte
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((address & 0x00FF00) >>  8)); // Byte address - MID Sig Byte
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((address & 0x0000FF) >>  0)); // Byte address - LSB Sig Byte
  const uint8_t _byte1 = shiftIn(_MISOPin, _SCKPin, MSBFIRST);

  return _byte1;

}
void Memory::setNewAddress()
{
  Serial.println((long)currentAddress, HEX);
  currentAddress = 0x000001;
  //currentAddress =  currentAddress + 0x000001;
  Serial.println((long)currentAddress, HEX);
  eraseBlock();


  digitalWrite(_SSPin1, LOW);
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, OPCODE_WRITEENABLE);
  digitalWrite(_SSPin1, HIGH);
  digitalWrite(_SSPin1, LOW);
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, OPCODE_PROGRAM);
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((addressFinder & 0xFF0000) >> 16)); // Byte address - MSB Sig Byte
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((addressFinder & 0x00FF00) >>  8)); // Byte address - MID Sig Byte
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((addressFinder & 0x0000FF) >>  0)); // Byte address - LSB Sig Byte

  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((currentAddress & 0xFF0000) >> 16));
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((currentAddress & 0x00FF00) >>  8));
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((currentAddress & 0x0000FF) >>  0));
  digitalWrite(_SSPin1, HIGH);

}

void Memory::eraseBlock()
{

  digitalWrite(_SSPin1, LOW);
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, OPCODE_WRITEENABLE);
  digitalWrite(_SSPin1, HIGH);

  digitalWrite(_SSPin1, LOW);
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, OPCODE_04K_ERASE);
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((addressFinder & 0xFF0000) >> 16)); // Byte address - MSB Sig Byte
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((addressFinder & 0x00FF00) >>  8)); // Byte address - MID Sig Byte
  shiftOut(_MOSIPin, _SCKPin, MSBFIRST, ((addressFinder & 0x0000FF) >>  0)); // Byte address - LSB Sig Byte
  digitalWrite(_SSPin1, HIGH);
}

void Memory::storeData(unsigned long curTime, float alt)
{
  Serial.print("current time:");
  Serial.print(curTime);
  Serial.print("  current alt:");
  Serial.print(alt);
  Serial.println("");
}

uint8_t Memory::selectBlock()
{
  for (uint8_t i = 0; i < 32; ++i) {

    uint8_t blockHeader = getByte(memoryBlock[i]);

    Serial.print("Memory address: ");
    Serial.print(memoryBlock[i], HEX);
    Serial.print(" data: ");
    Serial.print(blockHeader);
    Serial.println(" ");
  }
}
void Memory::writeByte(uint8_t byteToWrite, uint32_t byteLocation)
{
  
}


