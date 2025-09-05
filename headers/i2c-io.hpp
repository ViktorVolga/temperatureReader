#pragma once 

#include <string>
#include <stdint.h>

#include "connectionContext.hpp"

struct i2cId{
    uint8_t m_buss;
    uint8_t m_address;
    i2cId(uint8_t buss, uint8_t address) : m_buss(buss), m_address(address){};
    bool operator<(const i2cId & other) const;
};

class i2cIO{
    int m_file;
    std::string m_fileName;
    i2cContext * _context;
public: 
    i2cIO(std::string && fileName);
    bool writeByte(uint8_t byte);
    bool writeByteToAddress(uint8_t address, uint8_t byte);
    ssize_t readByte();
    ssize_t readByteFromAddress(uint8_t address);
    bool operator<(const i2cIO & other) const;
};