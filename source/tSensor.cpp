#include "../headers/tSensor.hpp"
#include "../headers/logger.hpp"

float tSensorInterface::getCurrentTemperature()
{
    SHTLogger()->trace("getCurrentTemperature()");
    const std::lock_guard<std::mutex> lock(_mutex);    
    return _currentT;
}

void tSensorInterface::setCurrentTemperature(float && temperature)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    _currentT = temperature;
}

W1TSensor::W1TSensor(const std::string &rom)
{
    SHTLogger()->trace("W1TSensor(), rom [{}]", rom);
    if(rom.empty())
    {
        SHTLogger()->error("W1TSensor(), rom is empty", rom);
        _rom = 0;
    }        
    _rom = stoull(rom);
    SHTLogger()->debug("ROM binary value from string [{}]. Rom uint64_t [{}]", rom, _rom);
}