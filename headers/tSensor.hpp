#pragma once

#include <cstdint>
#include <mutex>
#include <string>

class tSensorInterface{
    float _currentT {0};
    std::mutex _mutex;
public:
    tSensorInterface() = default;
    tSensorInterface(tSensorInterface & other) = delete;
    virtual ~tSensorInterface();
    float getCurrentTemperature();
    void setCurrentTemperature(float && temperature);
};

class W1TSensor : public tSensorInterface{
    uint64_t _rom; /*unique id for each sensor*/
public:
    W1TSensor(const std::string & rom);
    W1TSensor(W1TSensor & other) = delete;
    bool operator<(const W1TSensor& other) const;
};