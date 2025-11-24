#pragma once

#include <boost/core/noncopyable.hpp>
#include <boost/system.hpp>

enum class MessageTypes
{
    Temperature,
};

    //MessageType m_type = 1;
    //float m_temperature = 2;
    //int64 m_timeStamp = 3;
    //uint64 m_deviceId = 4;
    //uint64 m_sensorId = 5;

class MessageHandler : private boost::noncopyable
{
    const uint64_t m_device_id;
public:
    MessageHandler(const uint64_t device_id);
    void prepare_message (const uint64_t sensor_id,
                    const float temperature,
                    MessageTypes type,
                    std::string & result,
                    boost::system::error_code &ec);
    int64_t getTimeStamp();
};