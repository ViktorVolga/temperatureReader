#pragma once

#include "config.hpp"
#include "connectionHandler.hpp"

#include <memory>
#include <set>

class Application
{
    std::unique_ptr<ConnectionHandler> connHandler;
    std::unique_ptr<Config> _config;
public:
    Application(int argc, char * argv[]);
    void run();
private:
    /**
     * @brief Create connections to the data sources.
     * 
     * This function creates connections to the data sources defined in the configuration file.
     * It uses the ConnectionHandler class to create the connections.
     * 
     * @note This function is called by the Application constructor.
     * @note This function is called by the Application constructor.
     */
    void createConnections();
};