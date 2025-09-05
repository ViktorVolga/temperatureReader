#include "../headers/w1.hpp"
#include "../headers/logger.hpp"

w1Buss::w1Buss(DS2482* ds2482)
{
    SHTLogger()->trace("[w1Buss()] start");
    /*todo -read params from config module*/
    m_ds2482 = ds2482;
    m_romFinder = std::make_unique<RomFinder>(ds2482);
    SHTLogger()->trace("[w1Buss()] end");
}

void w1Buss::resetBuss()
{
    SHTLogger()->trace("[resetBuss()] start");
    if(m_ds2482 && m_ds2482->W1ResetBus())
        SHTLogger()->trace("buss reset -ok");
    else{
        if(!m_ds2482){
            SHTLogger()->error("_ds2482 - nullptr");
            return;
        }
        SHTLogger()->error("buss reset - error");
    }
    SHTLogger()->trace("[resetBuss()] end");
}

float w1Buss::readTemperature(const std::string & ROM)
{
    SHTLogger()->trace("readTemperature()] start, ROM [{}]", ROM);
    resetBuss();
    //_ds2482->W1WriteByte(static_cast<uint8_t>(W1Commands::matchRom));
    m_ds2482->W1WriteByte(static_cast<uint8_t>(W1Commands::scipRom));
    //for(const auto & c : ROM)
    //    _ds2482->W1Writebit(c == '1');
    m_ds2482->W1WriteByte(static_cast<uint8_t>(W1MemoryCommands::convertT));
    
    resetBuss();
    //_ds2482->W1WriteByte(static_cast<uint8_t>(W1Commands::matchRom));
    m_ds2482->W1WriteByte(static_cast<uint8_t>(W1Commands::scipRom));
    //for(const auto & c : ROM)
    //    _ds2482->W1Writebit(c == '1');
    m_ds2482->W1WriteByte(static_cast<uint8_t>(W1MemoryCommands::readScratchpad));
    std::vector<uint8_t> rawTemperature;
    int count = 0;
    while (count <= 8)
    {
        count += 1;
        /*todo -add here error cheking*/
        rawTemperature.push_back(static_cast<uint8_t>(m_ds2482->W1ReadByte()));
    }
    resetBuss();
    float temperature = 0;
    if(rawTemperature[0] & 0x80)
        temperature += 8;
    if(rawTemperature[0] & 0x40)
        temperature += 4;
    if(rawTemperature[0] & 0x20)
        temperature += 2;
    if(rawTemperature[0] & 0x10)
        temperature += 1;
    if(rawTemperature[0] & 0x08)
        temperature += 0.5;
    if(rawTemperature[0] & 0x04)
        temperature += 0.25;
    if(rawTemperature[0] & 0x02)
        temperature += 0.0625;
    if(rawTemperature[0] & 0x01)
        temperature += 0.0125;

    if(rawTemperature[1] & 0x04)
        temperature += 64;
    if(rawTemperature[1] & 0x02)
        temperature += 32;
    if(rawTemperature[1] & 0x01)
        temperature += 16;
    SHTLogger()->info("current temperature {}", temperature);
    return temperature;
}