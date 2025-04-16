#pragma once 

#include <string>
#include <stdint.h>

class i2cIO{
    int m_file;
    std::string m_fileName;
public: 
    i2cIO(std::string && fileName);
    bool writeByte(uint8_t byte);
    bool writeByteToAddress(uint8_t address, uint8_t byte);
    ssize_t readByte();
    ssize_t readByteFromAddress(uint8_t address);
};