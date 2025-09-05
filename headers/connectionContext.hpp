#pragma once

#include <string>
#include <memory>

#include <nlohmann/json.hpp>

#include "errors.hpp"

/*
    Here will be defined connection context with master devices.
    Each master device is provider data from temperature sensors.
*/

enum class connectionType
{
    i2c = 0x01,
    error = 0xFF,
};

static std::map<std::string, connectionType> connectionTypesMap {
    {"i2c", connectionType::i2c}
};

/*MasterDevice - MD*/
class MDConnectionContext{
public:
    std::string m_connectionType;

    MDConnectionContext(const MDConnectionContext & other) = delete;
    MDConnectionContext& operator=(const MDConnectionContext&) = delete;
    MDConnectionContext() = delete;
    MDConnectionContext(const std::string & type) : m_connectionType(type) {};
    MDConnectionContext(std::string && type) : m_connectionType(std::move(type)) {};

    const std::string & getTypeString();
    connectionType getConnectionType() const;

    virtual ~MDConnectionContext();
};

class i2cContext : public MDConnectionContext{
public:
    uint8_t m_i2cBuss;
    uint8_t m_address;
    std::string _devicePath;

    i2cContext(const i2cContext & other) = delete;
    i2cContext& operator=(const i2cContext & other) = delete;
    bool operator<(const i2cContext & other) const;
    i2cContext() = delete;
    i2cContext(i2cContext && other);
    i2cContext(uint8_t buss,
                uint8_t address,
                const char * devicePath);
};

using MDConnectionContextPtr = std::unique_ptr<MDConnectionContext>;

using json = nlohmann::json;

class ConnectionContextFabrica{
public:
    MDConnectionContextPtr getConnectionContext(const json & connectionContext, boost::system::error_code & ec);
};

