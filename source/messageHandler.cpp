#include "../headers/messageHandler.hpp"
#include "messages.pb.h"

#include <chrono>

MessageHandler::MessageHandler(const uint64_t device_id) : m_device_id(device_id)
{

}

void MessageHandler::prepare_message(const uint64_t sensor_id,
                                const float temperature,
                                MessageTypes type,
                                std::string & result,
                                boost::system::error_code &ec)
{
    TemperatureMessage tMes;
    switch (type)
    {
        case MessageTypes::Temperature:            
            tMes.set_m_deviceid(m_device_id);
            tMes.set_m_sensorid(sensor_id);
            tMes.set_m_temperature(temperature);
            tMes.set_m_timestamp(getTimeStamp());
            tMes.set_m_type(MessageType::TEMPERATURE);
            tMes.SerializeToString(&result);
            break;
        default:
            //todo error handling later here
            break;
    }
}

int64_t MessageHandler::getTimeStamp()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}