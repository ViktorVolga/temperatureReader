#include "../headers/app.hpp"
#include "../headers/programOptions.hpp"
#include "../headers/logger.hpp"
#include "../headers/zeroMqAgent.hpp"

#include <thread>
#include <chrono>

Application::Application(int argc, char *argv[])
{
    ProgramOptions po(argc, argv);
    po.init();

    std::string path = po.getConfigPath();

    if(path.empty())
        return;

    _config = std::make_unique<Config>(path, false);
    connHandler = std::make_unique<ConnectionHandler> ();
    createConnections();
}

void Application::run()
{
    boost::system::error_code ec;
    std::weak_ptr<w1Buss> bussPtr = connHandler->getW1Buss(0x01, 0x18, ec);
    if(ec)
    {
        SHTLogger()->error("{}, buss 0x01, address 0x18", ec.what());
    }
    std::string rom{""};
    TemperaturePublisher publisher(ConnectionType::tcp, "6666");
    while(true)
    {
        if(auto buss = bussPtr.lock())
        {
            using namespace std::chrono_literals;
            auto temperature = buss->readTemperature(rom);
            publisher.publish(temperature);
            std::this_thread::sleep_for(1s);
        }
        else
        {
            break;
        }
    }
}

void Application::createConnections()
{
    const std::vector<std::unique_ptr<DataSource>> & dataSources = _config->getDataSources();
    for(const auto & dataSource : dataSources)
    {
        connHandler->addDataSource(dataSource.get());
    }
}