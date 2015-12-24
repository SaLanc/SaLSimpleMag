#ifndef Memory_h
#define Memory_h

#include "Arduino.h"

class Memory
{
  public:
    Memory(uint8_t SSpin, uint8_t MOSIPin, uint8_t MISOPin, uint8_t SCKPin);
    uint8_t getData(uint8_t address);
    void eraseBlock();

    void storeData(unsigned long curTime, float alt);


  private:

    void writeByte(uint8_t byteToWrite, uint32_t byteLocation);
    uint8_t getByte(uint8_t address);
    uint8_t selectBlock();
    uint8_t currentBlock;
    uint8_t _SSPin1;
    uint8_t _MOSIPin;
    uint8_t _MISOPin;
    uint8_t _SCKPin;

    uint32_t addressFinder = 0x000000;
    uint32_t currentAddress;
    const byte OPCODE_SLOWREAD_ARRAY = 0x03;
    const byte OPCODE_WRITEENABLE    = 0x06;
    const byte OPCODE_PROGRAM        = 0x02;
    const byte OPCODE_04K_ERASE      = 0x20;  // Erase a  4K block

    const uint32_t memoryBlock[32] = {0x000000, 0x008000, 0x010000, 0x018000, 0x020000, 0x028000, 0x030000, 0x038000,
                                      0x040000, 0x048000, 0x050000, 0x058000, 0x060000, 0x068000, 0x070000, 0x078000,
                                      0x080000, 0x087000, 0x090000, 0x098000, 0x0A0000, 0x0A8000, 0x0B0000, 0x0B8000,
                                      0x0C0000, 0x0C8000, 0x0D0000, 0x0D8000, 0x0E0000, 0x0E8000, 0x0F0000, 0x0F8000
                                     };



};
#endif
