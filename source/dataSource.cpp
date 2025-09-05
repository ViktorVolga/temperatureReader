#include "../headers/dataSource.hpp"

DataSource::DataSource(const json &config, boost::system::error_code &ec)
{
    if(config.count("id") and config["id"].is_number_integer())
    {
        _id = static_cast<uint8_t>(config["id"].template get<int>());
    }
    else
    {
        ec = boost::system::error_code(errorsEnum::wrongDataSourceId, getErrorCategory());
        return;
    }

    if(config.count("type") and config["type"].is_string())
    {
        m_type = config["type"].template get<std::string>();
    }
    else
    {
        ec = boost::system::error_code(errorsEnum::wrongDataSourceType, getErrorCategory());
        return;
    }

    if(config.count("master device") and config["master device"].is_string())
    {
        m_masterDevice = config["master device"].template get<std::string>();
    }
    else 
    {
        ec = boost::system::error_code(errorsEnum::wrongDataSourceMasterDevice, getErrorCategory());
        return;
    }

    if(config.count("connection with master") and config["connection with master"].is_object())
    {
        ConnectionContextFabrica ccf;
        const json & connectionContextConfig = config.at("connection with master");
        m_connectionContextPtr = ccf.getConnectionContext(connectionContextConfig, ec);
    }
    else
    {
        ec = boost::system::error_code(errorsEnum::connectionConfigAbsent, getErrorCategory());
        return;
    }
}