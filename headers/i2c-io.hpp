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

/*
    interface
*/
class i2cIO
{
public:
    virtual bool writeByte(uint8_t byte) = 0;
    virtual bool writeByteToAddress(uint8_t address, uint8_t byte) = 0;
    virtual ssize_t readByte()= 0;
    virtual ssize_t readByteFromAddress(uint8_t address) = 0;
    virtual ~i2cIO() = default;
};

/*
    i2c read write frought special file
*/
class Filei2cIO : public i2cIO
{
    int m_file;
    std::string m_fileName;
    i2cContext * _context;
public: 
    Filei2cIO(std::string&& fileName);
    ~Filei2cIO();
    bool writeByte(uint8_t byte) override;
    bool writeByteToAddress(uint8_t address, uint8_t byte) override;
    ssize_t readByte() override;
    ssize_t readByteFromAddress(uint8_t address) override;
    bool operator<(const Filei2cIO & other) const;
};

class MockI2cIO : public i2cIO
{
public:
    ~MockI2cIO();
    bool writeByte(uint8_t byte) override;
    bool writeByteToAddress(uint8_t address, uint8_t byte) override;
    ssize_t readByte() override;
    ssize_t readByteFromAddress(uint8_t address) override;
    bool operator<(const Filei2cIO & other) const;
};