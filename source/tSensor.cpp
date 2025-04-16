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