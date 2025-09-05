#pragma once

#include "connectionContext.hpp"

//#include <nlohmann/json.hpp>

//#using json = nlohmann::json;
//just info about data sources
class DataSource{
public:
    MDConnectionContextPtr m_connectionContextPtr;
    uint8_t _id;

    /*list of supported: DS2482*/
    std::string m_masterDevice;

    /*list of supported: w1Buss*/
    std::string m_type;

    DataSource() = delete;
    DataSource(const DataSource & other) = delete;
    DataSource& operator=(const DataSource & other) = delete;

    DataSource(const json & config, boost::system::error_code & ec);
};