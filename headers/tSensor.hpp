#pragma once

#include <cstdint>
#include <mutex>

class tSensorInterface{
    float _currentT {0};
    std::mutex _mutex;
public:
    tSensorInterface() = default;
    virtual ~tSensorInterface();
    float getCurrentTemperature();
    void setCurrentTemperature(float && temperature);
};

class W1TSensor : public tSensorInterface{
    uint64_t _rom; /*unique id for each sensor*/
public:
    
};