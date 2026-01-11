#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include <iostream>

#include "../headers/i2c-io.hpp"
#include "../headers/logger.hpp"

extern "C" {
    #include <linux/i2c-dev.h>
}

bool i2cId::operator<(const i2cId & other) const
{
    if (other.m_buss == m_buss)
    {
        return m_address < other.m_address;
    }
    else
    {
        return m_buss < other.m_buss;
    }
}

Filei2cIO::Filei2cIO(std::string && fileName)
{
    SHTLogger()->trace("Filei2cIO::Filei2cIO() - start");

    m_fileName =  fileName;
    
    m_file = open(m_fileName.c_str(), O_RDWR);
    if (m_file <= 0)
        SHTLogger()->error("can't open file [{}]", m_fileName);
    else 
        std::cout << "file " << m_fileName << " opened" << std::endl;

    SHTLogger()->trace("Filei2cIO::Filei2cIO()) - end");
}

Filei2cIO::~Filei2cIO()
{
    SHTLogger()->trace("Filei2cIO::~Filei2cIO())");
}

bool Filei2cIO::writeByte(uint8_t byte)
{
    static uint8_t toWrite[3];
    toWrite[0] = 0x00;
    toWrite[1] = byte;
    ssize_t ret =  write(m_file, &toWrite, 2);
    if (ret == 258)
    {
        SHTLogger()->error("i2c_smbus_write_byte return error");
        return false;
    }
    SHTLogger()->trace("Filei2cIO::writeByte - write byte [{}] - ok");
    return true;
}

bool Filei2cIO::writeByteToAddress(uint8_t address, uint8_t byte)
{
    static uint8_t toWrite[3];
    toWrite[0] = 0x00;
    toWrite[1] = address;
    toWrite[2] = byte;
    ssize_t ret =  write(m_file, &toWrite, 3);
    if(ret != 3)
    {
        SHTLogger()->error("eror - can't write byte [{}] data by address [{}]", (int)byte, address);
        return false;
    }
    else
    {
        SHTLogger()->trace("writeByteToAddress - successfull");
        return true;
    }
}

ssize_t Filei2cIO::readByte()
{
    static uint8_t toWrite[3];
    toWrite[0] = 0x01;
    ssize_t readedValue = write(m_file, &toWrite, 2);
    if (readedValue == 259)
    {
        SHTLogger()->error("error with reading byte");
        return 0;
    }
    else
    {
        SHTLogger()->trace("readByte return [{}]", readedValue);
        return readedValue;
    }
}

ssize_t Filei2cIO::readByteFromAddress(uint8_t address)
{
    static uint8_t toWrite[3];
    toWrite[0] = 0x01;
    toWrite[1] = address;
    ssize_t readedValue = write(m_file, &toWrite, 2);
    if (readedValue == 259)
    {
        SHTLogger()->error("error with reading byte");
        return 0;
    }
    else
    {
        SHTLogger()->trace("readed byte [{}] from address [{}] - ok", readedValue, (int)address);
        return readedValue;
    }
}

bool Filei2cIO::operator<(const Filei2cIO &other) const
{
    if(_context and other._context)
        return _context->m_i2cBuss < other._context->m_i2cBuss;
    else
    {
        /* todo later report error*/
    }
    return false;
}

bool MockI2cIO::writeByte(uint8_t byte)
{
    return true;
}

bool MockI2cIO::writeByteToAddress(uint8_t address, uint8_t byte)
{
    return true;
}

ssize_t MockI2cIO::readByte()
{
    return 0;
}

ssize_t MockI2cIO::readByteFromAddress(uint8_t address)
{
    return 0;
}

MockI2cIO::~MockI2cIO()
{
    SHTLogger()->trace("MockI2cIO::~MockI2cIO())");
}
