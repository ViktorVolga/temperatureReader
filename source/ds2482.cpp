#include "../headers/ds2482.hpp"
#include "../headers/logger.hpp"

DS2482::DS2482()
{
    SHTLogger()->info(m_logPrefix + "DS2482() - start");
    std::string ioFileName = "/dev/SH_TEMPERATURE_READER";
    //m_ds2482 = std::make_weak<i2cIO>(std::move(ioFileName));
}

DS2482::DS2482(std::weak_ptr<i2cIO> && io)
{
    m_ds2482 = std::move(io);
    SHTLogger()->info(m_logPrefix + "DS2482() - created");
}

uint8_t DS2482::calculate_config(uint8_t config)
{
    if(m_activePullUp)
        config |= 0x01;
    return config | ((~config & 0x0f) << 4);
}

bool DS2482::selectRegister(REGISTER reg)
{
    SHTLogger()->trace(m_logPrefix + "selectRegister() - start");
    if(m_readPtr != reg)
    {
        m_readPtr = reg;
        if (auto io = m_ds2482.lock())
            return io->writeByteToAddress(static_cast<uint8_t>(COMMANDS::SET_READ_POINTER), static_cast<uint8_t>(reg));
    }
    return true;
}

ssize_t DS2482::wait1WireIdle()
{
    SHTLogger()->trace(m_logPrefix + "wait1WireIdle() - start");
    ssize_t result = -1;
    int retries = 0;
    if (selectRegister(REGISTER::PTR_CODE_STATUS)){
        if (auto io = m_ds2482.lock())
            result = io->readByte();
        while(result >= 0 && (result & static_cast<ssize_t>(SRBD::STS_1WB)) && retries < 500)
        {
            if (auto io = m_ds2482.lock())
                result = io->readByte();
            retries += 1;
        }
    } else {
        retries = 500;
    }

    if(retries < 500){
        SHTLogger()->trace(m_logPrefix + "1wire buss ready");
        return result;
    } else {
        SHTLogger()->error(m_logPrefix + "1wire buss not ready");
        return -1;
    }
}

ssize_t DS2482::W1TouchBit(bool bit)
{
    wait1WireIdle();
    if(bit)
    {
        if (auto io = m_ds2482.lock())
            io->writeByteToAddress(static_cast<uint8_t>(COMMANDS::ONE_WIRE_SINGLE_BIT), 0xff);
    }
    else
    {
        if (auto io = m_ds2482.lock())
            io->writeByteToAddress(static_cast<uint8_t>(COMMANDS::ONE_WIRE_SINGLE_BIT), 0);
    }
    return wait1WireIdle();
}

void  DS2482::W1Writebit(bool bit){
    if(bit)
    {
        if(auto io = m_ds2482.lock())
        {
            io->writeByteToAddress(static_cast<uint8_t>(COMMANDS::ONE_WIRE_SINGLE_BIT), 0xff);
        }
    }
    else
    {
        if(auto io = m_ds2482.lock())
        {
            io->writeByteToAddress(static_cast<uint8_t>(COMMANDS::ONE_WIRE_SINGLE_BIT), 0);
        }
    }
}

uint8_t DS2482::W1Triplet(bool bit)
{
    wait1WireIdle();
    if(bit)
    {
        if(auto io = m_ds2482.lock())
            io->writeByteToAddress(static_cast<uint8_t>(COMMANDS::ONE_WIRE_TRIPLET), 0xff);
    }
    else
    {
        if(auto io = m_ds2482.lock())
            io->writeByteToAddress(static_cast<uint8_t>(COMMANDS::ONE_WIRE_TRIPLET), 0);
    }
    return (wait1WireIdle() >> 5);
}

void DS2482::W1WriteByte(uint8_t byte)
{
    wait1WireIdle();
    if(auto io = m_ds2482.lock())
        io->writeByteToAddress(static_cast<uint8_t>(COMMANDS::ONE_WIRE_WRITE_BYTE), byte);
}

ssize_t DS2482::W1ReadByte()
{
    wait1WireIdle();
    if(auto io = m_ds2482.lock())
        io->writeByte(static_cast<uint8_t>(COMMANDS::ONE_WIRE_READ_BYTE));
    wait1WireIdle();
    selectRegister(REGISTER::PTR_CODE_DATA);
    if(auto io = m_ds2482.lock())
        return io->readByte();
    //todo later - error report and handling
    return 0;
}

bool DS2482::W1ResetBus()
{
    wait1WireIdle();
    if(auto io = m_ds2482.lock())
        io->writeByte(static_cast<uint8_t>(COMMANDS::ONE_WIRE_RESET));
    return (wait1WireIdle() & 0x02);
}
