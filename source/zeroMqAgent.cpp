#include "../headers/zeroMqAgent.hpp"
#include "../headers/logger.hpp"

TemperaturePublisher::TemperaturePublisher(ConnectionType connectionType, std::string address)
{
    m_connectionType = connectionType;
    if(address.empty())
    {
        SHTLogger()->error("adress should be not empty");
        return;
    }
    switch(connectionType)
    {
        case ConnectionType::ipc:
            zmq::context_t context(1);
            m_context = std::move(context);
            zmq::socket_t socket(m_context, zmq::socket_type::pub);
            m_socket = std::move(socket);
            std::string fullAddress = "ipc://" + address;
            try
            {
                m_socket.bind(fullAddress);
            }
            catch(const zmq::error_t& e)
            {
                SHTLogger()->error("full adress {}", fullAddress);
            }
            SHTLogger()->info("Created ipc publisher with address {}", address);
            break;
    }
}

void TemperaturePublisher::publish(float temperature)
{
    zmq::message_t currentTemperature(std::to_string(temperature));
    if(m_socket.send(currentTemperature, zmq::send_flags::dontwait))
    {
        SHTLogger()->info("sended temperature {}", std::to_string(temperature));
    }
    else
    {
        SHTLogger()->error("erorr with sendidng temperature");
    }
}