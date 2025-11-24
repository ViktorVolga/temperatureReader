#pragma once

#include <zmq.hpp>
#include <boost/core/noncopyable.hpp>

enum class ConnectionType
{
    ipc,
    udp,
    tcp,
    pgm
};


class TemperaturePublisher : private boost::noncopyable
{
    zmq::socket_t m_socket;
    zmq::context_t m_context;
    std::string m_Address;
    ConnectionType m_connectionType;
public:
    TemperaturePublisher(ConnectionType connectionType, std::string adress);
    void publish(const float temperature);
    void publish(std::string& temperatureMessage);
};

