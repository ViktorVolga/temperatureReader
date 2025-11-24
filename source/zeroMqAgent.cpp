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
    std::string fullAddress;
    switch(connectionType)
    {
        case ConnectionType::ipc:
            fullAddress = "ipc:///" + address;
            break;
        case ConnectionType::tcp:
            fullAddress = "tcp://*:" + address;
            break;
    }
    zmq::context_t context(1);
    m_context = std::move(context);
    zmq::socket_t socket(m_context, zmq::socket_type::pub);
    m_socket = std::move(socket);
    try
    {
        m_socket.bind(fullAddress);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    SHTLogger()->info("Created publisher with address {}", address);
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

void TemperaturePublisher::publish(std::string & temperatureMessage)
{
    zmq::message_t  message(temperatureMessage);
    if(m_socket.send(message, zmq::send_flags::dontwait))
    {
        SHTLogger()->info("sended temperatureMessage");
    }
    else
    {
        SHTLogger()->error("erorr with sendidng temperature");
    }
}