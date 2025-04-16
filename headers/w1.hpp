#pragma once

#include "../headers/w1.hpp"
#include "../headers/ds2482.hpp"
#include "../headers/RomFinder.hpp"

enum class W1Commands : uint8_t{
    readROM = 0x33,
    matchRom = 0x55,
    scipRom = 0xCC,
    searchRom= 0xF0,
    alarmSearch = 0xEC,
};

enum class W1MemoryCommands : uint8_t{
    writeScratchpad = 0x4E,
    readScratchpad = 0xBE,
    copyScratchpad = 0x48,
    convertT = 0x44,
    recallE2 = 0xB8,
    readPowerSupply = 0xB4,
};

class w1BussInterface{
public:
    virtual void resetBuss() = 0;
    virtual float readTemperature(const std::string & ROM) = 0;
};

class w1Buss : w1BussInterface{
    std::unique_ptr<DS2482> _ds2482;
    std::unique_ptr<RomFinder> _romFinder;
public:
    w1Buss();
    void resetBuss() override;
    float readTemperature(const std::string & ROM) override;
};