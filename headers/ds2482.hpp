#pragma once

#include "i2c-io.hpp"

#include <memory>

enum class COMMANDS{
    RESET = 0xf0,
    SET_READ_POINTER = 0xe1,
    WRITE_CONFIG = 0xd2,
    ONE_WIRE_SINGLE_BIT = 0x87,
    ONE_WIRE_RESET = 0xB4,
    ONE_WIRE_WRITE_BYTE = 0xA5,
    ONE_WIRE_READ_BYTE = 0x96,
    ONE_WIRE_TRIPLET = 0x78,
};

enum class REGISTER{
    PTR_CODE_STATUS = 0xf0,
    PTR_CODE_DATA = 0xe1,
    PTR_CODE_CONFIG = 0xc3,
};

enum class PTR_VALUES {
    PTR_CODE_STATUS = 0xf0,
    PTR_CODE_DATA = 0xE1,    
    PTR_CODE_CONFIG = 0xC3,
};

enum class SRBD { // Status Register Bit Definitions    
    STS_DIR = 0x80,
    STS_TSB = 0x40,
    TS_SBR = 0x20,
    STS_RST = 0x10,
    STS_LL = 0x08,
    STS_SD = 0x04,
    STS_PPD = 0x02,
    STS_1WB = 0x01,
};

class DS2482{
    const std::string m_logPrefix{"[ds2482] "};
    bool m_activePullUp = false;
    std::weak_ptr<i2cIO> m_ds2482;
    uint8_t m_config;
    REGISTER m_readPtr;
public:
    DS2482();
    DS2482(std::weak_ptr<i2cIO> && io);
    uint8_t calculate_config(uint8_t config);
    bool selectRegister(REGISTER reg);

    /*
        Waits until the 1-wire interface is idle (not busy)
        Return: the last value read from status or False (failure)
    */
    ssize_t wait1WireIdle();

    /*
        Performs the touch-bit function, which writes a 0 or 1 and reads the level.
            bit:	The level to write: 0 or non-zero
            Return:	The level read: 0 or 1
    */
    ssize_t W1TouchBit(bool bit);
    void  W1Writebit(bool bit);

    /*
        Performs the triplet function, which reads two bits and writes a bit.
            The bit written is determined by the two reads:
            00 => bit, 01 => 0, 10 => 1
            bit - The direction to choose if both branches are valid
            Return:	b0=read1 b1=read2 b3=bit written
    */
    uint8_t W1Triplet(bool bit);

    /*
        Performs the write byte function.
            byte - The value to write
    */
    void W1WriteByte(uint8_t byte);

    /*
        Performs the read byte function.
            Return:	The value read
    */
    ssize_t W1ReadByte();

    /*
        Sends a reset on the 1-wire interface
            Return:	True if sensors present on bus or False if not
    */
    bool W1ResetBus();
    bool waitConvertTComplete();
};